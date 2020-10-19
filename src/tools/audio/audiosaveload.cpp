#include "audiosaveload.h"
#include "logging.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"

#include <QSettings>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QImage>
#include <QBuffer>
#include <QPainter>

/**
 * @brief Find audio project files
 */
void AudioSaveLoad::findProjects(bool forEditor)
{
    auto requestId = FileManager::getUniqueRequestId();

    qCDebug(gmAudioSaveLoad()) << "Finding audio projects ... forEditor:" << forEditor << "request id:" << requestId;

    auto *context = new QObject(this);
    connect(FileManager::getInstance(), &FileManager::receivedFiles, context, [ = ](int id, const QList<QByteArray>&data) {
        if (id != requestId) return;

        qCDebug(gmAudioSaveLoad()) << "Found audio projects. forEditor:" << forEditor << "request id:" << requestId;

        QList<AudioProject *>projects;

        for (const auto& fileContent : data)
        {
            projects.append(new AudioProject(QJsonDocument::fromJson(fileContent).object()));
        }

        emit foundProjects(projects, forEditor);
        context->deleteLater();
    });

    FileManager::getInstance()->getFiles(requestId, SettingsManager::getPath("audio"), "*.audio");
}

void AudioSaveLoad::findMissingFiles(const QList<AudioFile *>&audioFiles, const QString& basePath)
{
    qCDebug(gmAudioSaveLoad()) << "Finding missing files ...";

    QStringList filePaths;

    for (auto audioFile : audioFiles)
    {
        if (!audioFile) continue;

        if (audioFile->source() != 0)
        {
            audioFile->setMissing(false);
        }
        else
        {
            filePaths.append(basePath + audioFile->url());
        }
    }

    auto requestId = FileManager::getUniqueRequestId();

    auto *context = new QObject(this);
    connect(FileManager::getInstance(), &FileManager::checkedIfFilesExist, context, [ = ](int id, const QStringList& found, const QStringList& notFound) {
        if (requestId != id) return;

        for (auto audioFile : audioFiles)
        {
            audioFile->setMissing(audioFile->source() == 0 && !found.contains(basePath + audioFile->url()));
        }

        if (notFound.length() > 0)
        {
            qCDebug(gmAudioSaveLoad()) << "Did not find" << notFound.length() << "files:" << notFound;
        }

        context->deleteLater();
    });

    FileManager::getInstance()->checkIfFilesExist(requestId, filePaths);
}

/**
 * @brief Save an AudioProject to disc
 * @param project Pointer to AudioProject to save
 */
void AudioSaveLoad::saveProject(AudioProject *project)
{
    if (!project) return;

    qCDebug(gmAudioSaveLoad()) << "Saving project:" << project->name() << "...";

    if (project->isSaved())
    {
        qCDebug(gmAudioSaveLoad()) << "    Project does not need to be saved, no changes were made.";
        return;
    }

    // Get json representation
    auto data = QJsonDocument(project->toJson()).toJson(QJsonDocument::Indented);

    auto filePath = SettingsManager::getPath("audio") + "/" + project->name() + ".audio";

    // If project was renamed, save under new filename and delete old
    if (project->wasRenamed())
    {
        project->setWasRenamed(false);
        auto filePathOld = SettingsManager::getPath("audio") + "/" + project->oldName() + ".audio";
        FileManager::getInstance()->renameFile(filePath, filePathOld, data);
    }

    // Save file
    else
    {
        FileManager::getInstance()->saveFile(filePath, data);
    }

    project->setIsSaved(true);
}

void AudioSaveLoad::deleteProject(AudioProject *project)
{
    FileManager::getInstance()->deleteFile(SettingsManager::getPath("audio") + "/" + project->name() + ".audio");
}
