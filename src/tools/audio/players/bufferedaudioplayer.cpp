#include "bufferedaudioplayer.h"
#include "../thumbnails/loaders/youtubeimageloader.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "services/youtube/youtube.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmAudioBufferedPlayer, "gm.audio.buffered")

BufferedAudioPlayer::BufferedAudioPlayer(const QString &settingsId, QNetworkAccessManager *networkManager,
                                         QObject *parent)
    : AudioPlayer(parent), m_networkManager(networkManager), m_settingsId(settingsId),
      m_playlist(std::make_unique<ResolvingAudioPlaylist>(settingsId, networkManager))
{
    m_mediaPlayer.setAudioOutput(&m_audioOutput);

    connect(&m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &BufferedAudioPlayer::onMediaPlayerPlaybackStateChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &BufferedAudioPlayer::onMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, &BufferedAudioPlayer::onMediaPlayerErrorOccurred);

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

void BufferedAudioPlayer::setIndex(qsizetype index)
{
    playlistIndex(index);

    if (Utils::isInBounds(m_playlist->files(), index))
    {
        auto *file = m_playlist->at(index);

        if (file)
        {
            loadMedia(*file);
        }
        else
        {
            next(true);
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
    next(true);
}

void BufferedAudioPlayer::onFileReceived(const Files::FileDataResult &result)
{
    if (!result.success() || result.data().isEmpty())
    {
        next(true);
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

void BufferedAudioPlayer::next(bool withError)
{
    if (!m_element || m_playlist->isEmpty())
    {
        stop();
        return;
    }

    if (withError)
    {
        auto *file = m_playlist->at(playlistIndex());
        if (file)
        {
            file->hadError(true);
        }

        if (!m_playlist->hasElementsWithoutErrors())
        {
            stop();
            return;
        }
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
    if (newState == QMediaPlayer::PlayingState)
    {
        state(State::Playing);

        auto *file = m_playlist->at(playlistIndex());
        if (file)
        {
            file->hadError(false);
        }
    }
}

void BufferedAudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCDebug(gmAudioBufferedPlayer()) << "Status changed:" << status;

    switch (status)
    {
    case QMediaPlayer::EndOfMedia:
        qCDebug(gmAudioBufferedPlayer()) << "End of media was reached, playing next file ...";
        next(false);
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

    if (error == QMediaPlayer::ResourceError && m_currentFileSource == AudioFile::Source::Youtube)
    {
        // if the stream can not be read, change the piped instance
        Services::YouTube::instance()->selectNewPipedInstance();
    }

    if (error != QMediaPlayer::NoError)
    {
        next(true);
    }
}

void BufferedAudioPlayer::startPlaying()
{
    if (!m_element || m_playlist->isEmpty()) return;

    if (m_element->mode() == AudioElement::Mode::Random)
    {
        next(false);
        return;
    }

    setIndex(0);
}

void BufferedAudioPlayer::loadMedia(AudioFile &file)
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
    case AudioFile::Source::Youtube:
        loadYouTubeFile(file);
        break;
    default:
        handleUnsupportedMediaSource(file);
        break;
    }
}

void BufferedAudioPlayer::loadLocalFile(const AudioFile &file)
{
    const auto path = FileUtils::fileInDir(file.url(), SettingsManager::getPath(m_settingsId));
    const auto callback = [this](const Files::FileDataResult &result) { onFileReceived(result); };

    Files::File::getDataAsync(path, Files::Option::AllowCache).then(callback);
}

void BufferedAudioPlayer::loadWebFile(const AudioFile &file)
{
    loadWebFile(file.url());
}

void BufferedAudioPlayer::loadWebFile(const QString &url)
{
    m_mediaPlayer.stop();
    m_mediaPlayer.setPosition(0);
    m_mediaPlayer.setSource(QUrl()); // set null url to reset state
    m_mediaPlayer.setSource(QUrl(url));
    m_mediaPlayer.play();
    m_audioOutput.setMuted(false);

    QMediaMetaData metaData;
    metaData.insert(QMediaMetaData::Key::Title, "-");
    metaData.insert(QMediaMetaData::Key::Author, "-");
    metaData.insert(QMediaMetaData::Key::AlbumTitle, "-");
    metaData.insert(QMediaMetaData::Key::CoverArtImage, QImage());
    emit metaDataChanged(metaData);
}

void BufferedAudioPlayer::loadYouTubeFile(AudioFile &file)
{
    const Services::VideoId id(file.url());
    if (!id.isValid())
    {
        next(true);
        return;
    }

    Services::YouTube::instance()
        ->getStreamInfoAsync(id, Services::Option::None)
        .then([this, &file](const Services::YouTubeVideo &video) {
            if (video.audioStreamUrl.isEmpty())
            {
                next(true);
                return;
            }

            loadWebFile(video.audioStreamUrl);

            file.title(video.title);

            QMediaMetaData metaData;
            metaData.insert(QMediaMetaData::Key::Title, video.title);
            metaData.insert(QMediaMetaData::Key::Author, video.uploader);
            emit metaDataChanged(metaData);

            YouTubeImageLoader::loadImageAsync(video, m_networkManager)
                .then([this, metaData](const QPixmap &thumbnail) mutable {
                    metaData.insert(QMediaMetaData::Key::ThumbnailImage, thumbnail.toImage());
                    emit metaDataChanged(metaData);
                });
        })
        .onCanceled([this]() { next(true); });
}

void BufferedAudioPlayer::applyShuffleMode()
{
    qCDebug(gmAudioBufferedPlayer()) << "Applying shuffle mode" << m_element->mode();

    if (m_element->mode() == AudioElement::Mode::RandomList)
    {
        m_playlist->shuffle();
    }

    emit playlistChanged();
}
