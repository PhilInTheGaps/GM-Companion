#include "audiotool.h"
#include "audiosaveload.h"
#include "services/spotify/spotify.h"
#include "settings/settingsmanager.h"
#include "thumbnails/audiothumbnailgenerator.h"
#include "utils/utils.h"
#include <QLoggingCategory>
#include <QQmlContext>
#include <QSettings>
#include <cstdlib>
#include <exception>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmAudioTool, "gm.audio.tool")

AudioTool::AudioTool(QQmlEngine *engine, QObject *parent)
    : AbstractTool(parent), m_editor(engine), musicPlayer(engine->networkAccessManager(), metaDataReader),
      soundPlayerController(engine->networkAccessManager()), radioPlayer(engine->networkAccessManager(), metaDataReader)
{
    qCDebug(gmAudioTool()) << "Loading ...";

    // Spotify
    connect(Spotify::instance(), &Spotify::authorized, this, &AudioTool::onSpotifyAuthorized);

    // Music Player
    connect(&musicPlayer, &MusicPlayer::stateChanged, this, &AudioTool::onStateChanged);
    connect(&musicPlayer, &MusicPlayer::playlistChanged, this, &AudioTool::playlistChanged);
    connect(&musicPlayer, &MusicPlayer::playlistIndexChanged, this, &AudioTool::currentIndexChanged);

    // Radio Player
    connect(&radioPlayer, &RadioPlayer::stateChanged, this, &AudioTool::onStateChanged);

#ifndef NO_DBUS
    mprisManager.setMetaDataReader(&metaDataReader);
    // Mpris
    connect(&mprisManager, &MprisManager::play, this, [this]() {
        if (playbackState() != AudioPlayer::State::Playing) playPause();
    });
    connect(&mprisManager, &MprisManager::playPause, this, [this]() { playPause(); });
    connect(&mprisManager, &MprisManager::pause, this, [this]() {
        if (playbackState() == AudioPlayer::State::Playing) playPause();
    });
    connect(&mprisManager, &MprisManager::stop, this, [this]() {
        if (playbackState() == AudioPlayer::State::Playing) playPause();
    });
    connect(&mprisManager, &MprisManager::next, this, [this]() { next(); });
    connect(&mprisManager, &MprisManager::previous, this, [this]() { again(); });
    connect(&mprisManager, &MprisManager::changeVolume, this,
            [this](double volume) { setMusicVolume(static_cast<float>(volume)); });
#endif

    setMusicVolume(SettingsManager::instance()->get("musicVolume"_L1, DEFAULT_MUSIC_VOLUME));
    setSoundVolume(SettingsManager::instance()->get("soundVolume"_L1, DEFAULT_SOUND_VOLUME));
}

auto AudioTool::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> AudioTool *
{
    Q_UNUSED(jsEngine)
    return new AudioTool(qmlEngine, qmlEngine);
}

void AudioTool::loadData()
{
    if (isDataLoaded()) return;

    isLoading(true);
    setIsDataLoaded(true);

    updateProjectList();
}

void AudioTool::updateProjectList()
{
    AudioSaveLoad::findProjectsAsync(this).then(
        [this](const std::vector<AudioProject *> &projects) { onProjectsChanged(projects); });
}

/**
    Change the project, notify UI
    @param projects List with pointers to audio projects
 */
void AudioTool::onProjectsChanged(const std::vector<AudioProject *> &projects)
{
    a_projects = Utils::toList<AudioProject *>(projects);

    emit projectsChanged();
    isLoading(false);
}

void AudioTool::onCurrentScenarioChanged() const
{
    if (currentProject() && currentProject()->currentCategory())
    {
        AudioThumbnailGenerator::generateThumbnails(currentProject()->currentCategory()->currentScenario());
    }
}

/**
    Set the current project, notify UI
    @param project Name of the project
 */
void AudioTool::setCurrentProject(int index)
{
    if (a_projects.isEmpty()) return;

    qCDebug(gmAudioTool) << "Setting current project:" << index << a_projects.at(index)->name();

    if (currentProject())
    {
        disconnect(currentProject(), &AudioProject::currentScenarioChanged, this, &AudioTool::onCurrentScenarioChanged);
    }

    a_currentProject = a_projects.at(index);
    onCurrentScenarioChanged();

    if (currentProject())
    {
        connect(currentProject(), &AudioProject::currentScenarioChanged, this, &AudioTool::onCurrentScenarioChanged);
    }

    emit currentProjectChanged(a_currentProject);
}

auto AudioTool::getCurrentProjectIndex() const -> qsizetype
{
    if (!currentProject()) return 0;

    return a_projects.indexOf(currentProject());
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

    auto prepareMusic = [this](const AudioElement &element) {
        metaDataReader.clearMetaData();
        m_musicElementType = element.type();
        setMusicVolume(m_musicVolume);
        currentElementName(element.name());
    };

    switch (element->type())
    {
    case AudioElement::Type::Music:
        prepareMusic(*element);
        radioPlayer.stop();
        musicPlayer.play(element);
        break;
    case AudioElement::Type::Radio:
        prepareMusic(*element);
        emit playlistChanged();
        musicPlayer.stop();
        radioPlayer.play(element);
        break;
    case AudioElement::Type::Sound:
        soundPlayerController.play(element);
        setSoundVolume(m_soundVolume);
        break;
    }
}

void AudioTool::onStateChanged(AudioPlayer::State state)
{
    playbackState(state);

#ifndef NO_DBUS
    mprisManager.setPlaybackStatus(state);
#endif
}

/**
 * @brief Switch to next song
 */
void AudioTool::next()
{
    if (m_musicElementType == AudioElement::Type::Music)
    {
        musicPlayer.next();
    }
}

/**
 * @brief Toggle between play and pause
 */
void AudioTool::playPause()
{
    if (playbackState() == AudioPlayer::State::Playing)
    {
        switch (m_musicElementType)
        {
        case AudioElement::Type::Music:
            musicPlayer.pause();
            break;
        case AudioElement::Type::Radio:
            radioPlayer.pause();
            break;
        default:
            break;
        }

        return;
    }

    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        musicPlayer.play();
        break;
    case AudioElement::Type::Radio:
        radioPlayer.play();
        break;
    default:
        break;
    }
}

/**
 * @brief Stop everything
 */
void AudioTool::stop()
{
    musicPlayer.stop();
    radioPlayer.stop();
    soundPlayerController.stop();
    playbackState(AudioPlayer::State::Stopped);
}

/**
 * @brief Start playback of current song again
 */
void AudioTool::again()
{
    if (m_musicElementType == AudioElement::Type::Music)
    {
        musicPlayer.again();
    }
}

/**
 * @brief Set volume of music players (music, radio)
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setMusicVolume(float volume)
{
    // Convert volume to logarithmic scale
    auto logarithmicVolume = makeLogarithmicVolume(volume);
    auto linearVolume = makeLinearVolume(volume);

    qCDebug(gmAudioTool()) << "Setting music volume:" << linearVolume;
    m_musicVolume = volume;

    musicPlayer.setVolume(linearVolume, logarithmicVolume);
    radioPlayer.setVolume(linearVolume, logarithmicVolume);

#ifndef NO_DBUS
    mprisManager.setVolume(logarithmicVolume);
#endif

    SettingsManager::instance()->set("musicVolume"_L1, volume);
}

/**
 * @brief Set volume of sound players
 * @param volume Volume, float between 0 and 1
 */
void AudioTool::setSoundVolume(float volume)
{
    auto linearVolume = makeLinearVolume(volume);
    auto logarithmicVolume = makeLogarithmicVolume(volume);
    m_soundVolume = volume;

    soundPlayerController.setVolume(linearVolume, logarithmicVolume);

    SettingsManager::instance()->set("soundVolume"_L1, volume);
}

auto AudioTool::makeLinearVolume(float linearVolume) -> int
{
    return qRound(linearVolume * VOLUME_FACTOR);
}

auto AudioTool::makeLogarithmicVolume(float linearVolume) -> int
{
    return qRound(QAudio::convertVolume(linearVolume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) *
                  VOLUME_FACTOR);
}

auto AudioTool::playlistQml() -> QQmlListProperty<AudioFile>
{
    if (m_musicElementType == AudioElement::Type::Music)
    {
        return musicPlayer.playlistQml();
    }

    return {};
}

/**
 * @brief Get index of current song in playlist
 * @return Index as integer
 */
auto AudioTool::index() const -> qsizetype
{
    if (m_musicElementType == AudioElement::Type::Music)
    {
        qCDebug(gmAudioTool()) << musicPlayer.playlistIndex();
        return musicPlayer.playlistIndex();
    }

    return 0;
}

/**
 * @brief Switch to song at index
 * @param index Index of the new song in playlist
 */
void AudioTool::setMusicIndex(int index)
{
    if (m_musicElementType == AudioElement::Type::Music)
    {
        musicPlayer.setIndex(index);
    }
}
