#include "audiotool.h"
#include "src/functions.h"

#include <QDebug>
#include <QSettings>
#include <QMediaMetaData>

// Linux only
 #ifdef Q_OS_LINUX
# ifndef Q_OS_ANDROID
 #  include "taglib/tag.h"
 #  include "taglib/taglib.h"
 #  include "taglib/fileref.h"
 #  include "taglib/mpegfile.h"
 #  include "taglib/id3v2tag.h"
 #  include "taglib/attachedpictureframe.h"
# endif // ifndef Q_OS_ANDROID
 #endif // ifdef __linux__

AudioTool::AudioTool(QObject *parent) : QObject(parent)
{
    qDebug().noquote() << "Loading AudioTool ...";

    m_sManager = new SettingsManager;

    m_musicPlayer = new QMediaPlayer;
    connect(m_musicPlayer,   SIGNAL(metaDataChanged()),        SLOT(onMetaDataChanged()));
    m_musicPlaylist = new QMediaPlaylist;
    connect(m_musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(onCurrentSongChanged()));
    m_musicVolume = 75;

    m_soundVolume = 25;

    m_radioPlaylist = new QMediaPlaylist;

    connect(&m_spotify, &Spotify::iconChanged, this, &AudioTool::onSpotifyIconChanged);
}

// Returns list of all project files found
QStringList AudioTool::projectList()
{
    QString path = m_sManager->getSetting(Setting::audioPath);

    m_projects.clear();

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".audio")) m_projects.append(file.replace(".audio", ""));
    }

    return m_projects;
}

// Returns the currently loaded project
QString AudioTool::currentProject()
{
    return m_currentProject;
}

// Set the current project
void AudioTool::setCurrentProject(QString project)
{
    m_currentProject = project;
    emit currentProjectChanged();
    emit categoriesChanged();
}

// Returns list of all categories in project
QStringList AudioTool::categories()
{
    if (m_currentProject != NULL)
    {
        QString   path = m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
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
        m_categories = settings.value("categories").toStringList();
    }

    return m_categories;
}

// Set the current category
void AudioTool::setCurrentCategory(QString category)
{
    m_currentCategory = category;
    emit currentCategoryChanged();
    emit scenariosChanged();
}

QStringList AudioTool::scenarios()
{
    if ((m_currentProject != NULL) && (m_currentCategory != NULL))
    {
        QString   path = m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(m_currentCategory);
        m_scenarios = settings.value("scenarios").toStringList();
        settings.endGroup();
    }

    return m_scenarios;
}

void AudioTool::setCurrentScenario(QString scenario)
{
    m_currentScenario = scenario;
    emit currentScenarioChanged();
}

QString AudioTool::elementIcon(QString element)
{
    QString path = "/icons/media/music_image.png";

    int index = m_elements.indexOf(element);

    if (index > -1) path = m_elementIcons.at(index);

    return path;
}

void AudioTool::findElements()
{
    if ((m_currentProject != NULL) && (m_currentCategory != NULL) && (m_currentScenario != NULL))
    {
        QString   path    = m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QString   resPath = m_sManager->getSetting(Setting::resourcesPath);
        QSettings settings(path, QSettings::IniFormat);

        // Get all the elements
        m_elements.clear();
        m_elementIcons.clear();
        m_elementTypes.clear();

        int index = -1;

        for (QString type : { "_music", "_sounds", "_radios", "_spotify" })
        {
            QStringList tempElements;

            settings.beginGroup(m_currentCategory);
            tempElements = settings.value(m_currentScenario + type).toStringList();
            m_elements.append(tempElements);
            settings.endGroup();

            bool containsSpotify = false;

            for (QString element : tempElements)
            {
                settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + element + (type == "_radios" ? "_radio" : type));

                QString defaultIcon;
                int     typeInt = -1;
                index++;

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
                else if (type == "_radios")
                {
                    defaultIcon = "/icons/media/radio_image.png";
                    typeInt     = 2;
                }
                else if (type == "_spotify")
                {
                    defaultIcon = "";
                    typeInt     = 3;
                    m_spotify.fetchIcon(settings.value("id", "").toString(), index);
                    containsSpotify = true;
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

                m_elementIcons.append(iconPath);
                m_elementTypes.append(typeInt);
                settings.endGroup();
            }

            if (containsSpotify && !m_spotify.isGranted()) m_spotify.grant();
        }

        emit elementsChanged();
    }
}

void AudioTool::setCurrentElement(QString element)
{
    m_currentElement = element;
    emit currentElementChanged();
}

void AudioTool::playMusic(QString element)
{
    qDebug() << "Playling music:" << element;

    if ((m_currentProject != NULL) && (m_currentCategory != NULL) && (m_currentScenario != NULL) && (element != NULL))
    {
        m_musicPlaylist->clear();
        m_songs.clear();

        m_currentElement = element;
        QString   basePath = m_sManager->getSetting(Setting::musicPath);
        QString   path     = m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        QList<Song> songList;

        settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + m_currentElement + "_music");

        int mode  = settings.value("mode", 0).toInt();
        int count = settings.beginReadArray("songs");

        // Get songs
        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            Song song;
            song.path  = basePath + settings.value("song").toStringList().at(1);
            song.title = settings.value("song").toStringList().at(0);

            if (QFile(song.path).exists())
            {
                Song song;
                song.path  = basePath + settings.value("song").toStringList().at(1);
                song.title = settings.value("song").toStringList().at(0);
                songList.append(song);
            }
            else
            {
                qDebug() << song.title << "Does not exist!";
                qDebug() << "Path:" << song.path;
            }
        }

        settings.endArray();
        settings.endGroup();

        m_musicPlayer->setPlaylist(m_musicPlaylist);
        m_musicPlayer->setVolume(m_musicVolume);

        // Playback mode
        switch (mode) {
        case 0: // Shuffle Playlist
            std::random_shuffle(songList.begin(), songList.end());

            m_musicPlayer->setPlaylist(m_musicPlaylist);
            m_musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 1: // Complete Random Mode
            m_musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
            m_musicPlaylist->next();
            break;

        case 2: // Loop
            m_musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
            break;

        case 3: // Sequential
            m_musicPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;

        default:
            break;
        }

        for (int i = 0; i < songList.size(); i++)
        {
            m_musicPlaylist->addMedia(QUrl::fromLocalFile(songList.at(i).path));
            m_songs.append(songList.at(i).title);
        }

        qDebug() << songs();
        emit songsChanged();
        m_musicNotRadio = true;
        m_spotify.stop();
        m_spotifyPlaying = false;

        if (songList.size() > 0)
        {
            m_musicPlayer->play();

            if (!m_isPlaying)
            {
                m_isPlaying = true;
            }
        }

        emit isPlayingChanged();
    }
}

void AudioTool::setMusicIndex(int index)
{
    if (m_spotifyPlaying)
    {
        m_spotify.setIndex(index);
    }
    else
    {
        m_musicPlaylist->setCurrentIndex(index);
        emit currentSongChanged();
    }
}

void AudioTool::musicNext()
{
    if (m_spotifyPlaying) m_spotify.next();
    else m_musicPlaylist->next();
}

void AudioTool::musicAgain()
{
    if (m_spotifyPlaying) m_spotify.again();
    else m_musicPlayer->setPosition(0);
}

void AudioTool::musicPausePlay()
{
    if (m_spotifyPlaying)
    {
        m_spotify.pausePlay();
        m_isPlaying = m_spotify.isPlaying();
        emit isPlayingChanged();
    }
    else
    {
        if (m_musicPlayer->state() == QMediaPlayer::PlayingState)
        {
            m_musicPlayer->pause();
            m_isPlaying = false;
            emit isPlayingChanged();
        }
        else if (m_musicPlayer->state() == QMediaPlayer::PausedState)
        {
            m_musicPlayer->play();
            m_isPlaying = true;
            emit isPlayingChanged();
        }
    }
}

void AudioTool::setMusicVolume(float volume)
{
    // Convert volume to logarithmic scale
    qreal linearVolume = QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);

    m_musicVolume = qRound(linearVolume * 100);
    m_musicPlayer->setVolume(m_musicVolume);

    m_spotifyVolume = qRound(volume * 100);
    m_spotify.setVolume(m_spotifyVolume); // Spotify takes linear volume
}

void AudioTool::setSoundVolume(float volume)
{
    // Convert volume to logarithmic scale
    qreal linearVolume = QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);

    m_soundVolume = qRound(linearVolume * 100);

    for (QMediaPlayer *player : m_soundPlayerList)
    {
        player->setVolume(m_soundVolume);
    }
}

void AudioTool::playSound(QString element)
{
    if ((m_currentProject != NULL) && (m_currentCategory != NULL) && (m_currentScenario != NULL) && (element != NULL))
    {
        if (!isSoundPlayling(element))
        {
            QMediaPlayer   *player   = new QMediaPlayer;
            QMediaPlaylist *playlist = new QMediaPlaylist;

            player->setPlaylist(playlist);
            m_soundPlayerList.append(player);

            player->setObjectName(element);

            // Read properties
            QSettings settings(m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio", QSettings::IniFormat);
            QString   basePath = m_sManager->getSetting(Setting::soundPath);

            settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + element + "_sounds");

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

            connect(player, &QMediaPlayer::stateChanged, this, &AudioTool::onSoundPlaybackStateChanged);

            player->setVolume(m_soundVolume);
            player->play();
        }
    }
}

bool AudioTool::isSoundPlayling(QString element)
{
    for (QMediaPlayer *player : m_soundPlayerList)
    {
        if (player->objectName() == element)
        {
            return true;
        }
    }

    return false;
}

void AudioTool::onSoundPlaybackStateChanged(QMediaPlayer::State status)
{
    qDebug() << "Sound Status:" << status;

    // Check if status is EndOfMedia, find endet sound player and remove it
    if (status == QMediaPlayer::StoppedState)
    {
        QMediaPlayer *player = nullptr;

        int  i             = 0;
        bool removedPlayer = false;

        while (!removedPlayer)
        {
            player = m_soundPlayerList[i];

            if (player->state() == QMediaPlayer::StoppedState)
            {
                qDebug() << "Found player!";

                player->stop();
                m_soundPlayerList.removeOne(player);

                qDebug() << "Emitting signal...";
                emit soundPlayerRemoved(i);

                removedPlayer = true;
            }

            i++;
        }

        player->deleteLater();
    }
}

void AudioTool::removeSound(QString element)
{
    for (QMediaPlayer *player : m_soundPlayerList)
    {
        if (player->objectName() == element)
        {
            player->stop();
            m_soundPlayerList.removeOne(player);
            delete player;
        }
    }
}

void AudioTool::playRadio(QString element)
{
    if ((m_currentProject != NULL) && (m_currentCategory != NULL) && (m_currentScenario != NULL) && (element != NULL))
    {
        qDebug() << "Playing radio:" << element;

        m_musicPlaylist->clear();
        m_currentElement = element;
        m_songs.clear();

        // Read properties
        QSettings settings(m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio", QSettings::IniFormat);
        QString   basePath = m_sManager->getSetting(Setting::radioPath);

        settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + m_currentElement + "_radio");

        QString url   = settings.value("url").toString();
        bool    local = settings.value("local").toBool();

        if (local)
        {
            qDebug() << "Playing from local playlist:" << basePath + url;
            url = basePath + url;
            m_radioPlaylist->load(QUrl::fromLocalFile(url));
            m_musicPlayer->setPlaylist(m_radioPlaylist);
        }
        else
        {
            m_musicPlayer->setMedia(QUrl(url));
        }

        settings.endGroup();

        m_spotify.stop();
        m_spotifyPlaying = false;

        m_musicPlayer->setVolume(m_musicVolume);
        m_musicNotRadio = false;
        m_musicPlayer->play();

        if (!m_isPlaying)
        {
            m_isPlaying = true;
            emit isPlayingChanged();
        }
    }
}

void AudioTool::playSpotify(QString element)
{
    if ((m_currentProject != NULL) && (m_currentCategory != NULL) && (m_currentScenario != NULL) && (element != NULL))
    {
        m_musicPlaylist->clear();
        m_currentElement = element;

        // Read properties
        QSettings settings(m_sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio", QSettings::IniFormat);

        settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + m_currentElement + "_spotify");
        QString id = settings.value("id").toString();
        settings.endGroup();

        m_spotify.setVolume(m_spotifyVolume);
        m_spotifyPlaying = true;
        m_musicPlayer->pause();

        m_spotify.play(id);
        m_isPlaying = m_spotify.isPlaying();

        m_songs.clear();

        emit currentElementChanged();
        emit songsChanged();
        emit isPlayingChanged();
    }
}

#include <QImage>

QString AudioTool::convertCoverImage()
{
    if (m_musicPlayer->availableMetaData().contains("CoverArtImage"))
    {
        QImage myImage = m_musicPlayer->metaData(QMediaMetaData::CoverArtImage).value<QImage>();
        QByteArray bArray;
        QBuffer    buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        myImage.save(&buffer, "JPEG");

        QString image("data:image/jpg;base64,");
        image.append(QString::fromLatin1(bArray.toBase64().data()));
        return image;
    }
    else
    {
        return "";
    }
}

void AudioTool::getMetaData()
{
    // I can't get TagLib to work on Windows though, so this mess below has
    // to do

    m_songName = m_musicPlayer->metaData(QMediaMetaData::Title).toString();
    m_album    = m_musicPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
    m_artist   = m_musicPlayer->metaData(QMediaMetaData::Author).toString();

    if (m_artist.isNull()) m_artist = m_musicPlayer->metaData(QMediaMetaData::AlbumArtist).toString();

    if (m_artist.isNull()) m_artist = m_musicPlayer->metaData(QMediaMetaData::Composer).toString();

    m_cover = convertCoverImage();
}

void AudioTool::getMetaDataTagLib()
{
    #ifdef Q_OS_LINUX
    # ifndef Q_OS_ANDROID

    if (m_musicPlayer->bufferStatus() == 100)
    {
        if (m_musicPlayer->currentMedia().resources().size() > 0)
        {
            QString path = m_musicPlaylist->currentMedia().resources().first().url().path();

            // Album, Artist and Title
            TagLib::FileRef f(path.toUtf8());
            m_album    = f.tag()->album().toCString(true);
            m_artist   = f.tag()->artist().toCString(true);
            m_songName =  f.tag()->title().toCString(true);
            m_cover    = convertCoverImage();
        }
    }
    # endif // ifndef Q_OS_ANDROID
    #endif  // ifdef Q_OS_LINUX
}

void AudioTool::onMetaDataChanged()
{
    m_songName.clear();
    m_artist.clear();
    m_album.clear();

    if (m_musicPlayer->isMetaDataAvailable())
    {
        #ifdef Q_OS_LINUX
        # ifndef Q_OS_ANDROID

        // Reading tags using TagLib, as it is way more reliable than the Qt
        // implementation
        if (m_musicNotRadio) getMetaDataTagLib();
        else getMetaData();  // TagLib bugs out with radios somehow

        # else // ifndef Q_OS_ANDROID
        getMetaData();
        # endif // ifndef Q_OS_ANDROID
        #else  // ifdef __linux__
        getMetaData();
        #endif  // ifdef __linux__

        if (m_album.isEmpty()) m_album = " - ";

        if (m_artist.isEmpty()) m_artist = " - ";

        if (m_songName.isEmpty()) m_songName = " - ";
    }

    emit metaDataChanged();
}

void AudioTool::onSpotifyIconChanged(int index, QString url)
{
    if ((index > -1) && (m_elementIcons.size() > index))
    {
        m_elementIcons[index] = url;
        emit elementIconsChanged();
    }
    else
    {
        qDebug() << "Did not find element!";
    }
}
