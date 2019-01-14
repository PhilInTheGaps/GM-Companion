#include "musicplayer.h"
#include <QDebug>
#include <algorithm>
#include <cstdlib>

MusicPlayer::MusicPlayer(FileManager *fManager)
{
    qDebug() << "Loading MusicPlayer ...";

    fileManager = fManager;

    player   = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);
    player->setObjectName(tr("Music"));

    connect(player, &QMediaPlayer::stateChanged, [ = ]() {
        if (player->state() == QMediaPlayer::PlayingState) emit startedPlaying();
    });

    connect(player, QOverload<>::of(&QMediaObject::metaDataChanged), this, &MusicPlayer::onMetaDataChanged);
}

MusicPlayer::~MusicPlayer()
{
    playlist->deleteLater();
    player->deleteLater();
}

/**
 * @brief Play music list
 * @param name Name of the element
 */
void MusicPlayer::play(MusicElement *element)
{
    if (!element) return;

    qDebug() << "Playing music list:" << element->name();

    player->stop();
    player->setObjectName(tr("Music") + ": " + element->name());
    playlist->clear();
    m_songNames.clear();
    currentElement   = element;
    m_waitingForUrls = true;

    QStringList songs = element->files();

    if (element->mode() == 0)
    {
        // Shuffle song list
        std::random_shuffle(songs.begin(), songs.end());
    }

    // Tell FileManager to fetch song urls
    qDebug() << "MusicPlayer:" << songs;
    fileManager->getAudioFileManager()->fetchMusicPaths(songs);

    for (auto s : songs)
    {
        m_songNames.append(s.right(s.length() - s.lastIndexOf("/") - 1));
    }

    emit songNamesChanged();
}

/**
 * @brief Add new found songs to the playlist
 * @param urls List of URLs of new songs
 */
void MusicPlayer::onSongPathsChanged(QList<QUrl>urls)
{
    qDebug() << "MusicPlayer: Received new song paths!";

    // Add media to playlist
    for (auto u : urls)
    {
        playlist->addMedia(u);
    }

    // Playback mode
    switch (currentElement->mode())
    {
    case 0: // Shuffle Playlist
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;

    case 1: // Complete Random Mode
        playlist->setPlaybackMode(QMediaPlaylist::Random);

        if (m_waitingForUrls) playlist->next();
        break;

    case 2: // Loop
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;

    case 3: // Sequential
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        break;

    default:
        break;
    }

    m_waitingForUrls = false;

    for (int i = 0; i < urls.count(); i++)
    {
        QString s = playlist->media(i).canonicalUrl().toString();
    }

    if (player->state() != QMediaPlayer::State::PlayingState) player->play();

    emit songNamesChanged();
}

/**
 * @brief Start playing music
 */
void MusicPlayer::play()
{
    qDebug() << "Continue play of MusicPlayer ...";
    player->play();
}

/**
 * @brief Pause the music playback
 */
void MusicPlayer::pause()
{
    qDebug() << "Pausing MusicPlayer ...";
    player->pause();
}

/**
 * @brief Stop playing music
 */
void MusicPlayer::stop()
{
    qDebug() << "Stopping MusicPlayer ...";

    player->stop();
    m_songNames.clear();
    emit songNamesChanged();
}

/**
 * @brief Tell MetaDataReader that new MetaData is available
 */
void MusicPlayer::onMetaDataChanged()
{
    if (player->bufferStatus() == 100)
    {
        qDebug() << "MusicPlayer: MetaData changed!";
        emit metaDataChanged(player);
    }
}
