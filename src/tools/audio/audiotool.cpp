#include "audiotool.h"
#include "src/functions.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <cstdlib>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusMetaType>

AudioTool::AudioTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug().noquote() << "Loading AudioTool ...";

    sManager = new SettingsManager;

    editor         = new AudioEditor(fManager, qmlEngine, this);
    metaDataReader = new MetaDataReader;

    // Music Player
    musicPlayer = new MusicPlayer(fileManager);
    connect(musicPlayer,                        &MusicPlayer::startedPlaying,        this,        &AudioTool::onStartedPlaying);
    connect(musicPlayer,                        &MusicPlayer::songNamesChanged,      [ = ]() { emit songsChanged(); });
    connect(fileManager->getAudioFileManager(), &AudioFileManager::songPathsChanged, musicPlayer, &MusicPlayer::onSongPathsChanged);
    musicPlayers.append(musicPlayer);

    // Sound Player
    soundPlayer = new SoundPlayer(fileManager);
    connect(fileManager->getAudioFileManager(), &AudioFileManager::soundPathsChanged, soundPlayer, &SoundPlayer::onSoundPathsChanged);
    connect(soundPlayer,                        &SoundPlayer::soundsChanged,          this,        &AudioTool::onSoundsChanged);
    soundModel = new AudioElementModel;
    qmlEngine->rootContext()->setContextProperty("soundModel", soundModel);

    // Radio Player
    radioPlayer = new RadioPlayer(fileManager);
    connect(fileManager->getAudioFileManager(), &AudioFileManager::radioPathChanged, radioPlayer, &RadioPlayer::onUrlChanged);
    connect(radioPlayer,                        &RadioPlayer::startedPlaying,        this,        &AudioTool::onStartedPlaying);
    musicPlayers.append(radioPlayer);

    // Spotify
    spotify = new Spotify(fileManager, metaDataReader);
    connect(spotify, &Spotify::startedPlaying,   this, &AudioTool::onStartedPlaying);
    connect(spotify, &Spotify::songNamesChanged, [ = ]() { emit songsChanged(); });
    connect(spotify, &Spotify::authorize,        [ = ](QUrl url) { emit authorizeSpotify(url); });
    musicPlayers.append(spotify);

    elementModel = new AudioElementModel;
    qmlEngine->rootContext()->setContextProperty("elementModel", elementModel);

    connect(fileManager->getAudioFileManager(), &AudioFileManager::projectsChanged, this,           &AudioTool::onProjectsChanged);
    connect(this,                               &AudioTool::currentScenarioChanged, this,           &AudioTool::onCurrentScenarioChanged);

    // Meta Data
    connect(musicPlayer,                        &MusicPlayer::metaDataChanged,      metaDataReader, &MetaDataReader::updateMetaData);
    connect(radioPlayer,                        &RadioPlayer::metaDataChanged,      metaDataReader, &MetaDataReader::updateMetaData);
    connect(metaDataReader,                     &MetaDataReader::metaDataUpdated,   this,           &AudioTool::onMetaDataUpdated);

    // Connect to DBus
    mprisAdaptor       = new MprisAdaptor(this);
    mprisPlayerAdaptor = new MprisPlayerAdaptor(this);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::next,         [ = ]() { next(); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::changeVolume, [ = ](double volume) { setMusicVolume(volume); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::pause,        [ = ]() { if (!m_isPaused) playPause(); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::previous,     [ = ]() { again(); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::playPause,    [ = ]() { playPause(); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::stop,         [ = ]() { if (!m_isPaused) playPause(); });
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::play,         [ = ]() { if (m_isPaused) playPause(); });

    QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this);
    QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2.gm-companion");

    // Find and load projects
    fileManager->getAudioFileManager()->findProjects(fileManager->getModeInt());

    connect(spotify, &Spotify::authorize,                this, &AudioTool::onSpotifyAuthorize);
    connect(spotify, &Spotify::authorized,               this, &AudioTool::onSpotifyAuthorized);
    connect(this,    &AudioTool::currentScenarioChanged, [ = ]() {
        if (spotify) spotify->fetchIcons(m_currentProject->currentCategory()->currentScenario());
    });
}

AudioTool::~AudioTool()
{
    musicPlayer->deleteLater();
    soundPlayer->deleteLater();
    radioPlayer->deleteLater();
    spotify->deleteLater();

    mprisAdaptor->deleteLater();
    mprisPlayerAdaptor->deleteLater();

    editor->deleteLater();
    fileManager->deleteLater();
    metaDataReader->deleteLater();
    delete sManager;
}

/**
    Change the project, notify UI
    @param projects List with pointers to audio projects
 */
void AudioTool::onProjectsChanged(QList<AudioProject *>projects)
{
    m_projects = projects;
    emit projectsChanged();
}

/**
    Set the current project, notify UI
    @param project Name of the project
 */
void AudioTool::setCurrentProject(int index)
{
    if (m_projects.isEmpty()) return;

    qDebug() << "AudioTool: Setting current project:" << index << m_projects[index]->name();
    m_currentProject = m_projects[index];
    emit currentProjectChanged();
    emit currentCategoryChanged();
    emit currentScenarioChanged();
}

/**
    Get the names of all projects
    @return QStringList with project names
 */
QStringList AudioTool::projectNames()
{
    if (m_projects.isEmpty()) return {};

    QStringList list;

    for (AudioProject *p : m_projects)
    {
        list.append(p->name());
    }

    return list;
}

/**
    Set the current category, notify UI
    @param category Name of the category
 */
void AudioTool::setCurrentCategory(QString category)
{
    if (!m_currentProject) return;

    m_currentProject->setCurrentCategory(category);
    emit currentCategoryChanged();
    emit currentScenarioChanged();
}

/**
    Set the current scenario, notify UI
    @return QStringList with scenario names
 */
void AudioTool::setCurrentScenario(QString scenario)
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return;

    m_currentProject->currentCategory()->setCurrentScenario(scenario);
    emit currentScenarioChanged();
}

/**
 * @brief Update element model
 */
void AudioTool::onCurrentScenarioChanged()
{
    elementModel->setElements(elements());

    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario()) return;

    spotify->addElements(m_currentProject->currentCategory()->currentScenario()->spotifyElements());
}

/**
 * @brief Start playing an element
 * @param name Name of the element
 * @param type Type of the element. 0: Music, 1: Sounds, 2: Radio, 3: Spotify
 */
void AudioTool::playElement(QString name, int type)
{
    if (!m_currentProject) return;

    AudioScenario *scenario = m_currentProject->currentCategory()->currentScenario();

    if (type != 1) m_musicMode = type;

    switch (type)
    {
    case 0: // Music
        spotify->stop();
        radioPlayer->stop();
        musicPlayer->play(scenario->musicElement(name));
        break;

    case 1: // Sounds
        soundPlayer->play(scenario->soundElement(name));
        break;

    case 2: // Radio
        spotify->stop();
        musicPlayer->stop();
        radioPlayer->play(scenario->radioElement(name));
        break;

    case 3: // Spotify
        musicPlayer->stop();
        radioPlayer->stop();
        spotify->play(scenario->spotifyElement(name));
        break;
    }
}

/**
 * @brief Switch to next song
 */
void AudioTool::next()
{
    switch (m_musicMode)
    {
    case 0: musicPlayer->next(); break;

    case 3: spotify->next(); break;

    default: return;
    }
}

/**
 * @brief Set volume of music players (music, radio, spotify)
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setMusicVolume(float volume)
{
    qDebug() << "AudioTool: Setting music volume ...";

    // Convert volume to logarithmic scale
    float logarithmicVolume = qRound(QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) * 100);
    int   linearVolume      = volume * 100;

    for (auto a : musicPlayers)
    {
        a->setVolume(logarithmicVolume);
        a->setVolume(linearVolume);
    }

    mprisPlayerAdaptor->setVolume(logarithmicVolume);
}

/**
 * @brief Set volume of sound players
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setSoundVolume(float volume)
{
    qDebug() << "AudioTool: Setting sound volume ...";

    float logarithmicVolume = qRound(QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) * 100);
    soundPlayer->setVolume(logarithmicVolume);
}

/**
 * @brief Toggle between play and pause
 */
void AudioTool::playPause()
{
    if (m_isPaused)
    {
        switch (m_musicMode)
        {
        case 0:  musicPlayer->play(); break;

        case 2: radioPlayer->play();  break;

        case 3: spotify->play(); break;

        default: break;
        }
    }
    else
    {
        switch (m_musicMode)
        {
        case 0: musicPlayer->pause(); break;

        case 2: radioPlayer->pause(); break;

        case 3: spotify->pause(); break;

        default: break;
        }

        m_isPaused = true;
        emit isPausedChanged();

        mprisPlayerAdaptor->setPlaybackStatus(2);
        sendMprisUpdateSignal("PlaybackStatus", mprisPlayerAdaptor->playbackStatus());
    }
}

/**
 * @brief Start playback of current song again
 */
void AudioTool::again()
{
    switch (m_musicMode)
    {
    case 0: musicPlayer->again(); break;

    case 3: spotify->again(); break;

    default: break;
    }
}

/**
 * @brief Get list of song names depending on mode
 * @return QStringList of song names
 */
QStringList AudioTool::songs() const
{
    switch (m_musicMode)
    {
    case 0: return musicPlayer->songNames();

    case 3: return spotify->songNames();

    default: return {};
    }
}

/**
 * @brief Get index of current song in playlist
 * @return Index as integer
 */
int AudioTool::index() const
{
    switch (m_musicMode)
    {
    case 0: return musicPlayer->index();

    case 3: return spotify->index();

    default: return 0;
    }
}

/**
 * @brief Switch to song at index
 * @param index Index of the new song in playlist
 */
void AudioTool::setMusicIndex(int index)
{
    switch (m_musicMode)
    {
    case 0: musicPlayer->setIndex(index); break;

    case 3: spotify->setIndex(index); break;

    default: return;
    }
}

/**
 * @brief Update the list of active songs when they change
 * @param elements List of active sound elements
 */
void AudioTool::onSoundsChanged(QList<SoundElement *>elements)
{
    qDebug() << "AudioTool: Sounds changed!";

    QList<AudioElement *> list;

    for (auto a : elements)
    {
        list.append(a);
    }

    soundModel->setElements(list);
}

/**
 * @brief Send a PropertiesChanged notification via dbus
 * @param property Name of the property
 * @param value Value of the property
 */
void AudioTool::sendMprisUpdateSignal(QString property, QVariant value)
{
    QDBusMessage signal = QDBusMessage::createSignal(
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged");

    signal << "org.mpris.MediaPlayer2.Player";

    QVariantMap changedProps;
    changedProps.insert(property, value);
    signal << changedProps;

    QStringList invalidatedProps;
    signal << invalidatedProps;

    QDBusConnection::sessionBus().send(signal);
}

void AudioTool::onMetaDataUpdated(MetaData metaData)
{
    m_metaData = metaData;
    emit metaDataChanged();

    // Change MPRIS Metadata
    QMap<QString, QVariant> map;
    map.insert("mpris:trackid",     "gmcompanion:music");
    map.insert("mpris:length",      metaData.length);
    map.insert("mpris:artUrl",      "file://" + m_metaData.elementIcon);
    map.insert("xesam:album",       m_metaData.album.isEmpty() ? tr("Unknown Album") : m_metaData.album);
    map.insert("xesam:albumArtist", m_metaData.artist.isEmpty() ? QStringList({ tr("Unknown Artist") }) : QStringList({ m_metaData.artist }));
    map.insert("xesam:artist",      m_metaData.artist.isEmpty() ? QStringList({ tr("Unknown Artist") }) : QStringList({ m_metaData.artist }));
    map.insert("xesam:title",       m_metaData.title.isEmpty() ? tr("Unknown Title") : m_metaData.title);
    mprisPlayerAdaptor->setMetadata(map);

    sendMprisUpdateSignal("Metadata", map);
}
