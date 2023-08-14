#include "radioplayer.h"
#include "../playlist/audioplaylist.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;

static constexpr auto BUFFER_FULL = 100;

Q_LOGGING_CATEGORY(gmAudioRadio, "gm.audio.radio")

RadioPlayer::RadioPlayer(QNetworkAccessManager &networkManager, MetaDataReader &metaDataReader, QObject *parent)
    : AudioPlayer(parent), m_networkManager(networkManager)
{
    qCDebug(gmAudioRadio()) << "Loading RadioPlayer ...";

    m_mediaPlayer.setObjectName(tr("Radio"));
    m_mediaPlayer.setAudioOutput(&m_audioOutput);

    connect(&m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &RadioPlayer::onMediaPlayerPlaybackStateChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &RadioPlayer::onMediaPlayerMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, &RadioPlayer::onMediaPlayerErrorOccurred);

    // MetaData
    connect(&m_mediaPlayer, &QMediaPlayer::metaDataChanged, this, &RadioPlayer::onMetaDataChanged);
    connect(this, &RadioPlayer::metaDataChanged, &metaDataReader,
            QOverload<const QMediaMetaData &>::of(&MetaDataReader::updateMetaData));
}

/**
 * @brief Play a radio element
 * @param element RadioElement to be played
 */
void RadioPlayer::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioRadio()) << "Playing radio element:" << element->name();

    m_mediaPlayer.stop();
    m_mediaPlayer.setObjectName(tr("Radio") + ": " + element->name());
    m_currentElement = element;

    if (element->files().empty()) return;

    const auto *audioFile = element->files().constFirst();
    play(audioFile);
}

void RadioPlayer::play(const AudioFile *audioFile)
{
    m_fileName = audioFile->url();

    if (audioFile->source() == AudioFile::Source::File)
    {
        qCDebug(gmAudioRadio()) << "Playing radio from local playlist:" << audioFile->url() << "...";

        if (m_fileRequestContext) m_fileRequestContext->deleteLater();
        m_fileRequestContext = new QObject(this);

        m_fileName = audioFile->url();
        Files::File::getDataAsync(FileUtils::fileInDir(audioFile->url(), SettingsManager::getPath(u"radio"_s)))
            .then(m_fileRequestContext, [this](Files::FileDataResult *result) { onFileReceived(result); });
        return;
    }

    qCDebug(gmAudioRadio()) << "Playing radio from url:" << audioFile->url();

    if (isPlaylist(audioFile->url()))
    {
        auto *reply = m_networkManager.get(QNetworkRequest(QUrl(audioFile->url())));
        QtFuture::connect(reply, &QNetworkReply::finished).then(this, [this, reply]() {
            onFileReceived(new Files::FileDataResult(reply->readAll(), this));
            reply->deleteLater();
        });
        return;
    }

    m_mediaPlayer.setSource(QUrl(audioFile->url()));
    m_audioOutput.setMuted(false);
    m_mediaPlayer.play();
}

/**
 * @brief Start playing the radio element
 */
void RadioPlayer::play()
{
    qCDebug(gmAudioRadio()) << "Continue play of RadioPlayer ...";
    m_mediaPlayer.play();
}

/**
 * @brief Pause the playback of the radio element
 */
void RadioPlayer::pause()
{
    qCDebug(gmAudioRadio()) << "Pausing RadioPlayer ...";
    m_mediaPlayer.pause();
}

/**
 * @brief Stop radio playback
 */
void RadioPlayer::stop()
{
    qCDebug(gmAudioRadio()) << "Stopping RadioPlayer ...";

    m_mediaPlayer.stop();
}

void RadioPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(linear)
    m_audioOutput.setVolume(normalizeVolume(logarithmic));
}

auto RadioPlayer::isPlaylist(const QString &file) -> bool
{
    return file.endsWith(".m3u"_L1) || file.endsWith(".m3u8"_L1) || file.endsWith(".pls"_L1);
}

void RadioPlayer::playAudio(const QByteArray &data)
{
#ifdef Q_OS_WIN
    QFile file(m_tempDir.path() + "/" + FileUtils::fileName(m_fileName));

    if (!file.open(QIODevice::WriteOnly))
    {
        if (file.error() == QFileDevice::OpenError)
        {
            file.setFileName(FileUtils::incrementFileName(file.fileName()));
            if (!file.open(QIODevice::WriteOnly))
            {
                qCWarning(gmAudioRadio()) << "Error: Could not open temporary file even after incrementing the filename"
                                          << file.fileName() << file.errorString();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioRadio()) << "Error: Could not open temporary file:" << file.fileName()
                                      << file.errorString();
            return;
        }
    }

    qCWarning(gmAudioRadio()) << file.fileName();
    file.write(data);
    file.close();

    m_mediaPlayer.setSource(QUrl::fromLocalFile(file.fileName()));
#else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(data);
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(&m_mediaBuffer);
#endif

    m_audioOutput.setMuted(false);
    m_mediaPlayer.play();
}

void RadioPlayer::playPlaylist(const QByteArray &data)
{
    const AudioPlaylist playlist(data, this);
    if (playlist.isEmpty())
    {
        qCWarning(gmAudioRadio()) << "Playlist is empty";
        return;
    }

    play(playlist.files().constFirst());
}

/**
 * @brief Tell MetaDataReader that there is new MetaData available
 */
void RadioPlayer::onMetaDataChanged()
{
    qCDebug(gmAudioRadio()) << "MetaData changed!";

    if (m_mediaPlayer.bufferProgress() >= BUFFER_FULL || (m_mediaPlayer.mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        emit metaDataChanged(m_mediaPlayer.metaData());
    }
}

void RadioPlayer::onFileReceived(Files::FileDataResult *result)
{
    if (!result) return;

    qCDebug(gmAudioRadio()) << "Received file ...";

    m_mediaPlayer.stop();
    const auto data = result->data();
    result->deleteLater();

    if (data.isEmpty())
    {
        qCWarning(gmAudioRadio()) << "Error: File is empty!";
        return;
    }

    if (data.isValidUtf8())
    {
        qCDebug(gmAudioRadio()) << "Received file is utf-8, so probably a playlist file";
        playPlaylist(data);
        return;
    }

    playAudio(data);
}

void RadioPlayer::onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    qCDebug(gmAudioRadio()) << "Playback state changed:" << newState;

    if (newState == QMediaPlayer::PlayingState) emit startedPlaying();
}

void RadioPlayer::onMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCDebug(gmAudioRadio()) << "Media status changed:" << status;
}

void RadioPlayer::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qCDebug(gmAudioRadio()) << "Error:" << error << errorString;
}
