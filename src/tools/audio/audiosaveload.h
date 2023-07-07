#ifndef AUDIOSAVELOAD_H
#define AUDIOSAVELOAD_H

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

    static QFuture<std::vector<AudioProject *>> findProjectsAsync(QObject *context, const QString &folder = u""_s);

    static AudioProject *loadProject(const QByteArray &data, QObject *parent);

    static QFuture<bool> findMissingFilesAsync(QObject *context, const QList<AudioFile *> &audioFiles,
                                               const QString &basePath);

    static QFuture<bool> saveProject(AudioProject *project, const QString &folder = u""_s);

    static QFuture<bool> deleteProject(AudioProject *project, const QString &folder = u""_s);

private:
    static constexpr const auto PROJECT_FILE_SUFFIX = ".audio";

    static QFuture<std::vector<AudioProject *>> loadProjects(QObject *context, Files::FileListResult *files);
    static QStringList getFilePathsToCheck(const QList<AudioFile *> &audioFiles, const QString &basePath);

    static QFuture<bool> saveProject(AudioProject *project, const QString &filePath, const QByteArray &data);
    static QFuture<bool> saveRenamedProject(AudioProject *project, const QString &filePath, const QByteArray &data,
                                            const QString &folder);

    static QString getProjectFolder(const QString &preferredFolder);
};

#endif // AUDIOSAVELOAD_H
