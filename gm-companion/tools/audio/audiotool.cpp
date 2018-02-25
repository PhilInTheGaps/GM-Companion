#include "audiotool.h"

// #include "ui_audiotool.h"
#include "gm-companion/functions.h"

// #include "gm-companion/ui/flowlayout.h"
#include "gm-companion/tools/audio/audioeditor.h"

#include <QDebug>
#include <QSettings>
#include <QElapsedTimer>

//// Only Windows relevant
// #ifdef _WIN32

//// # include <QtWinExtras>

// #endif // ifdef _WIN32

//// Linux only
// #ifdef __linux__
// # include "taglib/tag.h"
// # include "taglib/taglib.h"
// # include "taglib/fileref.h"
// # include "taglib/mpegfile.h"
// # include "taglib/id3v2tag.h"
// # include "taglib/attachedpictureframe.h"
// #endif // ifdef __linux__

AudioTool::AudioTool(QObject *parent) : QObject(parent)
{
    qDebug().noquote() << "Loading AudioTool ...";

    sManager = new SettingsManager;

    musicPlayer   = new QMediaPlayer;
    musicPlaylist = new QMediaPlaylist;
    connect(musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(onCurrentSongChanged()));
    musicVolume = 75;

    soundVolume = 25;

    radioPlaylist = new QMediaPlaylist;
}

AudioTool::~AudioTool()
{
}

// Returns list of all project files found
QStringList AudioTool::projectList()
{
    QString path = sManager->getSetting(Setting::audioPath);

    projects = getFiles(path);
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
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject;
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
        l_categories.clear();
        count = settings.beginReadArray("Categories");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            QString category = settings.value("name").toString();

            // If category is found in order list, insert the category at the
            // found position
            int index = categoriesOrder.indexOf(category);

            // If the found index is larger than the temporary list, insert
            // category at the end
            if (index > i) index = i;

            l_categories.insert(index, category);
        }
        settings.endArray();
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
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject;
        QSettings settings(path, QSettings::IniFormat);

        // Get the order in which the scenarios should be displayed
        QStringList scenarioOrder;
        int count = settings.beginReadArray(l_currentCategory + "_Scenarios_Order");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            scenarioOrder.append(settings.value("name").toString());
        }

        settings.endArray();

        // Get all the scenarios
        l_scenarios.clear();
        count = settings.beginReadArray(l_currentCategory + "_Scenarios");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            QString scenario = settings.value("name").toString();

            // If scenario is found in order list, insert the scenario at the
            // found position
            int index = scenarioOrder.indexOf(scenario);

            // If the found index is larger than the temporary list, insert
            // the scenario at the end
            if (index > i) index = i;

            l_scenarios.insert(index, scenario);
        }
        settings.endArray();
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
        QString   path    = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject;
        QString   resPath = sManager->getSetting(Setting::resourcesPath);
        QSettings settings(path, QSettings::IniFormat);

        // Get the order in which the elements should be displayed
        QStringList elementOrder;
        int count = settings.beginReadArray(l_currentCategory + "_" + l_currentScenario + "_Order");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            elementOrder.append(settings.value("name").toString());
        }

        settings.endArray();

        // Get all the elements
        l_elements.clear();
        l_elementIcons.clear();
        l_elementTypes.clear();

        for (QString type : { "MusicLists", "SoundLists", "Radios" })
        {
            count = settings.beginReadArray(l_currentCategory + "_" + l_currentScenario + "_" + type);

            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);
                QString element = settings.value("name").toString();
                QString defaultIcon;
                int     typeInt = -1;

                if (type == "MusicLists")
                {
                    defaultIcon = "/icons/media/music_image.png";
                    typeInt     = 0;
                }
                else if (type == "SoundLists")
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

                if (iconPath != defaultIcon)
                {
                    iconPath = resPath + iconPath;
                }

                // If element is found in order list, insert the element at the
                // found position
                int index = elementOrder.indexOf(element);

                // If the found index is larger than the temporary list, insert
                // the element at the end
                if (index > i) index = i;

                l_elements.insert(index, element);
                l_elementIcons.insert(index, iconPath);
                l_elementTypes.insert(index, typeInt);
            }
            settings.endArray();
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
        QString basePath = sManager->getSetting(Setting::musicPath);

        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject;
        QSettings settings(path, QSettings::IniFormat);

        bool randomPlaylist = false;
        bool randomPlayback = true;
        bool loop           = false;
        bool sequential     = false;

        QList<Song> songList;
        int count = settings.beginReadArray(l_currentCategory + "_" + l_currentScenario + "_MusicLists");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == element)
            {
                randomPlayback = settings.value("randomPlayback", true).toBool();
                randomPlaylist = settings.value("randomPlaylist", false).toBool();
                loop           = settings.value("loop", false).toBool();
                sequential     = settings.value("sequential", false).toBool();

                int songs = settings.beginReadArray("songs");

                for (int j = 0; j < songs; j++)
                {
                    settings.setArrayIndex(j);

                    Song song;
                    song.path  = basePath + settings.value("path").toString();
                    song.title = settings.value("name", song.path).toString();
                    songList.append(song);
                }
                settings.endArray();
            }
        }

        settings.endArray();

        // Shuffle playlist
        if (randomPlaylist) {
            if (randomPlaylist) std::random_shuffle(songList.begin(), songList.end());

            for (int i = 0; i < songList.size(); i++)
            {
                musicPlaylist->addMedia(QUrl::fromLocalFile(songList.at(i).path));
                l_songs.append(songList.at(i).title);
            }
        }

        musicPlayer->setPlaylist(musicPlaylist);
        musicPlayer->setVolume(musicVolume);

        // If random mode is active, start with a random song
        if (randomPlayback) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
            musicPlaylist->next();
        } else if (loop || randomPlaylist) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        } else if (sequential) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
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

            bool randomPlaylist = false;
            bool randomPlayback = true;
            bool loop           = false;
            bool sequential     = false;

            // Read properties
            QSettings settings(sManager->getSetting(Setting::audioPath) + "/" + l_currentProject, QSettings::IniFormat);
            QString   basePath = sManager->getSetting(Setting::soundPath);

            int count = settings.beginReadArray(l_currentCategory + "_" + l_currentScenario + "_SoundLists");

            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == element)
                {
                    randomPlaylist = settings.value("randomPlaylist", false).toBool();
                    randomPlayback = settings.value("randomPlayback", true).toBool();
                    loop           = settings.value("loop", false).toBool();
                    sequential     = settings.value("sequential", false).toBool();

                    int sounds = settings.beginReadArray("sounds");

                    for (int j = 0; j < sounds; j++)
                    {
                        settings.setArrayIndex(j);
                        QString path = basePath + settings.value("path").toString();

                        if (QFile(path).exists()) playlist->addMedia(QUrl::fromLocalFile(path));
                    }

                    settings.endArray();
                }
            }

            settings.endArray();

            if (randomPlaylist) playlist->shuffle();

            // Set playback mode
            if (randomPlayback) {
                playlist->setPlaybackMode(QMediaPlaylist::Random);
                playlist->next();
            }
            else if (loop || randomPlaylist) {
                playlist->setPlaybackMode(QMediaPlaylist::Loop);
            }
            else if (sequential) {
                playlist->setPlaybackMode(QMediaPlaylist::Sequential);
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
        QString basePath = sManager->getSetting(Setting::radioPath);

        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject;
        QSettings settings(path, QSettings::IniFormat);

        int count = settings.beginReadArray(l_currentCategory + "_" + l_currentScenario + "_Radios");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == element)
            {
                QString url = settings.value("URL").toString();

                if (url.contains(".m3u") || url.contains(".pls"))
                {
                    url = basePath + url;
                    radioPlaylist->load(QUrl::fromLocalFile(url));
                    musicPlayer->setPlaylist(radioPlaylist);
                }
                else
                {
                    musicPlayer->setMedia(QUrl(url));
                }
            }
        }

        settings.endArray();

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
