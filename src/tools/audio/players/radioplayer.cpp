#include "radioplayer.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"
#include "logging.h"

RadioPlayer::RadioPlayer(QObject *parent) : AudioPlayer(parent)
{
    qCDebug(gmAudioRadio()) << "Loading RadioPlayer ...";

    player = new QMediaPlayer;
    player->setObjectName(tr("Radio"));
    playlist = new QMediaPlaylist;

    connect(player, &QMediaPlayer::stateChanged, [ = ]() {
        if (player->state() == QMediaPlayer::PlayingState) emit startedPlaying();
    });

    connect(player,                     QOverload<>::of(&QMediaObject::metaDataChanged), this, &RadioPlayer::onMetaDataChanged);
    connect(FileManager::getInstance(), &FileManager::receivedFile,                      this, &RadioPlayer::onFileReceived);

    connect(playlist,                   &QMediaPlaylist::loaded,                         [ = ]() { qCDebug(gmAudioRadio()) << "Successfully loaded playlist."; });
    connect(playlist,                   &QMediaPlaylist::loadFailed,                     [ = ]() { qCDebug(gmAudioRadio()) << "Failed to load playlist:" << playlist->errorString(); });
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

    if (element->files().size() == 0) return;

    auto audioFile = element->files()[0];

    if (audioFile->source() == 0)
    {
        qCDebug(gmAudioRadio()) << "Playing radio from local playlist:" << audioFile->url() << "...";
        m_fileRequestId = FileManager::getUniqueRequestId();
        FileManager::getInstance()->getFile(m_fileRequestId, SettingsManager::getPath("radio") + "/" + audioFile->url());
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
        emit metaDataChanged(player, currentElement ? currentElement->icon()->imageId() : QPixmap());
    }
}

void RadioPlayer::onFileReceived(int id, QByteArray data)
{
    if (m_fileRequestId != id) return;

    player->stop();

    if (data.isEmpty()) return;


    if (m_mediaBuffer) delete m_mediaBuffer;

    m_mediaData   = data;
    m_mediaBuffer = new QBuffer(&m_mediaData);

    m_mediaBuffer->open(QIODevice::ReadOnly);
    m_mediaBuffer->seek(0);

    player->setMedia(QMediaContent(), m_mediaBuffer);
    player->play();
}
