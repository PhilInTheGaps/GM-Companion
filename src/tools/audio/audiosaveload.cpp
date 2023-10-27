#include "audiosaveload.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "filesystem/results/filelistresult.h"
#include "filesystem/results/filemulticheckresult.h"
#include "settings/settingsmanager.h"
#include "tools/audio/project/audiofile.h"
#include "tools/audio/project/audioproject.h"
#include "tools/audio/project/audioprojectupgrader.h"
#include "utils/fileutils.h"
#include <QJsonDocument>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

Q_LOGGING_CATEGORY(gmAudioSaveLoad, "gm.audio.saveload")

/**
 * @brief Find audio project files
 */
auto AudioSaveLoad::findProjectsAsync(QObject *context, const QString &folder) -> QFuture<std::vector<AudioProject *>>
{
    qCDebug(gmAudioSaveLoad()) << "Finding audio projects ...";

    auto futureList = File::listAsync(getProjectFolder(folder), true, false);
    return futureList.then(context, [context](FileListResult &&list) { return loadProjects(context, std::move(list)); })
        .unwrap();
}

auto AudioSaveLoad::loadProject(const QByteArray &data, QObject *parent) -> AudioProject *
{
    AudioProjectUpgrader upgrader;
    upgrader.parse(data);
    const auto convertedData = upgrader.run();

    const auto doc = QJsonDocument::fromJson(convertedData.isEmpty() ? data : convertedData);
    return new AudioProject(doc.object(), parent);
}

auto AudioSaveLoad::loadProjects(QObject *context, FileListResult &&files) -> QFuture<std::vector<AudioProject *>>
{
    QStringList fileNames;
    const auto filePaths = files.filesFull();
    for (const auto &file : filePaths)
    {
        if (file.endsWith(PROJECT_FILE_SUFFIX))
        {
            fileNames << file;
        }
    }

    auto futureContents = File::getDataAsync(fileNames);

    return futureContents.then(context, [](const std::vector<FileDataResult> &contents) {
        qCDebug(gmAudioSaveLoad()) << "Found audio projects.";
        std::vector<AudioProject *> projects;
        projects.reserve(contents.size());

        for (const auto &content : contents)
        {
            projects.push_back(loadProject(content.data(), nullptr));
        }

        return projects;
    });
}

auto AudioSaveLoad::findMissingFilesAsync(QObject *context, const QList<AudioFile *> &audioFiles,
                                          const QString &basePath) -> QFuture<bool>
{
    qCDebug(gmAudioSaveLoad()) << "Finding missing files ...";

    const auto filePaths = getFilePathsToCheck(audioFiles, basePath);

    auto futureCheckResult = File::checkAsync(filePaths, true);
    return futureCheckResult
        .then(context,
              [audioFiles, basePath](FileMultiCheckResult &&multiResult) {
                  const auto foundPaths = multiResult.existing();

                  for (auto *audioFile : audioFiles)
                  {
                      audioFile->missing(audioFile->source() == AudioFile::Source::File &&
                                         !foundPaths.contains(FileUtils::fileInDir(audioFile->url(), basePath)));
                  }

                  if (const auto missingPaths = multiResult.missing(); !missingPaths.isEmpty())
                  {
                      qCDebug(gmAudioSaveLoad())
                          << "Could not find" << missingPaths.length() << "files:" << missingPaths;
                  }

                  return true;
              })
        .onCanceled(context, []() { return false; });
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
        return QtFuture::makeReadyFuture(false);
    }

    qCDebug(gmAudioSaveLoad()) << "Saving project:" << project->name() << "...";

    if (project->isSaved())
    {
        qCDebug(gmAudioSaveLoad()) << "Project does not need to be saved, no changes were made.";
        return QtFuture::makeReadyFuture(true);
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
    return File::saveAsync(filePath, data)
        .then(project,
              [project](FileResult &&result) {
                  project->isSaved(result.success());
                  return true;
              })
        .onCanceled(project, []() { return false; });
}

auto AudioSaveLoad::saveRenamedProject(AudioProject *project, const QString &filePath, const QByteArray &data,
                                       const QString &folder) -> QFuture<bool>
{
    auto filePathOld = getProjectFolder(folder) + "/" + project->originalName() + PROJECT_FILE_SUFFIX;
    auto future = File::moveAsync(filePathOld, filePath);

    return future
        .then(project,
              [filePath, data, project](FileResult &&result) {
                  if (!result.success()) return QtFuture::makeReadyFuture(false);

                  project->wasRenamed(false);
                  return saveProject(project, filePath, data);
              })
        .onCanceled(project, []() { return QtFuture::makeReadyFuture(false); })
        .unwrap();
}

auto AudioSaveLoad::deleteProject(AudioProject *project, const QString &folder) -> QFuture<bool>
{
    if (!project)
    {
        return QtFuture::makeReadyFuture(false);
    }

    auto filePath = getProjectFolder(folder) + "/" + project->name() + PROJECT_FILE_SUFFIX;
    return File::deleteAsync(filePath)
        .then(project, [](const FileResult &result) { return result.success(); })
        .onCanceled(project, []() { return false; });
}

auto AudioSaveLoad::getProjectFolder(const QString &preferredFolder) -> QString
{
    if (!preferredFolder.isEmpty())
    {
        return preferredFolder;
    }

    return SettingsManager::getPath(u"audio"_s);
}
