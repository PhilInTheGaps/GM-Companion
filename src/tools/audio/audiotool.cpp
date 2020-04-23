#include "audiotool.h"
#include "audioelementimageprovider.h"
#include "audioicongenerator.h"
#include "services/spotify.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <cstdlib>
#include <exception>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusMetaType>

AudioTool::AudioTool(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), qmlEngine(engine)
{
    qDebug().noquote() << "Loading AudioTool ...";

    editor         = new AudioEditor(qmlEngine, &audioSaveLoad, this);
    metaDataReader = new MetaDataReader;

    // QML Engine
    engine->rootContext()->setContextProperty("audio_tool", this);
    engine->rootContext()->setContextProperty("audio_editor", editor);
    engine->addImageProvider("audioElementIcons", new AudioElementImageProvider);

    // Spotify
    spotifyPlayer = new SpotifyPlayer(metaDataReader);
    connect(Spotify::getInstance(), &Spotify::authorized, this, &AudioTool::onSpotifyAuthorized);

    // Music Player
    musicPlayer = new MusicPlayer(spotifyPlayer);
    connect(musicPlayer, &MusicPlayer::startedPlaying,      this, &AudioTool::onStartedPlaying);
    connect(musicPlayer, &MusicPlayer::songNamesChanged,    [ = ]() { emit songsChanged(); });
    connect(musicPlayer, &MusicPlayer::currentIndexChanged, this, &AudioTool::currentIndexChanged);

    audioPlayers.append(static_cast<AudioPlayer *>(musicPlayer));

    // Sound Player
    soundPlayer = new SoundPlayerController;
    connect(soundPlayer, &SoundPlayerController::soundsChanged, this, &AudioTool::onSoundsChanged);
    soundModel = new AudioElementModel;
    qmlEngine->rootContext()->setContextProperty("soundModel", soundModel);

    // Radio Player
    radioPlayer = new RadioPlayer;
    connect(radioPlayer, &RadioPlayer::startedPlaying, this, &AudioTool::onStartedPlaying);
    audioPlayers.append(radioPlayer);

    elementModel = new AudioElementModelModel;
    elementModel->setElements({ new AudioElementModel });
    qmlEngine->rootContext()->setContextProperty("elementModel", elementModel);

    connect(this,           &AudioTool::currentScenarioChanged,                                            this,           &AudioTool::onCurrentScenarioChanged);

    // Meta Data
    connect(musicPlayer,    QOverload<QMediaPlayer *>::of(&MusicPlayer::metaDataChanged),                  metaDataReader, QOverload<QMediaPlayer *>::of(&MetaDataReader::updateMetaData));
    connect(radioPlayer,    QOverload<QMediaPlayer *>::of(&RadioPlayer::metaDataChanged),                  metaDataReader, QOverload<QMediaPlayer *>::of(&MetaDataReader::updateMetaData));
    connect(metaDataReader, &MetaDataReader::metaDataUpdated,                                              this,           &AudioTool::onMetaDataUpdated);
    connect(musicPlayer,    &MusicPlayer::clearMetaData,                                                   metaDataReader, &MetaDataReader::clearMetaData);
    connect(musicPlayer,    QOverload<const QString&, const QVariant&>::of(&MusicPlayer::metaDataChanged), metaDataReader,
            QOverload<const QString&, const QVariant&>::of(&MetaDataReader::updateMetaData));
    connect(radioPlayer,    QOverload<const QString&, const QVariant&>::of(&RadioPlayer::metaDataChanged), metaDataReader,
            QOverload<const QString&, const QVariant&>::of(&MetaDataReader::updateMetaData));

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
    QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2.gm_companion");
#endif // ifdef NO_DBUS

    // Find and load projects
    connect(&audioSaveLoad, &AudioSaveLoad::foundProjects, this, &AudioTool::onProjectsChanged);
    audioSaveLoad.findProjects();
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
    metaDataReader->deleteLater();
}

/**
    Change the project, notify UI
    @param projects List with pointers to audio projects
 */
void AudioTool::onProjectsChanged(QList<AudioProject *>projects, bool forEditor)
{
    if (forEditor) return;

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

    AudioIconGenerator::generateIcons(m_currentProject->currentCategory()->currentScenario());
}

void AudioTool::onStartedPlaying()
{
    m_isPaused = false;
    emit isPausedChanged();

    if (mprisAdaptor && mprisPlayerAdaptor)
    {
        mprisPlayerAdaptor->setPlaybackStatus(1);
        sendMprisUpdateSignal("PlaybackStatus", mprisPlayerAdaptor->playbackStatus());
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
        setMusicVolume(m_musicVolume);
        break;

    case 1: // Sounds
        soundPlayer->play(scenario->soundElement(name));
        setSoundVolume(m_soundVolume);
        break;

    case 2: // Radio
        musicPlayer->stop();
        radioPlayer->play(scenario->radioElement(name));
        setMusicVolume(m_musicVolume);
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
void AudioTool::setMusicVolume(qreal volume)
{
    // Convert volume to logarithmic scale
    int logarithmicVolume = qRound(QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) * 100);
    int linearVolume      = qRound(volume * 100);

    m_musicVolume = volume;

    qDebug() << "AudioTool: Setting music volume" << linearVolume;

    for (auto a : audioPlayers)
    {
        a->setLogarithmicVolume(logarithmicVolume);
        a->setLinearVolume(linearVolume);
    }

    if (mprisPlayerAdaptor) mprisPlayerAdaptor->setVolume(logarithmicVolume);
}

/**
 * @brief Set volume of sound players
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setSoundVolume(qreal volume)
{
    qDebug() << "AudioTool: Setting sound volume ...";

    int logarithmicVolume = qRound(QAudio::convertVolume(volume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) * 100);
    m_soundVolume = volume;
    soundPlayer->setLogarithmicVolume(logarithmicVolume);
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
void AudioTool::onSoundsChanged(QList<AudioElement *>elements)
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
    map.insert("mpris:trackid",     QDBusObjectPath("/lol/rophil/gm_companion/audio/current_track"));
    map.insert("mpris:length",      metaData.length);
    map.insert("mpris:artUrl",      m_metaData.coverUrl);
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
