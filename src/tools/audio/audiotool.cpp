#include "audiotool.h"
#include "src/functions.h"
#include "src/services/spotify.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <cstdlib>
#include <exception>

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

    // Spotify
    spotifyPlayer = new SpotifyPlayer(fManager, metaDataReader);
    connect(Spotify::getInstance(), &Spotify::authorize,  [ = ](QUrl url) { emit authorizeSpotify(url); });
    connect(Spotify::getInstance(), &Spotify::authorize,  this, &AudioTool::onSpotifyAuthorize);
    connect(Spotify::getInstance(), &Spotify::authorized, this, &AudioTool::onSpotifyAuthorized);

    // Music Player
    musicPlayer = new MusicPlayer(fileManager, spotifyPlayer);
    connect(musicPlayer, &MusicPlayer::startedPlaying,      this, &AudioTool::onStartedPlaying);
    connect(musicPlayer, &MusicPlayer::songNamesChanged,    [ = ]() { emit songsChanged(); });
    connect(musicPlayer, &MusicPlayer::currentIndexChanged, this, &AudioTool::currentIndexChanged);

    musicPlayers.append(static_cast<AudioPlayer *>(musicPlayer));

    // Sound Player
    soundPlayer = new SoundPlayer(fileManager);
    connect(soundPlayer, &SoundPlayer::soundsChanged, this, &AudioTool::onSoundsChanged);
    soundModel = new AudioElementModel;
    qmlEngine->rootContext()->setContextProperty("soundModel", soundModel);

    // Radio Player
    radioPlayer = new RadioPlayer(fileManager);
    connect(fileManager->getAudioSaveLoad(), &AudioSaveLoad::radioPathChanged, radioPlayer, &RadioPlayer::onUrlChanged);
    connect(radioPlayer,                     &RadioPlayer::startedPlaying,     this,        &AudioTool::onStartedPlaying);
    musicPlayers.append(radioPlayer);

    elementModel = new AudioElementModelModel;
    elementModel->setElements({ new AudioElementModel });
    qmlEngine->rootContext()->setContextProperty("elementModel", elementModel);

    connect(fileManager->getAudioSaveLoad(), &AudioSaveLoad::foundProjects,      [ = ](QList<AudioProject *>projects, bool isEditor) {
        if (!isEditor) onProjectsChanged(projects);
    });
    connect(this,                            &AudioTool::currentScenarioChanged, this,           &AudioTool::onCurrentScenarioChanged);

    // Meta Data
    connect(musicPlayer,                     &MusicPlayer::metaDataChanged,      metaDataReader, &MetaDataReader::updateMetaData);
    connect(radioPlayer,                     &RadioPlayer::metaDataChanged,      metaDataReader, &MetaDataReader::updateMetaData);
    connect(metaDataReader,                  &MetaDataReader::metaDataUpdated,   this,           &AudioTool::onMetaDataUpdated);

#ifndef NO_DBUS

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
#endif // ifdef NO_DBUS

    // Find and load projects
    fileManager->getAudioSaveLoad()->findProjects(fileManager->getModeInt());
}

AudioTool::~AudioTool()
{
    musicPlayer->deleteLater();
    soundPlayer->deleteLater();
    radioPlayer->deleteLater();
    spotifyPlayer->deleteLater();

    if (mprisAdaptor) mprisAdaptor->deleteLater();

    if (mprisPlayerAdaptor) mprisPlayerAdaptor->deleteLater();

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
    if (elements().size() > 0) elementModel->element(0)->setElements(elements()[0]);
    else elementModel->element(0)->setElements({});

    elementModel->clear();

    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioTool: Scenario Names:" << m_currentProject->currentCategory()->scenarios().size() << m_currentProject->currentCategory()->currentScenario()->scenarioNames();

    for (auto s : m_currentProject->currentCategory()->currentScenario()->scenarios())
    {
        auto m = new AudioElementModel;
        m->setName(s->name());
        m->setElements(s->elements());

        elementModel->append(m);
    }
}

/**
 * @brief Start playing an element
 * @param name Name of the element
 * @param type Type of the element. 0: Music, 1: Sounds, 2: Radio
 * @param subscenario Name of the subscenario, empty if top level
 */
void AudioTool::playElement(QString name, int type, QString subscenario)
{
    if (!m_currentProject) return;

    AudioScenario *scenario;

    if (subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(subscenario);
    }

    if (type != 1) m_musicMode = type;

    switch (type)
    {
    case 0: // Music
        radioPlayer->stop();
        musicPlayer->play(scenario->musicElement(name));
        break;

    case 1: // Sounds
        soundPlayer->play(scenario->soundElement(name));
        break;

    case 2: // Radio
        musicPlayer->stop();
        radioPlayer->play(scenario->radioElement(name));
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

    default: return;
    }
}

/**
 * @brief Set volume of music players (music, radio)
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

    if (mprisPlayerAdaptor) mprisPlayerAdaptor->setVolume(logarithmicVolume);
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

        default: break;
        }
    }
    else
    {
        switch (m_musicMode)
        {
        case 0: musicPlayer->pause(); break;

        case 2: radioPlayer->pause(); break;

        default: break;
        }

        m_isPaused = true;
        emit isPausedChanged();

        if (mprisPlayerAdaptor)
        {
            mprisPlayerAdaptor->setPlaybackStatus(2);
            sendMprisUpdateSignal("PlaybackStatus", mprisPlayerAdaptor->playbackStatus());
        }
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
    case 0:
        return musicPlayer->index();

    default:
        return 0;
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

    if (mprisPlayerAdaptor) mprisPlayerAdaptor->setMetadata(map);

    sendMprisUpdateSignal("Metadata", map);
    emit currentIndexChanged();
}

QList<QList<AudioElement *> >AudioTool::elements() const
{
    if (m_currentProject)
    {
        QList<QList<AudioElement *> > list;

        if (m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentScenario())
        {
            list.append(m_currentProject->currentCategory()->currentScenario()->elements());

            for (auto s : m_currentProject->currentCategory()->currentScenario()->scenarios())
            {
                list.append(s->elements());
            }

            return list;
        }
    }

    return {};
}

void AudioTool::findElement(QString element)
{
    if (element.isEmpty())
    {
        emit currentScenarioChanged();
        return;
    }

    QList<QList<AudioElement *> > newElements;

    for (auto l : elements())
    {
        QList<AudioElement *> l2;

        for (auto e : l)
        {
            if (e && e->name().contains(element, Qt::CaseInsensitive))
            {
                l2.append(e);
            }
        }

        newElements.append(l2);
    }

    for (int i = 0; i < newElements.length(); i++)
    {
        elementModel->element(i)->setElements(newElements[i]);
    }
}
