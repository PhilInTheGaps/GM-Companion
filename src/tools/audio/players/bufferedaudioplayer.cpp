#include "bufferedaudioplayer.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioBufferedPlayer, "gm.audio.buffered")

BufferedAudioPlayer::BufferedAudioPlayer(const QString &settingsId, QNetworkAccessManager &networkManager,
                                         QObject *parent)
    : AudioPlayer(parent), m_settingsId(settingsId),
      m_playlist(std::make_unique<ResolvingAudioPlaylist>(settingsId, networkManager))
{
    m_mediaPlayer.setAudioOutput(&m_audioOutput);

    connect(&m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &BufferedAudioPlayer::onMediaPlayerPlaybackStateChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &BufferedAudioPlayer::onMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, &BufferedAudioPlayer::onMediaPlayerErrorOccurred);
    connect(&m_mediaPlayer, &QMediaPlayer::metaDataChanged, this,
            [this]() { emit metaDataChanged(m_mediaPlayer.metaData()); });

    connect(this, &BufferedAudioPlayer::playlistIndexChanged, this,
            [](int index) { qCDebug(gmAudioBufferedPlayer()) << index; });
}

auto BufferedAudioPlayer::playlistQml() -> QQmlListProperty<AudioFile>
{
    return m_playlist->filesQml(this);
}

auto BufferedAudioPlayer::element() const -> QPointer<AudioElement>
{
    return m_element;
}

void BufferedAudioPlayer::setIndex(int index)
{
    playlistIndex(index);

    if (Utils::isInBounds(m_playlist->files(), index))
    {
        const auto *file = m_playlist->at(index);

        if (file)
        {
            loadMedia(*file);
        }
        else
        {
            next();
        }
    }
}

void BufferedAudioPlayer::play(AudioElement *element)
{
    m_mediaPlayer.stop();
    playlistIndex(0);

    m_element = element;

    if (!m_element)
    {
        qCWarning(gmAudioBufferedPlayer()) << "Error: Could not play element, it is null";
        return;
    }

    loadPlaylist().then([this]() { startPlaying(); });
}

void BufferedAudioPlayer::play()
{
    m_mediaPlayer.play();
}

void BufferedAudioPlayer::play(const QByteArray &data)
{
    m_mediaPlayer.stop();

    if (m_mediaBuffer)
    {
        m_mediaBuffer->close();
    }

    m_mediaBuffer = std::make_unique<QBuffer>();
    m_mediaBuffer->setData(data);
    m_mediaBuffer->open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(m_mediaBuffer.get());

    state(AudioPlayer::State::Playing);

    m_audioOutput.setMuted(false);
    m_mediaPlayer.play();

    emit currentFileChanged(m_playlist->at(playlistIndex())->url(), data);
}

auto BufferedAudioPlayer::loadPlaylist() -> QFuture<void>
{
    m_playlist->setFiles(m_element->files());
    return m_playlist->resolve().then([this]() { applyShuffleMode(); });
}

void BufferedAudioPlayer::handleUnsupportedMediaSource(const AudioFile &file)
{
    qCWarning(gmAudioBufferedPlayer()) << "Media type" << file.source() << "is currently not supported.";
    next();
}

void BufferedAudioPlayer::onFileReceived(Files::FileDataResult &&result)
{
    if (!result.success() || result.data().isEmpty())
    {
        next();
        return;
    }

    play(result.data());
}

auto BufferedAudioPlayer::fileSource() const -> AudioFile::Source
{
    return m_currentFileSource;
}

void BufferedAudioPlayer::setPlaylist(std::unique_ptr<ResolvingAudioPlaylist> playlist)
{
    m_playlist = std::move(playlist);
    emit playlistChanged();
}

void BufferedAudioPlayer::pause()
{
    if (state() == AudioPlayer::State::Paused) return;

    m_mediaPlayer.pause();
    state(AudioPlayer::State::Paused);
}

void BufferedAudioPlayer::stop()
{
    if (state() == AudioPlayer::State::Stopped) return;

    m_mediaPlayer.stop();
    state(AudioPlayer::State::Stopped);

    m_playlist->clear();
}

void BufferedAudioPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(linear)
    m_audioOutput.setVolume(normalizeVolume(logarithmic));
}

void BufferedAudioPlayer::again()
{
    m_mediaPlayer.setPosition(0);
}

void BufferedAudioPlayer::next()
{
    if (!m_element || m_playlist->isEmpty())
    {
        stop();
        return;
    }

    // Complete random
    if (m_element->mode() == AudioElement::Mode::Random)
    {
        auto index = QRandomGenerator::system()->bounded(m_playlist->length());
        setIndex(index);
        return;
    }

    // choose next in line (mode RandomList, ListLoop, ListOnce)
    if (playlistIndex() + 1 < m_playlist->length())
    {
        setIndex(playlistIndex() + 1);
        return;
    }

    // loop around (mode RandomList or ListLoop)
    if (m_element->mode() != AudioElement::Mode::ListOnce)
    {
        setIndex(0);
        return;
    }

    // reached end of playlist, stop
    stop();
}

void BufferedAudioPlayer::onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    qCDebug(gmAudioBufferedPlayer) << "Media player playback state changed:" << newState;
    if (newState == QMediaPlayer::PlayingState) state(State::Playing);
}

void BufferedAudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCDebug(gmAudioBufferedPlayer()) << "Status changed:" << status;

    switch (status)
    {
    case QMediaPlayer::EndOfMedia:
        qCDebug(gmAudioBufferedPlayer()) << "End of media was reached, playing next file ...";
        next();
        break;
    case QMediaPlayer::BufferingMedia:
        state(State::Loading);
        break;
    default:
        break;
    }
}

void BufferedAudioPlayer::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qCWarning(gmAudioBufferedPlayer()) << error << errorString;

    if (error != QMediaPlayer::NoError)
    {
        next();
    }
}

void BufferedAudioPlayer::startPlaying()
{
    if (!m_element || m_playlist->isEmpty()) return;

    if (m_element->mode() == AudioElement::Mode::Random)
    {
        next();
        return;
    }

    setIndex(0);
}

void BufferedAudioPlayer::loadMedia(const AudioFile &file)
{
    qCDebug(gmAudioBufferedPlayer()) << "Loading media (" << file.url() << ") ...";

    m_mediaPlayer.stop();
    state(AudioPlayer::State::Loading);

    m_currentFileSource = file.source();

    switch (m_currentFileSource)
    {
    case AudioFile::Source::File:
        loadLocalFile(file);
        break;
    case AudioFile::Source::Web:
        loadWebFile(file);
        break;
    default:
        handleUnsupportedMediaSource(file);
        break;
    }
}

void BufferedAudioPlayer::loadLocalFile(const AudioFile &file)
{
    const auto path = FileUtils::fileInDir(file.url(), SettingsManager::getPath(m_settingsId));
    const auto callback = [this](Files::FileDataResult &&result) { onFileReceived(std::move(result)); };

    Files::File::getDataAsync(path).then(callback);
}

void BufferedAudioPlayer::loadWebFile(const AudioFile &file)
{
    m_mediaPlayer.setSource(QUrl(file.url()));
    m_mediaPlayer.play();
    m_audioOutput.setMuted(false);
}

void BufferedAudioPlayer::applyShuffleMode()
{
    qCDebug(gmAudioBufferedPlayer()) << "Applying shuffle mode" << m_element->mode();

    if (m_element->mode() != AudioElement::Mode::RandomList) return;

    m_playlist->shuffle();
    emit playlistChanged();
}
