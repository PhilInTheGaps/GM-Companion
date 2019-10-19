#include "soundplayer.h"
#include <QDebug>

SoundPlayer::SoundPlayer(FileManager *fManager)
{
    qDebug() << "Loading SoundPlayer ...";

    fileManager = fManager;
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

        if (element->mode() == 0)
        {
            // Shuffle sound list
            std::random_shuffle(sounds.begin(), sounds.end());
        }

        for (auto sound : sounds)
        {
            switch (sound.source())
            {
            case 0:
                playlist->addMedia(QUrl::fromLocalFile(sManager.getSetting(Setting::soundPath) + sound.url()));
                break;

            default:
                playlist->addMedia(QUrl(sound.url()));
                break;
            }
        }

        // Fetch sound urls
        // args[0] = Name of element
        // args[1] = Playlist mode
        //        fileManager->getAudioSaveLoad()->fetchSoundPaths(sounds, {
        // element->name(), QString::number(element->mode()) });

        player->play();
        emit soundsChanged(elements);
    }
}

/**
 * @brief Add new found URLs to sound playlist
 * @param urls List of new URLs
 * @param args Contains info on the audio playlist
 */
void SoundPlayer::onSoundPathsChanged(QList<QUrl>urls, QStringList args)
{
    //    qDebug() << "Sound paths changed!" << urls << args;

    //    QString name = args[0];
    //    int     mode = args[1].toInt();

    //    for (auto a : players)
    //    {
    //        if (a->objectName() == name)
    //        {
    //            // Add sounds to playlist
    //            for (auto b : urls)
    //            {
    //                a->playlist()->addMedia(b);
    //            }

    //            // Set playlist mode
    //            switch (mode)
    //            {
    //            case 0: // Shuffle Playlist
    //            case 2: // Loop
    //                a->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
    //                break;

    //            case 1: // Complete Random Mode
    //                a->playlist()->setPlaybackMode(QMediaPlaylist::Random);
    //                a->playlist()->next();
    //                break;

    //            case 3: // Sequential
    //
    //              a->playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
    //                break;

    //            default: break;
    //            }

    //            if (a->state() != QMediaPlayer::State::PlayingState)
    //            {
    //                a->play();
    //            }

    //            return;
    //        }
    //    }
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
