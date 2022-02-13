#include "audiotool.h"
#include "thumbnails/audiothumbnailgenerator.h"
#include "services/spotify/spotify.h"
#include "logging.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QSettings>
#include <QQmlContext>
#include <cstdlib>
#include <exception>

using namespace AsyncFuture;

AudioTool::AudioTool(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), a_isLoading(false), qmlEngine(engine)
{
    qCDebug(gmAudioTool()) << "Loading ...";

    auto *networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    editor         = new AudioEditor(qmlEngine, networkManager, this);
    metaDataReader = new MetaDataReader(this);
    mprisManager   = new MprisManager(this);
    discordPlayer  = new DiscordPlayer(this);
    musicPlayer    = new MusicPlayer(metaDataReader, discordPlayer, this);
    radioPlayer    = new RadioPlayer(metaDataReader, discordPlayer, this);
    audioPlayers   = { musicPlayer, radioPlayer };

    soundPlayerController = new SoundPlayerController(discordPlayer, this);

    // QML Engine
    engine->rootContext()->setContextProperty("audio_tool", this);
    engine->rootContext()->setContextProperty("audio_editor", editor);

    // Spotify
    connect(Spotify::instance(), &Spotify::authorized, this, &AudioTool::onSpotifyAuthorized);

    // Music Player
    connect(musicPlayer, &MusicPlayer::startedPlaying,      this, &AudioTool::onStartedPlaying);
    connect(musicPlayer, &MusicPlayer::songNamesChanged,    this, [ = ](const QStringList &names) { Q_UNUSED(names); emit songsChanged(); });
    connect(musicPlayer, &MusicPlayer::currentIndexChanged, this, &AudioTool::currentIndexChanged);

    // Radio Player
    connect(radioPlayer, &RadioPlayer::startedPlaying, this, &AudioTool::onStartedPlaying);

    // Meta Data
    connect(metaDataReader, &MetaDataReader::metaDataChanged, this, &AudioTool::onMetaDataUpdated);

    // Mpris
    connect(mprisManager, &MprisManager::play,         this, [ = ]() { if (m_isPaused) playPause(); });
    connect(mprisManager, &MprisManager::playPause,    this, [ = ]() { playPause(); });
    connect(mprisManager, &MprisManager::pause,        this, [ = ]() { if (!m_isPaused) playPause(); });
    connect(mprisManager, &MprisManager::stop,         this, [ = ]() { if (!m_isPaused) playPause(); });
    connect(mprisManager, &MprisManager::next,         this, [ = ]() { next(); });
    connect(mprisManager, &MprisManager::previous,     this, [ = ]() { again(); });
    connect(mprisManager, &MprisManager::changeVolume, this, [ = ](double volume) { setMusicVolume(volume); });
}

void AudioTool::loadData()
{
    if (m_isDataLoaded) return;

    isLoading(true);
    m_isDataLoaded = true;

    updateProjectList();
}

void AudioTool::updateProjectList()
{
    observe(AudioSaveLoad::findProjectsAsync()).subscribe([this](QVector<AudioProject*> projects) {
        onProjectsChanged(projects);
    });
}

/**
    Change the project, notify UI
    @param projects List with pointers to audio projects
 */
void AudioTool::onProjectsChanged(QVector<AudioProject *>projects)
{
    m_projects = std::move(projects);

    for (auto *project : m_projects)
    {
        project->setParent(this);
    }

    emit projectsChanged();
    isLoading(false);
}

void AudioTool::onCurrentScenarioChanged()
{
    if (m_currentProject && m_currentProject->currentCategory())
    {
        AudioThumbnailGenerator::generateThumbnails(m_currentProject->currentCategory()->currentScenario());
    }
}

/**
    Set the current project, notify UI
    @param project Name of the project
 */
void AudioTool::setCurrentProject(int index)
{
    if (m_projects.isEmpty()) return;

    qCDebug(gmAudioTool) << "Setting current project:" << index << m_projects[index]->name();

    if (m_currentProject)
    {
        disconnect(m_currentProject, &AudioProject::currentScenarioChanged,
                   this, &AudioTool::onCurrentScenarioChanged);
    }

    m_currentProject = m_projects[index];
    onCurrentScenarioChanged();

    if (m_currentProject)
    {
        connect(m_currentProject, &AudioProject::currentScenarioChanged,
                this, &AudioTool::onCurrentScenarioChanged);
    }

    emit currentProjectChanged();
}

auto AudioTool::getCurrentProjectIndex() -> int
{
    if (!m_currentProject) return 0;

    return m_projects.indexOf(m_currentProject);
}

/**
 * @brief Start to play an element
 */
void AudioTool::play(AudioElement *element)
{
    if (!element)
    {
        stop();
        return;
    }

    switch (element->type())
    {
    case AudioElement::Type::Music:
        m_musicElementType = element->type();
        radioPlayer->stop();
        musicPlayer->play(element);
        setMusicVolume(m_musicVolume);
        break;

    case AudioElement::Type::Sound:
        soundPlayerController->play(element);
        setSoundVolume(m_soundVolume);
        break;

    case AudioElement::Type::Radio:
        m_musicElementType = element->type();
        musicPlayer->stop();
        radioPlayer->play(element);
        setMusicVolume(m_musicVolume);
        break;
    }

    metaDataReader->updateMetaData("Type", element->name());
}

void AudioTool::onStartedPlaying()
{
    m_isPaused = false;
    emit isPausedChanged();
    mprisManager->setPlaybackStatus(1);
}

/**
 * @brief Switch to next song
 */
void AudioTool::next()
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        musicPlayer->next();
        break;
    default: return;
    }
}

/**
 * @brief Toggle between play and pause
 */
void AudioTool::playPause()
{
    if (m_isPaused)
    {
        switch (m_musicElementType)
        {
        case AudioElement::Type::Music:
            musicPlayer->play();
            break;
        case AudioElement::Type::Radio:
            radioPlayer->play();
            break;
        default: break;
        }

        discordPlayer->play();
    }
    else
    {
        switch (m_musicElementType)
        {
        case AudioElement::Type::Music:
            musicPlayer->pause();
            break;
        case AudioElement::Type::Radio:
            radioPlayer->pause();
            break;
        default: break;
        }

        discordPlayer->pause();

        m_isPaused = true;
        emit isPausedChanged();
        mprisManager->setPlaybackStatus(2);
    }
}

/**
 * @brief Stop everything
 */
void AudioTool::stop()
{
    musicPlayer->stop();
    radioPlayer->stop();
    soundPlayerController->stop();
    discordPlayer->pause();
    m_isPaused = true;
    emit isPausedChanged();
}

/**
 * @brief Start playback of current song again
 */
void AudioTool::again()
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        musicPlayer->again();
        break;
    default: break;
    }
}

/**
 * @brief Set volume of music players (music, radio)
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setMusicVolume(qreal volume)
{
    // Convert volume to logarithmic scale
    auto logarithmicVolume = makeLogarithmicVolume(volume);
    auto linearVolume = makeLinearVolume(volume);

    qCDebug(gmAudioTool()) << "Setting music volume:" << linearVolume;
    m_musicVolume = volume;

    for (auto *player : qAsConst(audioPlayers))
    {
        player->setVolume(linearVolume, logarithmicVolume);
    }

    discordPlayer->setMusicVolume(logarithmicVolume);
    mprisManager->setVolume(logarithmicVolume);
}

/**
 * @brief Set volume of sound players
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setSoundVolume(qreal volume)
{
    auto linearVolume = makeLinearVolume(volume);
    auto logarithmicVolume = makeLogarithmicVolume(volume);
    m_soundVolume = volume;

    soundPlayerController->setVolume(linearVolume, logarithmicVolume);
    discordPlayer->setSoundsVolume(logarithmicVolume);
}

auto AudioTool::makeLinearVolume(qreal linearVolume) -> int
{
    return qRound(linearVolume * VOLUME_FACTOR);
}

auto AudioTool::makeLogarithmicVolume(qreal linearVolume) -> int
{
    return qRound(QAudio::convertVolume(linearVolume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) * VOLUME_FACTOR);
}

/**
 * @brief Get list of song names depending on mode
 * @return QStringList of song names
 */
auto AudioTool::songs() const -> QStringList
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        return musicPlayer->songNames();
    default: return {};
    }
}

/**
 * @brief Get index of current song in playlist
 * @return Index as integer
 */
auto AudioTool::index() const -> int
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        return musicPlayer->playlistIndex();
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
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        musicPlayer->setIndex(index);
        break;
    default: return;
    }
}

void AudioTool::onMetaDataUpdated()
{
    mprisManager->updateMetaData(metaDataReader->metaData());
    emit metaDataChanged();
    emit currentIndexChanged();
}

void AudioTool::findElement(const QString& term)
{
    AudioScenario::setFilterString(term);

    if (m_currentProject && m_currentProject->currentCategory())
    {
        m_currentProject->currentCategory()->refreshElements();
    }
}
