#ifndef AUDIOSAVELOAD_H
#define AUDIOSAVELOAD_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <QMap>
#include <QThread>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "src/tools/audio/audioproject.h"
#include "src/settings/settingsmanager.h"
#include "src/cloud/googledrive.h"
#include "audioicongenerator.h"

class LocalAudioFileSource;

class FileFinder : public QObject
{
    Q_OBJECT
public:
    FileFinder(QList<AudioFile*> files, QString basePath) :
    m_files(files), m_basePath(basePath) {}
    virtual ~FileFinder() {}

private:
    QList<AudioFile*> m_files;
    QString m_basePath;

public slots:
    void findMissingFiles();

signals:
    void finished();
};


class AudioSaveLoad : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    explicit AudioSaveLoad(int mode, GoogleDrive *google);
    ~AudioSaveLoad();

    void findProjects(int mode);
    void findEditorProjects(int mode);
    void fetchMusicPaths(QStringList paths);
    void fetchSoundPaths(QStringList paths, QStringList args);
    void fetchRadioPath(QString path);

    void findIconPaths(QList<AudioProject*> projects, bool editor);
    void findMissingFiles(QList<AudioFile *> files, QString basePath);

    void saveProject(AudioProject *project);

    void setMode(int mode) { m_mode = mode; }

    AudioProject *initProject(QString path);

private:
    SettingsManager sManager;
    AudioIconGenerator iconGenerator;
    GoogleDrive *googleDrive;
    LocalAudioFileSource *localSource;

    QList<AudioProject*> m_projects;
    QList<AudioProject*> m_editorProjects;

    // Icons
    void findIconPaths(QStringList icons, bool editor);
    QMap<QString, QString> m_iconMap;

    QUrl musicPath(QString path, int mode);

    int m_radioRequestID;
    QString m_relativeRadioPath;

    int m_projectsRequestID;
    QList<int> m_projectFileRequestIDs;

    int m_mode;

    // Initialize elements
    AudioCategory *initCategory(QJsonObject object);
    AudioScenario *initScenario(QJsonObject object);
    QList<MusicElement*> initMusicLists(QJsonArray array);
    QList<SoundElement*> initSoundLists(QJsonArray array);
    QList<RadioElement*> initRadios(QJsonArray array);
    void initAudioElement(AudioElement *element, QJsonObject object);

    // Save Project
    QJsonObject saveCategory(AudioCategory *category);
    QJsonObject saveScenario(AudioScenario *scenario);
    QJsonArray saveMusicElements(AudioScenario *scenario);
    QJsonArray saveSoundElements(AudioScenario *scenario);
    QJsonArray saveRadioElements(AudioScenario *scenario);
    QJsonObject saveAudioElement(AudioElement *element);

signals:
    void foundProjects(QList<AudioProject*> projects, bool forEditor);
    void songPathsChanged(QList<QUrl>);
    void soundPathsChanged(QList<QUrl>, QStringList);
    void radioPathChanged(QUrl url);
    void startFindingMissingFiles();
    void missingFilesFound(QList<bool> missingFiles);

private slots:
    void onFoundProjects(QList<AudioProject*> projects, bool forEditor);
    void onFoundIconPaths(QMap<QString, QString> iconMap, bool editor);
};

#endif // AUDIOSAVELOAD_H
