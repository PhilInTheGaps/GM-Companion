#include "radioplayer.h"

#include <QDebug>
#include <QNetworkRequest>

RadioPlayer::RadioPlayer(FileManager *fManager)
{
    qDebug() << "Loading RadioPlayer ...";
    fileManager = fManager;

    player = new QMediaPlayer;
    player->setObjectName(tr("Radio"));
    playlist = new QMediaPlaylist;

    connect(player, &QMediaPlayer::stateChanged, [ = ]() {
        if (player->state() == QMediaPlayer::PlayingState) emit startedPlaying();
    });

    connect(player,   QOverload<>::of(&QMediaObject::metaDataChanged), this, &RadioPlayer::onMetaDataChanged);

    connect(playlist, &QMediaPlaylist::loaded,                         [ = ]() { qDebug() << "Radio: Successfully loaded playlist."; });
    connect(playlist, &QMediaPlaylist::loadFailed,                     [ = ]() { qDebug() << "Radio: Failed to load playlist:" << playlist->errorString(); });
}

/**
 * @brief Play a radio element
 * @param element RadioElement to be played
 */
void RadioPlayer::play(RadioElement *element)
{
    if (!element) return;

    qDebug() << "Playing radio element:" << element->name();

    player->stop();
    player->setObjectName(tr("Radio") + ": " + element->name());
    currentElement = element;

    if (element->files().size() == 0) return;

    auto audioFile = element->files()[0];

    if (audioFile->source() == 0)
    {
        qDebug() << "Playing radio from local playlist:" << audioFile->url() << "...";
        fileManager->getAudioSaveLoad()->fetchRadioPath(audioFile->url());
    }
    else
    {
        qDebug() << "Playing radio from url:" << audioFile->url();
        player->setMedia(QUrl(audioFile->url()));
        player->play();
    }
}

/**
 * @brief Load the new URL and start playing
 * @param url URL to be loaded
 */
void RadioPlayer::onUrlChanged(QUrl url)
{
    qDebug() << "Radio: Received URL:" << url;

    playlist->load(url);
    player->setPlaylist(playlist);
    player->play();
}

/**
 * @brief Start playing the radio element
 */
void RadioPlayer::play()
{
    qDebug() << "Continue play of RadioPlayer ...";
    player->play();
}

/**
 * @brief Pause the playback of the radio element
 */
void RadioPlayer::pause()
{
    qDebug() << "Pausing RadioPlayer ...";
    player->pause();
}

/**
 * @brief Stop radio playback
 */
void RadioPlayer::stop()
{
    qDebug() << "Stopping RadioPlayer ...";

    player->stop();
}

/**
 * @brief Tell MetaDataReader that there is new MetaData available
 */
void RadioPlayer::onMetaDataChanged()
{
    qDebug() << "RadioPlayer: MetaData changed!";

    if ((player->bufferStatus() == 100) || (player->mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        emit metaDataChanged(player, currentElement ? currentElement->icon()->background() : "");
    }
}
