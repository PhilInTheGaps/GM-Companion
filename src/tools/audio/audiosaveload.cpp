#include "audiosaveload.h"
#include "filesystem/file.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "tools/audio/project/audiofile.h"
#include "tools/audio/project/audioproject.h"
#include "utils/fileutils.h"

#include <QJsonDocument>

using namespace Files;
using namespace AsyncFuture;

/**
 * @brief Find audio project files
 */
auto AudioSaveLoad::findProjectsAsync(const QString &folder) -> QFuture<QVector<AudioProject *>>
{
    qCDebug(gmAudioSaveLoad()) << "Finding audio projects ...";

    auto futureList = File::listAsync(getProjectFolder(folder), true, false);
    return observe(futureList).subscribe([](FileListResult *list) { return loadProjects(list); }).future();
}

auto AudioSaveLoad::loadProject(const QByteArray &data, QObject *parent) -> AudioProject *
{
    const auto doc = QJsonDocument::fromJson(data);
    return new AudioProject(doc.object(), parent);
}

auto AudioSaveLoad::loadProjects(FileListResult *files) -> QFuture<QVector<AudioProject *>>
{
    QStringList fileNames;
    const auto filePaths = files->filesFull();
    for (const auto &file : filePaths)
    {
        if (file.endsWith(PROJECT_FILE_SUFFIX))
        {
            fileNames << file;
        }
    }

    auto futureContents = File::getDataAsync(fileNames);
    files->deleteLater();

    return observe(futureContents)
        .subscribe([](const QVector<FileDataResult *> &contents) {
            qCDebug(gmAudioSaveLoad()) << "Found audio projects.";
            QVector<AudioProject *> projects;
            projects.reserve(contents.size());

            for (auto *content : contents)
            {
                projects << loadProject(content->data(), nullptr);
                content->deleteLater();
            }

            return projects;
        })
        .future();
}

auto AudioSaveLoad::findMissingFilesAsync(const QList<AudioFile *> &audioFiles, const QString &basePath)
    -> QFuture<bool>
{
    qCDebug(gmAudioSaveLoad()) << "Finding missing files ...";

    const auto filePaths = getFilePathsToCheck(audioFiles, basePath);

    auto futureCheckResult = File::checkAsync(filePaths, true);
    return observe(futureCheckResult)
        .subscribe(
            [audioFiles, basePath](FileMultiCheckResult *multiResult) {
                const auto foundPaths = multiResult->existing();

                for (auto *audioFile : audioFiles)
                {
                    audioFile->missing(audioFile->source() == AudioFile::Source::File &&
                                       !foundPaths.contains(FileUtils::fileInDir(audioFile->url(), basePath)));
                }

                if (const auto missingPaths = multiResult->missing(); !missingPaths.isEmpty())
                {
                    qCDebug(gmAudioSaveLoad()) << "Could not find" << missingPaths.length() << "files:" << missingPaths;
                }

                multiResult->deleteLater();
                return true;
            },
            []() { return false; })
        .future();
}

auto AudioSaveLoad::getFilePathsToCheck(const QList<AudioFile *> &audioFiles, const QString &basePath) -> QStringList
{
    QStringList filePaths;

    for (auto *audioFile : audioFiles)
    {
        if (!audioFile) continue;

        if (audioFile->source() != AudioFile::Source::File)
        {
            audioFile->missing(false);
        }
        else
        {
            filePaths << FileUtils::fileInDir(audioFile->url(), basePath);
        }
    }

    return filePaths;
}

auto AudioSaveLoad::saveProject(AudioProject *project, const QString &folder) -> QFuture<bool>
{
    if (!project)
    {
        qCWarning(gmAudioSaveLoad()) << "Could not save project: project is null!";
        return completed(false);
    }

    qCDebug(gmAudioSaveLoad()) << "Saving project:" << project->name() << "...";

    if (project->isSaved())
    {
        qCDebug(gmAudioSaveLoad()) << "Project does not need to be saved, no changes were made.";
        return completed(true);
    }

    auto data = QJsonDocument(project->toJson()).toJson(QJsonDocument::Indented);
    auto filePath = getProjectFolder(folder) + "/" + project->name() + PROJECT_FILE_SUFFIX;

    // If project was renamed, save under new filename and delete old
    if (project->wasRenamed())
    {
        return saveRenamedProject(project, filePath, data, folder);
    }

    // Save file
    return saveProject(project, filePath, data);
}

auto AudioSaveLoad::saveProject(AudioProject *project, const QString &filePath, const QByteArray &data) -> QFuture<bool>
{
    return observe(File::saveAsync(filePath, data))
        .subscribe(
            [project](FileResult *result) {
                project->isSaved(result->success());
                return true;
            },
            []() { return false; })
        .future();
}

auto AudioSaveLoad::saveRenamedProject(AudioProject *project, const QString &filePath, const QByteArray &data,
                                       const QString &folder) -> QFuture<bool>
{
    project->wasRenamed(false);

    auto filePathOld = getProjectFolder(folder) + "/" + project->oldName() + PROJECT_FILE_SUFFIX;
    auto future = File::moveAsync(filePathOld, filePath);

    return observe(future)
        .subscribe([filePath, data, project]() { return saveProject(project, filePath, data); }, []() { return false; })
        .future();
}

auto AudioSaveLoad::deleteProject(AudioProject *project, const QString &folder) -> QFuture<bool>
{
    if (!project)
    {
        return completed(false);
    }

    auto filePath = getProjectFolder(folder) + "/" + project->name() + PROJECT_FILE_SUFFIX;
    return observe(File::deleteAsync(filePath))
        .subscribe(
            [](FileResult *result) {
                const bool success = result->success();
                result->deleteLater();
                return success;
            },
            []() { return false; })
        .future();
}

auto AudioSaveLoad::getProjectFolder(const QString &preferredFolder) -> QString
{
    if (!preferredFolder.isEmpty())
    {
        return preferredFolder;
    }

    return SettingsManager::getPath(QStringLiteral("audio"));
}
