#include "radioplayer.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"
#include "logging.h"
#include "utils/fileutils.h"

RadioPlayer::RadioPlayer(QObject *parent) : AudioPlayer(parent)
{
    qCDebug(gmAudioRadio()) << "Loading RadioPlayer ...";

    player = new QMediaPlayer;
    player->setObjectName(tr("Radio"));
    playlist = new QMediaPlaylist;

    connect(player, &QMediaPlayer::stateChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "State changed:" << player->state();

        if (player->state() == QMediaPlayer::PlayingState) emit startedPlaying();
    });

    connect(player, &QMediaPlayer::bufferStatusChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "Buffer status changed:" << player->bufferStatus();
    });
    connect(player, &QMediaPlayer::mediaStatusChanged, [ = ]() {
        qCDebug(gmAudioRadio()) << "Media status changed:" << player->mediaStatus();
    });
    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), [ = ](QMediaPlayer::Error error) {
        qCDebug(gmAudioRadio()) << "Error:" << error << player->errorString();
    });
    connect(player,                     QOverload<>::of(&QMediaObject::metaDataChanged), this, &RadioPlayer::onMetaDataChanged);
    connect(player,                     QOverload<const QString&, const QVariant&>::of(&QMediaObject::metaDataChanged),
            this, QOverload<const QString&, const QVariant&>::of(&RadioPlayer::metaDataChanged));
    connect(FileManager::getInstance(), &FileManager::receivedFile,                      this, &RadioPlayer::onFileReceived);

    connect(playlist,                   &QMediaPlaylist::loaded,                         [ = ]() { qCDebug(gmAudioRadio()) << "Successfully loaded playlist."; });
    connect(playlist,                   &QMediaPlaylist::loadFailed,                     [ = ]() {
        qCDebug(gmAudioRadio()) << "Failed to load playlist:" << playlist->errorString();
    });
}

RadioPlayer::~RadioPlayer()
{
    playlist->deleteLater();
    player->deleteLater();
}

/**
 * @brief Play a radio element
 * @param element RadioElement to be played
 */
void RadioPlayer::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioRadio()) << "Playing radio element:" << element->name();

    player->stop();
    player->setObjectName(tr("Radio") + ": " + element->name());
    currentElement = element;

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
        player->setMedia(QUrl(audioFile->url()));
        player->play();
    }
}

/**
 * @brief Start playing the radio element
 */
void RadioPlayer::play()
{
    qCDebug(gmAudioRadio()) << "Continue play of RadioPlayer ...";
    player->play();
}

/**
 * @brief Pause the playback of the radio element
 */
void RadioPlayer::pause()
{
    qCDebug(gmAudioRadio()) << "Pausing RadioPlayer ...";
    player->pause();
}

/**
 * @brief Stop radio playback
 */
void RadioPlayer::stop()
{
    qCDebug(gmAudioRadio()) << "Stopping RadioPlayer ...";

    player->stop();
}

/**
 * @brief Tell MetaDataReader that there is new MetaData available
 */
void RadioPlayer::onMetaDataChanged()
{
    qCDebug(gmAudioRadio()) << "MetaData changed!";

    if ((player->bufferStatus() == 100) || (player->mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        emit metaDataChanged(player);
    }
}

void RadioPlayer::onFileReceived(int id, const QByteArray& data)
{
    if (m_fileRequestId != id) return;

    qCDebug(gmAudioRadio()) << "Received file ...";

    player->stop();

    if (data.isEmpty())
    {
        qCWarning(gmAudioRadio()) << "Error: File is empty!";
        return;
    }

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

    playlist->load(QUrl::fromLocalFile(file.fileName()));
    #else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(data);
    m_mediaBuffer.open(QIODevice::ReadOnly);
    playlist->load(&m_mediaBuffer);
    #endif

    player->setPlaylist(playlist);
    player->play();
}
