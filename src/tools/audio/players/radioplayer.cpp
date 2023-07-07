#include "radioplayer.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

static constexpr auto BUFFER_FULL = 100;

Q_LOGGING_CATEGORY(gmAudioRadio, "gm.audio.radio")

RadioPlayer::RadioPlayer(MetaDataReader &metaDataReader, QObject *parent) : AudioPlayer(parent)
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
    m_fileName = audioFile->url();

    if (audioFile->source() == AudioFile::Source::File)
    {
        qCDebug(gmAudioRadio()) << "Playing radio from local playlist:" << audioFile->url() << "...";

        if (m_fileRequestContext) m_fileRequestContext->deleteLater();
        m_fileRequestContext = new QObject(this);

        m_fileName = audioFile->url();
        Files::File::getDataAsync(FileUtils::fileInDir(audioFile->url(), SettingsManager::getPath(u"music"_s)))
            .then(m_fileRequestContext, [this](Files::FileDataResult *result) { onFileReceived(result); });
    }
    else
    {
        qCDebug(gmAudioRadio()) << "Playing radio from url:" << audioFile->url();
        m_mediaPlayer.setSource(QUrl(audioFile->url()));
        m_audioOutput.setMuted(false);
        m_mediaPlayer.play();
    }
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

    if (result->data().isEmpty())
    {
        qCWarning(gmAudioRadio()) << "Error: File is empty!";
        result->deleteLater();
        return;
    }

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
                result->deleteLater();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioRadio()) << "Error: Could not open temporary file:" << file.fileName()
                                      << file.errorString();
            result->deleteLater();
            return;
        }
    }

    qCWarning(gmAudioRadio()) << file.fileName();
    file.write(result->data());
    file.close();

    m_mediaPlayer.setSource(QUrl::fromLocalFile(file.fileName()));
#else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(result->data());
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(&m_mediaBuffer);
#endif

    m_audioOutput.setMuted(false);
    m_mediaPlayer.play();

    result->deleteLater();
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
