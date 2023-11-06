#pragma once

#include <QFuture>
#include <QList>
#include <QUrl>

class AudioProject;
class AudioFile;

namespace Files
{
class FileListResult;
}

class AudioSaveLoad
{
public:
    AudioSaveLoad() = delete;

    static auto findProjectsAsync(const QObject *context, const QString &folder = QLatin1String())
        -> QFuture<std::vector<AudioProject *>>;

    static auto loadProject(const QByteArray &data, QObject *parent) -> AudioProject *;

    static auto findMissingFilesAsync(const QList<AudioFile *> &audioFiles, const QString &basePath) -> QFuture<bool>;

    static auto saveProject(AudioProject *project, const QString &folder = QLatin1String()) -> QFuture<bool>;

    static auto deleteProject(const AudioProject *project, const QString &folder = QLatin1String()) -> QFuture<bool>;

private:
    static auto loadProjects(const QObject *context, Files::FileListResult &&files)
        -> QFuture<std::vector<AudioProject *>>;
    static auto getFilePathsToCheck(const QList<AudioFile *> &audioFiles, const QString &basePath) -> QStringList;

    static auto saveProject(AudioProject *project, const QString &filePath, const QByteArray &data) -> QFuture<bool>;
    static auto saveRenamedProject(AudioProject *project, const QString &filePath, const QByteArray &data,
                                   const QString &folder) -> QFuture<bool>;

    static auto getProjectFolder(const QString &preferredFolder) -> QString;
};
