#include "radioplayer.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"
#include "logging.h"
#include "utils/fileutils.h"

RadioPlayer::RadioPlayer(MetaDataReader *metaDataReader, DiscordPlayer *discordPlayer, QObject *parent)
    : AudioPlayer(parent), m_discordPlayer(discordPlayer)
{
    qCDebug(gmAudioRadio()) << "Loading RadioPlayer ...";

    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayer->setObjectName(tr("Radio"));

    connect(m_mediaPlayer, &QMediaPlayer::stateChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "State changed:" << m_mediaPlayer->state();

        if (m_mediaPlayer->state() == QMediaPlayer::PlayingState) emit startedPlaying();
    });

    connect(m_mediaPlayer, &QMediaPlayer::bufferStatusChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "Buffer status changed:" << m_mediaPlayer->bufferStatus();
    });
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "Media status changed:" << m_mediaPlayer->mediaStatus();
    });
    connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), [ = ](QMediaPlayer::Error error) {
        qCDebug(gmAudioRadio()) << "Error:" << error << m_mediaPlayer->errorString();
    });

    // MetaData
    connect(m_mediaPlayer,  QOverload<>::of(&QMediaObject::metaDataChanged), this, &RadioPlayer::onMetaDataChanged);
    connect(m_mediaPlayer,  QOverload<const QString&, const QVariant&>::of(&QMediaObject::metaDataChanged),
            metaDataReader, QOverload<const QString&, const QVariant&>::of(&MetaDataReader::updateMetaData));
    connect(this,           QOverload<QMediaPlayer *>::of(&RadioPlayer::metaDataChanged),
            metaDataReader, QOverload<QMediaPlayer *>::of(&MetaDataReader::updateMetaData));

    // Files
    connect(FileManager::getInstance(), &FileManager::receivedFile, this, &RadioPlayer::onFileReceived);
}

/**
 * @brief Play a radio element
 * @param element RadioElement to be played
 */
void RadioPlayer::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioRadio()) << "Playing radio element:" << element->name();

    m_mediaPlayer->stop();
    m_mediaPlayer->setObjectName(tr("Radio") + ": " + element->name());
    m_currentElement = element;

    if (element->files().empty()) return;

    auto *audioFile = element->files()[0];
    m_fileName = audioFile->url();

    if (audioFile->source() == 0)
    {
        qCDebug(gmAudioRadio()) << "Playing radio from local playlist:" << audioFile->url() << "...";
        m_fileRequestId = FileManager::getUniqueRequestId();
        FileManager::getInstance()->getFile(m_fileRequestId, SettingsManager::getPath("radio") + audioFile->url());
    }
    else
    {
        qCDebug(gmAudioRadio()) << "Playing radio from url:" << audioFile->url();
        m_mediaPlayer->setMedia(QUrl(audioFile->url()));

        auto useDiscord = Discord::getInstance()->enabled();
        m_mediaPlayer->setMuted(useDiscord);
        if (useDiscord) m_discordPlayer->playMusic(audioFile->url());

        m_mediaPlayer->play();
    }
}

/**
 * @brief Start playing the radio element
 */
void RadioPlayer::play()
{
    qCDebug(gmAudioRadio()) << "Continue play of RadioPlayer ...";
    m_mediaPlayer->play();
}

/**
 * @brief Pause the playback of the radio element
 */
void RadioPlayer::pause()
{
    qCDebug(gmAudioRadio()) << "Pausing RadioPlayer ...";
    m_mediaPlayer->pause();
}

/**
 * @brief Stop radio playback
 */
void RadioPlayer::stop()
{
    qCDebug(gmAudioRadio()) << "Stopping RadioPlayer ...";

    m_mediaPlayer->stop();
}

/**
 * @brief Tell MetaDataReader that there is new MetaData available
 */
void RadioPlayer::onMetaDataChanged()
{
    qCDebug(gmAudioRadio()) << "MetaData changed!";

    if ((m_mediaPlayer->bufferStatus() == 100) || (m_mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        emit metaDataChanged(m_mediaPlayer);
    }
}

void RadioPlayer::onFileReceived(int id, const QByteArray& data)
{
    if (m_fileRequestId != id) return;

    qCDebug(gmAudioRadio()) << "Received file ...";

    m_mediaPlayer->stop();

    if (data.isEmpty())
    {
        qCWarning(gmAudioRadio()) << "Error: File is empty!";
        return;
    }

    if (m_playlist) m_playlist->deleteLater();

    m_playlist = new QMediaPlaylist(this);
    connect(m_playlist,                   &QMediaPlaylist::loaded,                         [ = ]() { qCDebug(gmAudioRadio()) << "Successfully loaded playlist."; });
    connect(m_playlist,                   &QMediaPlaylist::loadFailed,                     [ = ]() {
        qCDebug(gmAudioRadio()) << "Failed to load playlist:" << m_playlist->errorString();
    });

    #ifdef Q_OS_WIN
    QFile file(m_tempDir.path() + "/" +  FileUtils::fileName(m_fileName));

    if (!file.open(QIODevice::WriteOnly))
    {
        if (file.error() == QFileDevice::OpenError)
        {
            file.setFileName(FileUtils::incrementFileName(file.fileName()));
            if (!file.open(QIODevice::WriteOnly))
            {
                qCWarning(gmAudioSounds()) << "Error: Could not open temporary file even after incrementing the filename" << file.fileName() << file.errorString();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioSounds()) << "Error: Could not open temporary file:" << file.fileName() << file.errorString();
            return;
        }
    }

    qCWarning(gmAudioRadio()) << file.fileName();
    file.write(data);
    file.close();

    m_playlist->load(QUrl::fromLocalFile(file.fileName()));
    #else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(data);
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_playlist->load(&m_mediaBuffer);
    #endif

    auto useDiscord = Discord::getInstance()->enabled();
    m_mediaPlayer->setMuted(useDiscord);
    if (useDiscord) m_discordPlayer->playRadioPlaylist(data);

    m_mediaPlayer->setPlaylist(m_playlist);
    m_mediaPlayer->play();
}
