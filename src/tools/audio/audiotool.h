#pragma once

#include "common/abstracttool.h"
#include "editor/audioeditor.h"
#include "metadata/metadatareader.h"
#include "mpris/mprismanager.h"
#include "players/musicplayer.h"
#include "players/radioplayer.h"
#include "players/soundplayer.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QQmlEngine>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class AudioTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    READ_PROPERTY2(AudioProject *, currentProject, nullptr)
    READ_LIST_PROPERTY(AudioProject, projects)

    Q_PROPERTY(SoundPlayerController *soundController READ soundController CONSTANT)

    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(qreal musicVolume READ musicVolume NOTIFY musicVolumeChanged)
    Q_PROPERTY(qreal soundVolume READ soundVolume NOTIFY soundVolumeChanged)

    Q_PROPERTY(AudioMetaData *metaData READ metaData NOTIFY metaDataChanged)
    Q_PROPERTY(QQmlListProperty<AudioFile> playlist READ playlistQml NOTIFY playlistChanged)
    Q_PROPERTY(int index READ index NOTIFY currentIndexChanged)

public:
    AudioTool() = delete;
    explicit AudioTool(QQmlEngine *engine, QObject *parent = nullptr);
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> AudioTool *;

    Q_PROPERTY(AudioEditor *editor READ editor CONSTANT)
    [[nodiscard]] auto editor() -> AudioEditor *
    {
        return &m_editor;
    }

    // Project
    void updateProjectList();
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE int getCurrentProjectIndex();

    [[nodiscard]] auto soundController() -> SoundPlayerController *
    {
        return &soundPlayerController;
    }

    // Volume
    [[nodiscard]] auto musicVolume() const -> qreal
    {
        return m_musicVolume;
    }
    Q_INVOKABLE void setMusicVolume(qreal volume);

    [[nodiscard]] auto soundVolume() const -> qreal
    {
        return m_soundVolume;
    }
    Q_INVOKABLE void setSoundVolume(qreal volume);

    // Playback control
    Q_INVOKABLE void play(AudioElement *element);
    Q_INVOKABLE void next();
    Q_INVOKABLE void playPause();
    Q_INVOKABLE void again();
    Q_INVOKABLE void setMusicIndex(int index);
    Q_INVOKABLE void stopSound(const QString &sound)
    {
        soundPlayerController.stop(sound);
    }
    [[nodiscard]] auto isPaused() const -> bool
    {
        return m_isPaused;
    }
    void stop();

    Q_INVOKABLE void findElement(const QString &term) const;

    // Meta Data
    [[nodiscard]] auto metaData() const -> AudioMetaData *
    {
        return metaDataReader.metaData();
    }
    [[nodiscard]] auto index() const -> int;
    [[nodiscard]] auto playlist() const -> QList<AudioFile *>;
    [[nodiscard]] auto playlistQml() -> QQmlListProperty<AudioFile>;

public slots:
    void loadData() override;

signals:
    void isPausedChanged();
    void soundsChanged();
    void metaDataChanged();
    void currentIndexChanged();
    void spotifyAuthorized();
    void musicVolumeChanged();
    void soundVolumeChanged();
    void playlistChanged();

private slots:
    void onProjectsChanged(const std::vector<AudioProject *> &projects);
    void onCurrentScenarioChanged() const;
    void onStartedPlaying();
    void onMetaDataUpdated();
    void onSpotifyAuthorized()
    {
        emit spotifyAuthorized();
    }

private:
    AudioEditor m_editor;
    MetaDataReader metaDataReader;
    MprisManager mprisManager;

    // Players
    MusicPlayer musicPlayer;
    SoundPlayerController soundPlayerController;
    RadioPlayer radioPlayer;

    AudioElement::Type m_musicElementType = AudioElement::Type::Music;
    bool m_isPaused = true;

    // Volume
    static constexpr qreal DEFAULT_MUSIC_VOLUME = 0.25;
    static constexpr qreal DEFAULT_SOUND_VOLUME = 0.25;
    static constexpr int VOLUME_FACTOR = 100;

    qreal m_musicVolume = DEFAULT_MUSIC_VOLUME;
    qreal m_soundVolume = DEFAULT_SOUND_VOLUME;

    static auto makeLinearVolume(qreal linearVolume) -> int;
    static auto makeLogarithmicVolume(qreal linearVolume) -> int;
};
