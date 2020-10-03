#ifndef AUDIOSAVELOAD_H
#define AUDIOSAVELOAD_H

#include <QObject>
#include <QList>
#include <QUrl>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "tools/audio/project/audioproject.h"

class AudioSaveLoad : public QObject
{
    Q_OBJECT

public:
    void findProjects(bool forEditor = false);

    void findIconPaths(QList<AudioProject*> projects, bool forEditor);
    void findMissingFiles(QList<AudioFile *> files, QString basePath);

    void saveProject(AudioProject *project);
    void deleteProject(AudioProject *project);

private:
    int m_radioRequestID;
    QString m_relativeRadioPath;

    int m_projectsRequestID;
    QList<int> m_projectFileRequestIDs;

signals:
    void foundProjects(QList<AudioProject*> projects, bool forEditor);
    void songPathsChanged(QList<QUrl>);
    void soundPathsChanged(QList<QUrl>, QStringList);
    void radioPathChanged(QUrl url);
    void startFindingMissingFiles();
    void missingFilesFound(QList<bool> missingFiles);
};

#endif // AUDIOSAVELOAD_H
