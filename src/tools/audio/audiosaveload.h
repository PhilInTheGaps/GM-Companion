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

using namespace Qt::Literals::StringLiterals;

class AudioSaveLoad
{
public:
    AudioSaveLoad() = delete;

    static auto findProjectsAsync(QObject *context, const QString &folder = u""_s)
        -> QFuture<std::vector<AudioProject *>>;

    static auto loadProject(const QByteArray &data, QObject *parent) -> AudioProject *;

    static auto findMissingFilesAsync(const QList<AudioFile *> &audioFiles, const QString &basePath)
        -> QFuture<bool>;

    static auto saveProject(AudioProject *project, const QString &folder = u""_s) -> QFuture<bool>;

    static auto deleteProject(AudioProject *project, const QString &folder = u""_s) -> QFuture<bool>;

private:
    static constexpr const auto PROJECT_FILE_SUFFIX = ".audio";

    static auto loadProjects(QObject *context, Files::FileListResult &&files) -> QFuture<std::vector<AudioProject *>>;
    static auto getFilePathsToCheck(const QList<AudioFile *> &audioFiles, const QString &basePath) -> QStringList;

    static auto saveProject(AudioProject *project, const QString &filePath, const QByteArray &data) -> QFuture<bool>;
    static auto saveRenamedProject(AudioProject *project, const QString &filePath, const QByteArray &data,
                                   const QString &folder) -> QFuture<bool>;

    static auto getProjectFolder(const QString &preferredFolder) -> QString;
};
