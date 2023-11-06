#pragma once

#include "common/abstracttool.h"
#include "editor/audioeditor.h"
#include "metadata/metadatareader.h"
#include "players/musicplayer.h"
#include "players/radioplayer.h"
#include "players/soundplayercontroller.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QQmlEngine>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

#ifndef NO_DBUS
#include "mpris/mprismanager.h"
#endif

class AudioTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    READ_PROPERTY2(AudioProject *, currentProject, nullptr)
    READ_LIST_PROPERTY(AudioProject, projects)
    AUTO_PROPERTY(QString, currentElementName)

    Q_PROPERTY(SoundPlayerController *soundController READ soundController CONSTANT)

    AUTO_PROPERTY_VAL2(AudioPlayer::State, playbackState, AudioPlayer::State::Initialized)
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
    Q_INVOKABLE qsizetype getCurrentProjectIndex() const;

    [[nodiscard]] auto soundController() -> SoundPlayerController *
    {
        return &soundPlayerController;
    }

    // Volume
    [[nodiscard]] auto musicVolume() const -> qreal
    {
        return m_musicVolume;
    }
    Q_INVOKABLE void setMusicVolume(float volume);

    [[nodiscard]] auto soundVolume() const -> qreal
    {
        return m_soundVolume;
    }
    Q_INVOKABLE void setSoundVolume(float volume);

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
    void stop();

    Q_INVOKABLE void findElement(const QString &term) const;

    // Meta Data
    [[nodiscard]] auto metaData() -> AudioMetaData *
    {
        return metaDataReader.metaData();
    }
    [[nodiscard]] auto index() const -> qsizetype;
    [[nodiscard]] auto playlistQml() -> QQmlListProperty<AudioFile>;

public slots:
    void loadData() override;

signals:
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
    void onStateChanged(AudioPlayer::State state);
    void onSpotifyAuthorized()
    {
        emit spotifyAuthorized();
    }

private:
    AudioEditor m_editor;
    MetaDataReader metaDataReader;

#ifndef NO_DBUS
    MprisManager mprisManager;
#endif

    // Players
    MusicPlayer musicPlayer;
    SoundPlayerController soundPlayerController;
    RadioPlayer radioPlayer;

    AudioElement::Type m_musicElementType = AudioElement::Type::Music;

    // Volume
    static constexpr float DEFAULT_MUSIC_VOLUME = 0.25;
    static constexpr float DEFAULT_SOUND_VOLUME = 0.25;
    static constexpr int VOLUME_FACTOR = 100;

    float m_musicVolume = DEFAULT_MUSIC_VOLUME;
    float m_soundVolume = DEFAULT_SOUND_VOLUME;

    static auto makeLinearVolume(float linearVolume) -> int;
    static auto makeLogarithmicVolume(float linearVolume) -> int;
};
