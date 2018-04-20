#include "audiotool.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QSettings>
#include <QMediaMetaData>

// Linux only
 #ifdef __linux__
 # include "taglib/tag.h"
 # include "taglib/taglib.h"
 # include "taglib/fileref.h"
 # include "taglib/mpegfile.h"
 # include "taglib/id3v2tag.h"
 # include "taglib/attachedpictureframe.h"
 #endif // ifdef __linux__

AudioTool::AudioTool(QObject *parent) : QObject(parent)
{
    qDebug().noquote() << "Loading AudioTool ...";

    sManager = new SettingsManager;

    musicPlayer = new QMediaPlayer;
    connect(musicPlayer,   SIGNAL(metaDataChanged()),        SLOT(onMetaDataChanged()));
    musicPlaylist = new QMediaPlaylist;
    connect(musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(onCurrentSongChanged()));
    musicVolume = 75;

    soundVolume = 25;

    radioPlaylist = new QMediaPlaylist;
}

// Returns list of all project files found
QStringList AudioTool::projectList()
{
    QString path = sManager->getSetting(Setting::audioPath);

    projects.clear();

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".audio")) projects.append(file.replace(".audio", ""));
    }

    return projects;
}

// Returns the currently loaded project
QString AudioTool::currentProject()
{
    return l_currentProject;
}

// Set the current project
void AudioTool::setCurrentProject(QString project)
{
    l_currentProject = project;
    emit currentProjectChanged();
}

// Returns list of all categories in project
QStringList AudioTool::categories()
{
    if (l_currentProject != NULL)
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        // Get the order in which the categories should be displayed
        QStringList categoriesOrder;
        int count = settings.beginReadArray("Categories_Order");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            categoriesOrder.append(settings.value("name").toString());
        }
        settings.endArray();

        // Get all categories
        l_categories = settings.value("categories").toStringList();
    }

    return l_categories;
}

// Returns the currently selected category
QString AudioTool::currentCategory()
{
    return l_currentCategory;
}

// Set the current category
void AudioTool::setCurrentCategory(QString category)
{
    l_currentCategory = category;
    emit currentCategoryChanged();
}

QStringList AudioTool::scenarios()
{
    if ((l_currentProject != NULL) && (l_currentCategory != NULL))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(l_currentCategory);
        l_scenarios = settings.value("scenarios").toStringList();
        settings.endGroup();
    }

    return l_scenarios;
}

QString AudioTool::currentScenario()
{
    return l_currentScenario;
}

void AudioTool::setCurrentScenario(QString scenario)
{
    l_currentScenario = scenario;
    emit currentScenarioChanged();
}

QString AudioTool::elementIcon(QString element)
{
    QString path = "/icons/media/music_image.png";

    int index = l_elements.indexOf(element);

    if (index > -1) path = l_elementIcons.at(index);

    return path;
}

void AudioTool::findElements()
{
    if ((l_currentProject != NULL) && (l_currentCategory != NULL) && (l_currentScenario != NULL))
    {
        QString   path    = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QString   resPath = sManager->getSetting(Setting::resourcesPath);
        QSettings settings(path, QSettings::IniFormat);

        // Get all the elements
        l_elements.clear();
        l_elementIcons.clear();
        l_elementTypes.clear();

        for (QString type : { "_music", "_sounds", "_radios" })
        {
            QStringList tempElements;

            settings.beginGroup(l_currentCategory);
            tempElements = settings.value(l_currentScenario + type).toStringList();
            l_elements.append(tempElements);
            settings.endGroup();

            for (QString element : tempElements)
            {
                settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + element + (type == "_radios" ? "_radio" : type));

                QString defaultIcon;
                int     typeInt = -1;

                if (type == "_music")
                {
                    defaultIcon = "/icons/media/music_image.png";
                    typeInt     = 0;
                }
                else if (type == "_sounds")
                {
                    defaultIcon = "/icons/media/sound_image.png";
                    typeInt     = 1;
                }
                else
                {
                    defaultIcon = "/icons/media/radio_image.png";
                    typeInt     = 2;
                }

                QString iconPath = settings.value("icon", defaultIcon).toString();

                if ((iconPath == "") || iconPath.isNull())
                {
                    iconPath = defaultIcon;
                }

                if ((iconPath != defaultIcon))
                {
                    iconPath = resPath + iconPath;
                }

                l_elementIcons.append(iconPath);
                l_elementTypes.append(typeInt);
                settings.endGroup();
            }
        }
    }
}

QStringList AudioTool::elements()
{
    return l_elements;
}

QString AudioTool::currentElement()
{
    return l_currentElement;
}

int AudioTool::elementType(int index)
{
    return l_elementTypes.at(index);
}

void AudioTool::setCurrentElement(QString element)
{
    l_currentElement = element;
    emit currentElementChanged();
}

void AudioTool::playMusic(QString element)
{
    if ((l_currentProject != NULL) && (l_currentCategory != NULL) && (l_currentScenario != NULL) && (element != NULL))
    {
        musicPlaylist->clear();
        l_songs.clear();

        l_currentElement = element;
        QString   basePath = sManager->getSetting(Setting::musicPath);
        QString   path     = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        QList<Song> songList;

        settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + l_currentElement + "_music");

        int mode  = settings.value("mode", 0).toInt();
        int count = settings.beginReadArray("songs");

        // Get songs
        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            Song song;
            song.path  = basePath + settings.value("song").toStringList().at(1);
            song.title = settings.value("song").toStringList().at(0);
            songList.append(song);
        }

        settings.endArray();
        settings.endGroup();

        musicPlayer->setPlaylist(musicPlaylist);
        musicPlayer->setVolume(musicVolume);

        // Playback mode
        switch (mode) {
        case 0: // Shuffle Playlist
            std::random_shuffle(songList.begin(), songList.end());

            musicPlayer->setPlaylist(musicPlaylist);
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 1: // Complete Random Mode
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
            musicPlaylist->next();
            break;

        case 2: // Loop
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 3: // Sequential
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;

        default:
            break;
        }

        for (int i = 0; i < songList.size(); i++)
        {
            musicPlaylist->addMedia(QUrl::fromLocalFile(songList.at(i).path));
            l_songs.append(songList.at(i).title);
        }

        emit songsChanged();
        musicNotRadio = true;
        musicPlayer->play();

        if (!l_isPlaying)
        {
            l_isPlaying = true;
            emit isPlayingChanged();
        }
    }
}

void AudioTool::setMusicIndex(int index)
{
    musicPlaylist->setCurrentIndex(index);
    emit currentSongChanged();
}

void AudioTool::musicNext()
{
    musicPlaylist->next();
}

void AudioTool::musicAgain()
{
    musicPlayer->setPosition(0);
}

void AudioTool::musicPausePlay()
{
    if (musicPlayer->state() == QMediaPlayer::PlayingState)
    {
        musicPlayer->pause();
        l_isPlaying = false;
        emit isPlayingChanged();
    }
    else if (musicPlayer->state() == QMediaPlayer::PausedState)
    {
        musicPlayer->play();
        l_isPlaying = true;
        emit isPlayingChanged();
    }
}

bool AudioTool::isPlaying()
{
    return l_isPlaying;
}

QStringList AudioTool::songs()
{
    return l_songs;
}

int AudioTool::currentSongIndex()
{
    return musicPlaylist->currentIndex();
}

void AudioTool::onCurrentSongChanged()
{
    if (musicNotRadio) currentSongChanged();
}

void AudioTool::setMusicVolume(float volume)
{
    musicVolume = volume / 2 * 100;
    musicPlayer->setVolume(musicVolume);
}

void AudioTool::setSoundVolume(float volume)
{
    soundVolume = volume / 2 * 100;

    for (QMediaPlayer *player : soundPlayerList)
    {
        player->setVolume(soundVolume);
    }
}

void AudioTool::playSound(QString element)
{
    if ((l_currentProject != NULL) && (l_currentCategory != NULL) && (l_currentScenario != NULL) && (element != NULL))
    {
        if (!isSoundPlayling(element))
        {
            QMediaPlayer   *player   = new QMediaPlayer;
            QMediaPlaylist *playlist = new QMediaPlaylist;

            player->setPlaylist(playlist);
            soundPlayerList.append(player);

            player->setObjectName(element);

            // Read properties
            QSettings settings(sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio", QSettings::IniFormat);
            QString   basePath = sManager->getSetting(Setting::soundPath);

            settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + element + "_sounds");

            int mode  = settings.value("mode", 0).toInt();
            int count = settings.beginReadArray("sounds");

            // Get sounds
            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);

                QString path = basePath + settings.value("sound").toStringList().at(1);

                if (QFile(path).exists()) playlist->addMedia(QUrl::fromLocalFile(path));
            }

            settings.endArray();
            settings.endGroup();

            // Playback mode
            switch (mode) {
            case 0: // Shuffle Playlist
                playlist->shuffle();
                playlist->setPlaybackMode(QMediaPlaylist::Loop);
                break;

            case 1: // Complete Random Mode
                playlist->setPlaybackMode(QMediaPlaylist::Random);
                playlist->next();
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

            player->setVolume(soundVolume);
            player->play();
        }
    }
}

bool AudioTool::isSoundPlayling(QString element)
{
    for (QMediaPlayer *player : soundPlayerList)
    {
        if (player->objectName() == element)
        {
            return true;
        }
    }

    return false;
}

void AudioTool::removeSound(QString element)
{
    for (QMediaPlayer *player : soundPlayerList)
    {
        if (player->objectName() == element)
        {
            player->stop();
            soundPlayerList.removeOne(player);
            delete player;
        }
    }
}

void AudioTool::playRadio(QString element)
{
    if ((l_currentProject != NULL) && (l_currentCategory != NULL) && (l_currentScenario != NULL) && (element != NULL))
    {
        musicPlaylist->clear();

        l_currentElement = element;

        // Read properties
        QSettings settings(sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio", QSettings::IniFormat);
        QString   basePath = sManager->getSetting(Setting::radioPath);

        settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + l_currentElement + "_radio");

        QString url   = settings.value("url").toString();
        bool    local = settings.value("local").toBool();

        if (local)
        {
            url = basePath + url;
            radioPlaylist->load(QUrl::fromLocalFile(url));
            musicPlayer->setPlaylist(radioPlaylist);
        }
        else
        {
            musicPlayer->setMedia(QUrl(url));
        }

        settings.endGroup();

        musicPlayer->setVolume(musicVolume);
        musicNotRadio = false;
        musicPlayer->play();

        if (!l_isPlaying)
        {
            l_isPlaying = true;
            emit isPlayingChanged();
        }
    }
}

void AudioTool::onMetaDataChanged()
{
    l_songName.clear();
    l_artist.clear();
    l_album.clear();

    if (musicPlayer->isMetaDataAvailable())
    {
        // Reading tags using TagLib, as it is way more reliable than

        #ifdef __linux__

        if (musicPlayer->bufferStatus() == 100)
        {
            QString path = musicPlaylist->currentMedia().resources().first().url().path();

            // Album, Artist and Title
            TagLib::FileRef f(path.toUtf8());
            l_album    = f.tag()->album().toCString(true);
            l_artist   = f.tag()->artist().toCString(true);
            l_songName =  f.tag()->title().toCString(true);
        }
        #else // ifdef __linux__

        // I can't get TagLib to work on Windows though, so this mess below has
        // to do

        l_songName = musicPlayer->metaData(QMediaMetaData::Title).toString();
        l_album    = musicPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
        l_artist   = musicPlayer->metaData(QMediaMetaData::Author).toString();

        if (l_artist.isNull()) l_artist = musicPlayer->metaData(QMediaMetaData::AlbumArtist).toString();

        if (l_artist.isNull()) l_artist = musicPlayer->metaData(QMediaMetaData::Composer).toString();

        #endif // ifdef __linux__
    }

    emit metaDataChanged();
}

QString AudioTool::getSongName()
{
    return l_songName;
}

QString AudioTool::getArtist()
{
    return l_artist;
}

QString AudioTool::getAlbum()
{
    return l_album;
}
