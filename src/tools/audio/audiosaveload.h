#ifndef AUDIOSAVELOAD_H
#define AUDIOSAVELOAD_H

#include <QList>
#include <QUrl>
#include <QFuture>

class AudioProject;
class AudioFile;

namespace Files {
class FileListResult;
}

class AudioSaveLoad
{
public:
    AudioSaveLoad() = delete;

    static QFuture<QVector<AudioProject*>> findProjectsAsync(const QString &folder = "");

    static QFuture<bool> findMissingFilesAsync(const QList<AudioFile *>& audioFiles, const QString& basePath);

    static QFuture<bool> saveProject(AudioProject *project, const QString &folder = "");

    static QFuture<bool> deleteProject(AudioProject *project, const QString &folder = "");

private:
    static constexpr const char* PROJECT_FILE_SUFFIX = ".audio";

    static QFuture<QVector<AudioProject*>> loadProjects(Files::FileListResult *files);
    static QStringList getFilePathsToCheck(const QList<AudioFile *>& audioFiles, const QString& basePath);

    static QFuture<bool> saveProject(AudioProject *project, const QString &filePath, const QByteArray &data);
    static QFuture<bool> saveRenamedProject(AudioProject *project, const QString &filePath,
                                            const QByteArray &data, const QString &folder);

    static QString getProjectFolder(const QString &preferredFolder);
};

#endif // AUDIOSAVELOAD_H
