#include "soundplayer.h"
#include "youtubeutils.h"
#include <QDebug>
#include <internal/heuristics.h>

SoundPlayer::SoundPlayer(FileManager *fManager)
{
    qDebug() << "Loading SoundPlayer ...";

    fileManager = fManager;

    connect(&youtube, &YouTube::receivedVideoMediaStreamInfos, this, &SoundPlayer::onYtReceivedVideoMediaStreamInfos);
}

/**
 * @brief Play a sound element
 * @param element SoundElement to be played
 */
void SoundPlayer::play(SoundElement *element)
{
    if (!element) return;

    qDebug() << "Playing sound list:" << element->name() << "...";

    if (!isSoundPlaying(element))
    {
        QMediaPlayer   *player   = new QMediaPlayer;
        QMediaPlaylist *playlist = new QMediaPlaylist;
        player->setPlaylist(playlist);
        player->setObjectName(element->name());
        player->setVolume(m_volume);
        elements.append(element);

        connect(player, &QMediaPlayer::stateChanged, [ = ](QMediaPlayer::State state) {
            if (state == QMediaPlayer::State::StoppedState)
            {
                removeElement(player->objectName());
                players.removeAll(player);
                player->deleteLater();
            }
        });

        players.append(player);
        auto sounds = element->files();


        switch (element->mode())
        {
        case 0: // Shuffle Playlist
            std::random_shuffle(sounds.begin(), sounds.end());
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 2: // Loop
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 1: // Complete Random Mode
            playlist->setPlaybackMode(QMediaPlaylist::Random);
            playlist->next();
            break;

        case 3: // Sequential

            playlist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;
        }

        for (auto sound : sounds)
        {
            switch (sound->source())
            {
            case 0:
                playlist->addMedia(QUrl::fromLocalFile(sManager.getSetting(Setting::soundPath) + sound->url()));
                break;

            case 3:
                m_ytRequestMap.insert(youtube.getVideoMediaStreamInfos(YouTubeUtils::parseVideoId(sound->url())), player);
                break;

            default:
                playlist->addMedia(QUrl(sound->url()));
                break;
            }
        }

        player->play();
        emit soundsChanged(elements);
    }
}

/**
 * @brief Check if a sound element is currently playing
 * @param element SoundElement to be checked
 * @return True if element is playing
 */
bool SoundPlayer::isSoundPlaying(SoundElement *element)
{
    qDebug() << "Checking if sound" << element->name() << "is playing ...";

    for (auto a : players)
    {
        qDebug() << "   " << a->objectName();

        if (a->objectName() == element->name())
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Set the volume for all active sound elements
 * @param volume Volume value
 */
void SoundPlayer::setVolume(float volume)
{
    m_volume = volume;

    for (auto a : players) a->setVolume(volume);
}

/**
 * @brief Stop playback of a sound element
 * @param element SoundElement to be stopped
 */
void SoundPlayer::stop(QString element)
{
    for (auto a : players)
    {
        if (a->objectName() == element)
        {
            a->stop();
            players.removeAll(a);
            a->deleteLater();
            removeElement(element);
            return;
        }
    }
}

/**
 * @brief Remove a sound element from the active list
 * @param element SoundElement to be removed
 */
void SoundPlayer::removeElement(QString element)
{
    for (auto a : elements)
    {
        if (a->name() == element)
        {
            elements.removeOne(a);
            emit soundsChanged(elements);
            return;
        }
    }
}

void SoundPlayer::onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int requestId)
{
    if (!m_ytRequestMap.contains(requestId)) return;

    QUrl   tempUrl;
    qint64 tempBitrate = 0;
    auto   mediaPlayer = m_ytRequestMap[requestId];
    m_ytRequestMap.remove(requestId);

    if (!mediaPlayer) return;

    for (auto info : infos->audio())
    {
        auto encoding = Heuristics::audioEncodingToString(info->audioEncoding());
        qDebug() << info->audioEncoding() << info->bitrate()
                 << mediaPlayer->hasSupport("audio/" + encoding, { encoding });

        if ((mediaPlayer->hasSupport("audio/" + encoding, { encoding }) > 1) && (info->bitrate() > tempBitrate))
        {
            tempUrl     = info->url();
            tempBitrate = info->bitrate();
        }
    }

    mediaPlayer->playlist()->addMedia(tempUrl);

    if (mediaPlayer->state() != QMediaPlayer::PlayingState)
    {
        mediaPlayer->play();
    }

    delete infos;
}
