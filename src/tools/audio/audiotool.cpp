#include "audiotool.h"
#include "audiosaveload.h"
#include "services/spotify/spotify.h"
#include "thumbnails/audiothumbnailgenerator.h"
#include "utils/utils.h"
#include <QLoggingCategory>
#include <QQmlContext>
#include <QSettings>
#include <cstdlib>
#include <exception>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioTool, "gm.audio.tool")

AudioTool::AudioTool(QQmlApplicationEngine *engine, QNetworkAccessManager &networkManager, QObject *parent)
    : AbstractTool(parent), editor(engine), musicPlayer(metaDataReader), radioPlayer(networkManager, metaDataReader)
{
    qCDebug(gmAudioTool()) << "Loading ...";

    // QML Engine
    engine->rootContext()->setContextProperty(u"audio_tool"_s, this);
    engine->rootContext()->setContextProperty(u"audio_editor"_s, &editor);

    // Spotify
    connect(Spotify::instance(), &Spotify::authorized, this, &AudioTool::onSpotifyAuthorized);

    // Music Player
    connect(&musicPlayer, &MusicPlayer::startedPlaying, this, &AudioTool::onStartedPlaying);
    connect(&musicPlayer, &MusicPlayer::playlistChanged, this, [this](const QList<AudioFile *> &files) {
        Q_UNUSED(files);
        emit playlistChanged();
    });
    connect(&musicPlayer, &MusicPlayer::playlistChanged, this,
            [](const QList<AudioFile *> &files) { qCDebug(gmAudioTool()) << "Playlist Changed!" << files.length(); });
    connect(&musicPlayer, &MusicPlayer::currentIndexChanged, this, &AudioTool::currentIndexChanged);

    // Radio Player
    connect(&radioPlayer, &RadioPlayer::startedPlaying, this, &AudioTool::onStartedPlaying);

    // Meta Data
    connect(&metaDataReader, &MetaDataReader::metaDataChanged, this, &AudioTool::onMetaDataUpdated);

    // Mpris
    connect(&mprisManager, &MprisManager::play, this, [this]() {
        if (m_isPaused) playPause();
    });
    connect(&mprisManager, &MprisManager::playPause, this, [this]() { playPause(); });
    connect(&mprisManager, &MprisManager::pause, this, [this]() {
        if (!m_isPaused) playPause();
    });
    connect(&mprisManager, &MprisManager::stop, this, [this]() {
        if (!m_isPaused) playPause();
    });
    connect(&mprisManager, &MprisManager::next, this, [this]() { next(); });
    connect(&mprisManager, &MprisManager::previous, this, [this]() { again(); });
    connect(&mprisManager, &MprisManager::changeVolume, this, [this](double volume) { setMusicVolume(volume); });
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
        this, [this](const std::vector<AudioProject *> &projects) { onProjectsChanged(projects); });
}

auto AudioTool::currentProjectName() const -> QString
{
    if (m_currentProject)
    {
        return m_currentProject->name();
    }

    return u""_s;
}

/**
    Change the project, notify UI
    @param projects List with pointers to audio projects
 */
void AudioTool::onProjectsChanged(const std::vector<AudioProject *> &projects)
{
    m_projects = Utils::toList<AudioProject *>(projects);

    foreach (auto *project, m_projects)
    {
        project->setParent(this);
    }

    emit projectsChanged();
    isLoading(false);
}

void AudioTool::onCurrentScenarioChanged() const
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

    qCDebug(gmAudioTool) << "Setting current project:" << index << m_projects.at(index)->name();

    if (m_currentProject)
    {
        disconnect(m_currentProject, &AudioProject::currentScenarioChanged, this, &AudioTool::onCurrentScenarioChanged);
    }

    m_currentProject = m_projects.at(index);
    onCurrentScenarioChanged();

    if (m_currentProject)
    {
        connect(m_currentProject, &AudioProject::currentScenarioChanged, this, &AudioTool::onCurrentScenarioChanged);
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
        radioPlayer.stop();
        musicPlayer.play(element);
        setMusicVolume(m_musicVolume);
        break;

    case AudioElement::Type::Sound:
        soundPlayerController.play(element);
        setSoundVolume(m_soundVolume);
        break;

    case AudioElement::Type::Radio:
        m_musicElementType = element->type();
        musicPlayer.stop();
        radioPlayer.play(element);
        setMusicVolume(m_musicVolume);
        break;
    }

    metaDataReader.updateMetaData(QMediaMetaData::MediaType, element->name());
}

void AudioTool::onStartedPlaying()
{
    m_isPaused = false;
    emit isPausedChanged();
    mprisManager.setPlaybackStatus(1);
}

/**
 * @brief Switch to next song
 */
void AudioTool::next()
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        musicPlayer.next();
        break;
    default:
        return;
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
            musicPlayer.play();
            break;
        case AudioElement::Type::Radio:
            radioPlayer.play();
            break;
        default:
            break;
        }
    }
    else
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

        m_isPaused = true;
        emit isPausedChanged();
        mprisManager.setPlaybackStatus(2);
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
        musicPlayer.again();
        break;
    default:
        break;
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

    musicPlayer.setVolume(linearVolume, logarithmicVolume);
    radioPlayer.setVolume(linearVolume, logarithmicVolume);
    mprisManager.setVolume(logarithmicVolume);
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

    soundPlayerController.setVolume(linearVolume, logarithmicVolume);
}

auto AudioTool::makeLinearVolume(qreal linearVolume) -> int
{
    return qRound(linearVolume * VOLUME_FACTOR);
}

auto AudioTool::makeLogarithmicVolume(qreal linearVolume) -> int
{
    return qRound(QAudio::convertVolume(linearVolume, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale) *
                  VOLUME_FACTOR);
}

auto AudioTool::playlist() const -> QList<AudioFile *>
{
    switch (m_musicElementType)
    {
    case AudioElement::Type::Music:
        return musicPlayer.playlist();
    default:
        return {};
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
        return musicPlayer.playlistIndex();
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
        musicPlayer.setIndex(index);
        break;
    default:
        return;
    }
}

void AudioTool::onMetaDataUpdated()
{
    mprisManager.updateMetaData(metaDataReader.metaData());
    emit metaDataChanged();
    emit currentIndexChanged();
}

void AudioTool::findElement(const QString &term) const
{
    AudioScenario::setFilterString(term);

    if (m_currentProject && m_currentProject->currentCategory())
    {
        m_currentProject->currentCategory()->refreshElements();
    }
}
