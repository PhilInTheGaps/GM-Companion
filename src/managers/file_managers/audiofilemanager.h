#ifndef AUDIOFILEMANAGER_H
#define AUDIOFILEMANAGER_H

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

class FileFinder : public QObject
{
    Q_OBJECT
public:
    FileFinder(QStringList files, QString basePath) :
    m_files(files), m_basePath(basePath) {}
    virtual ~FileFinder() {}

private:
    QStringList m_files;
    QString m_basePath;

public slots:
    void findMissingFiles();

signals:
    void finished(QList<bool> missingFiles);
};

class AudioFileManager : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    explicit AudioFileManager(GoogleDrive *google);
    ~AudioFileManager() {
        workerThread.quit();
        workerThread.wait();
    }

    void findProjects(int mode);
    void findEditorProjects(int mode);
    void fetchMusicPaths(QStringList paths);
    void fetchSoundPaths(QStringList paths, QStringList args);
    void fetchRadioPath(QString path);

    void findMissingFiles(QStringList files, QString basePath);

    void saveProject(AudioProject *project);

private:
    SettingsManager sManager;
    QList<AudioProject*> m_projects;
    QList<AudioProject*> m_editorProjects;
    GoogleDrive *googleDrive;

    AudioProject *initProject(QString path, int mode);
    AudioCategory *initCategory(QJsonObject object);
    AudioScenario *initScenario(QJsonObject object);

    QList<MusicElement*> initMusicLists(QJsonArray array);
    QList<SoundElement*> initSoundLists(QJsonArray array);
    QList<RadioElement*> initRadios(QJsonArray array);
    QList<SpotifyElement*> initSpotifyElements(QJsonArray array);

    QUrl musicPath(QString path, int mode);

    QString iconPath(QString icon);
    QMap<QString, QString> m_iconMap;
    void updateIconPaths();

    int m_radioRequestID;
    QString m_relativeRadioPath;

    int m_projectsRequestID;
    QList<int> m_projectFileRequestIDs;

    int m_mode;

    // Save Project
    QJsonObject saveCategory(AudioCategory *category);
    QJsonObject saveScenario(AudioScenario *scenario);
    QJsonArray saveMusicElements(AudioScenario *scenario);
    QJsonArray saveSoundElements(AudioScenario *scenario);
    QJsonArray saveRadioElements(AudioScenario *scenario);
    QJsonArray saveSpotifyElements(AudioScenario *scenario);

signals:
    void projectsChanged(QList<AudioProject*>);
    void editorProjectsChanged(QList<AudioProject*>);
    void songPathsChanged(QList<QUrl>);
    void soundPathsChanged(QList<QUrl>, QStringList);
    void radioPathChanged(QUrl url);
    void startFindingMissingFiles();
    void missingFilesFound(QList<bool> missingFiles);

private slots:
    void onProjectsChanged() { updateIconPaths(); }
    void onMusicPathsChanged(QList<QUrl>urls, int mode, QStringList args);
    void onSoundPathsChanged(QList<QUrl>urls, int mode, QStringList args);
    void onRadioPathChanged(QList<QUrl>urls, int mode, QStringList args);
};

#endif // AUDIOFILEMANAGER_H
