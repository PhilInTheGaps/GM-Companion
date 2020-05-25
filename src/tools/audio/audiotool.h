#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QStringList>
#include <QQmlApplicationEngine>

#include "tools/abstracttool.h"
#include "audioeditor.h"
#include "players/spotifyplayer.h"
#include "players/musicplayer.h"
#include "players/soundplayer.h"
#include "players/radioplayer.h"
#include "metadatareader.h"

#include "mpris/mprisadaptor.h"
#include "mpris/mprisplayeradaptor.h"

#define DEFAULT_MUSIC_VOLUME 0.25
#define DEFAULT_SOUND_VOLUME 0.25

class AudioTool : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(QStringList projectNames READ projectNames NOTIFY projectsChanged)
    Q_PROPERTY(QStringList categoryNames READ categoryNames NOTIFY currentProjectChanged)
    Q_PROPERTY(QStringList scenarioNames READ scenarioNames NOTIFY currentCategoryChanged)

    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(qreal musicVolume READ musicVolume NOTIFY musicVolumeChanged)
    Q_PROPERTY(qreal soundVolume READ soundVolume NOTIFY soundVolumeChanged)

    Q_PROPERTY(QString type READ type NOTIFY metaDataChanged)
    Q_PROPERTY(QString title READ title NOTIFY metaDataChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY metaDataChanged)
    Q_PROPERTY(QString album READ album NOTIFY metaDataChanged)
    Q_PROPERTY(QString cover READ cover NOTIFY metaDataChanged)
    Q_PROPERTY(QStringList songs READ songs NOTIFY songsChanged)
    Q_PROPERTY(int index READ index NOTIFY currentIndexChanged)

public:
    explicit AudioTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~AudioTool();

    AudioEditor* getEditor() const { return editor; }

    // Project
    void updateProjectList() { audioSaveLoad.findProjects(false); }
    QStringList projectNames();
    QString currentProjectName() const { if (m_currentProject) return m_currentProject->name(); else return nullptr; }
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE int getCurrentProjectIndex();

    // Categories
    QStringList categoryNames() const { if (m_currentProject) return m_currentProject->categoryNames(); else return {}; }
    QString currentCategoryName() const { if (m_currentProject) return m_currentProject->currentCategory()->name(); else return ""; }
    Q_INVOKABLE void setCurrentCategory(QString category);

    // Scenarios
    QStringList scenarioNames() const { if (m_currentProject && m_currentProject->currentCategory()) return m_currentProject->currentCategory()->scenarioNames(); else return {}; }
    QString currentScenarioName() const { if (m_currentProject && m_currentProject->currentCategory()) return m_currentProject->currentCategory()->currentScenario()->name(); else return ""; }
    Q_INVOKABLE void setCurrentScenario(QString scenario);

    // Elements
    QList<QList<AudioElement*>> elements() const;
    Q_INVOKABLE void playElement(QString name, int type, QString subscenario);

    qreal musicVolume() const { return m_musicVolume; }
    Q_INVOKABLE void setMusicVolume(qreal volume);

    qreal soundVolume() const { return m_soundVolume; }
    Q_INVOKABLE void setSoundVolume(qreal volume);

    Q_INVOKABLE void next();
    Q_INVOKABLE void playPause();
    Q_INVOKABLE void again();
    Q_INVOKABLE void setMusicIndex(int index);
    Q_INVOKABLE void stopSound(QString sound) { soundPlayer->stop(sound); }
    bool isPaused() const { return m_isPaused; }

    Q_INVOKABLE void findElement(QString element);

    // Meta Data
    QString type() const { return m_metaData.type; }
    QString title() const { return m_metaData.title; }
    QString artist() const { return m_metaData.artist; }
    QString album() const { return m_metaData.album; }
    QString cover() const { return m_metaData.cover; }
    QStringList songs() const;
    int index() const;

public slots:
    void loadData() override;

signals:
    void projectsChanged();
    void currentProjectChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();
    void isPausedChanged();
    void soundsChanged();
    void songsChanged();
    void metaDataChanged();
    void currentIndexChanged();
    void spotifyAuthorized();
    void musicVolumeChanged();
    void soundVolumeChanged();

private slots:
    void onProjectsChanged(QList<AudioProject*> projects, bool forEditor);
    void onCurrentScenarioChanged();
    void onStartedPlaying();
    void onMetaDataUpdated(MetaData metaData);
    void onSoundsChanged(QList<AudioElement*> elements);
    void onSpotifyAuthorized() { emit spotifyAuthorized(); }

private:
    AudioEditor *editor = nullptr;
    QQmlApplicationEngine *qmlEngine = nullptr;
    MetaDataReader *metaDataReader = nullptr;
    AudioSaveLoad audioSaveLoad;

    // MPRIS
    MprisAdaptor *mprisAdaptor = nullptr;
    MprisPlayerAdaptor *mprisPlayerAdaptor = nullptr;
    void sendMprisUpdateSignal(QString property, QVariant value);

    // Players
    DiscordPlayer *discordPlayer = nullptr;
    MusicPlayer *musicPlayer = nullptr;
    SoundPlayerController *soundPlayer = nullptr;
    RadioPlayer *radioPlayer = nullptr;
    SpotifyPlayer *spotifyPlayer = nullptr;
    QList<AudioPlayer*> audioPlayers;

    AudioElementModelModel *elementModel = nullptr;
    AudioElementModel *soundModel = nullptr;

    int m_musicMode;
    bool m_isPaused = true;
    MetaData m_metaData;
    qreal m_musicVolume = DEFAULT_MUSIC_VOLUME;
    qreal m_soundVolume = DEFAULT_SOUND_VOLUME;

    // Project
    QList<AudioProject*> m_projects;
    AudioProject *m_currentProject = nullptr;
};

#endif // AUDIOTOOL_H
