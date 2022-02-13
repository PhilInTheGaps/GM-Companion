#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QStringList>
#include <QQmlApplicationEngine>

#include "common/abstracttool.h"
#include "editor/audioeditor.h"
#include "players/musicplayer.h"
#include "players/soundplayer.h"
#include "players/radioplayer.h"
#include "metadata/metadatareader.h"
#include "mpris/mprismanager.h"

#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioTool : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(QObject* currentProject READ currentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(QList<QObject*> projects READ projects NOTIFY projectsChanged)

    Q_PROPERTY(QObject* soundController READ soundController CONSTANT)

    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(qreal musicVolume READ musicVolume NOTIFY musicVolumeChanged)
    Q_PROPERTY(qreal soundVolume READ soundVolume NOTIFY soundVolumeChanged)

    Q_PROPERTY(QObject* metaData READ metaData NOTIFY metaDataChanged)
    Q_PROPERTY(QStringList songs READ songs NOTIFY songsChanged)
    Q_PROPERTY(int index READ index NOTIFY currentIndexChanged)

    AUTO_PROPERTY(bool, isLoading)

public:
    explicit AudioTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    AudioEditor* getEditor() const { return editor; }

    // Project
    QList<QObject*> projects() const { return Utils::toQObjectList(m_projects); }
    QObject* currentProject() const { return m_currentProject; }
    void updateProjectList();
    QString currentProjectName() const { if (m_currentProject) return m_currentProject->name(); else return nullptr; }
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE int getCurrentProjectIndex();

    QObject* soundController() const { return qobject_cast<QObject*>(soundPlayerController); }

    // Volume
    qreal musicVolume() const { return m_musicVolume; }
    Q_INVOKABLE void setMusicVolume(qreal volume);

    qreal soundVolume() const { return m_soundVolume; }
    Q_INVOKABLE void setSoundVolume(qreal volume);

    // Playback control
    Q_INVOKABLE void play(AudioElement *element);
    Q_INVOKABLE void next();
    Q_INVOKABLE void playPause();
    Q_INVOKABLE void again();
    Q_INVOKABLE void setMusicIndex(int index);
    Q_INVOKABLE void stopSound(const QString &sound) { soundPlayerController->stop(sound); }
    bool isPaused() const { return m_isPaused; }
    void stop();

    Q_INVOKABLE void findElement(const QString& term);

    // Meta Data
    QObject* metaData() const { return metaDataReader->metaData(); }
    QStringList songs() const;
    int index() const;

public slots:
    void loadData() override;

signals:
    void projectsChanged();
    void currentProjectChanged();

    void isPausedChanged();
    void soundsChanged();
    void songsChanged();
    void metaDataChanged();
    void currentIndexChanged();
    void spotifyAuthorized();
    void musicVolumeChanged();
    void soundVolumeChanged();
    void isLoadingChanged();

private slots:
    void onProjectsChanged(QVector<AudioProject *> projects);
    void onCurrentScenarioChanged();
    void onStartedPlaying();
    void onMetaDataUpdated();
    void onSpotifyAuthorized() { emit spotifyAuthorized(); }

private:
    AudioEditor *editor = nullptr;
    QQmlApplicationEngine *qmlEngine = nullptr;
    MetaDataReader *metaDataReader = nullptr;
    MprisManager *mprisManager = nullptr;

    // Players
    DiscordPlayer *discordPlayer = nullptr;
    MusicPlayer *musicPlayer = nullptr;
    SoundPlayerController *soundPlayerController = nullptr;
    RadioPlayer *radioPlayer = nullptr;
    QList<AudioPlayer*> audioPlayers;

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

    // Project
    QVector<AudioProject*> m_projects;
    AudioProject *m_currentProject = nullptr;
};

#endif // AUDIOTOOL_H
