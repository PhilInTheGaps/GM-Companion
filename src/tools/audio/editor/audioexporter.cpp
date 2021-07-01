#include "audioexporter.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"
#include "logging.h"

#include <QFile>

/**
 * @brief Export all files in project that are marked as enabled
 */
void AudioExporter::setProject(AudioProject *project)
{
    m_project = project;
    m_project->setIsOpen(true);
    m_model = m_project;
    emit modelChanged();
}

void AudioExporter::exportFiles()
{
    if (m_path.isEmpty())
    {
        qCWarning(gmAudioExporter()) << "Error: No export path selected!";
        return;
    }

    Worker *worker = new Worker(m_path, m_project);

    worker->moveToThread(&workerThread);
    connect(worker, &Worker::copiedFiles,         [ = ]() { delete worker; });
    connect(worker, &Worker::progressChanged,     this,   &AudioExporter::updateProgress);
    connect(this,   &AudioExporter::startCopying, worker, &Worker::startCopying);
    workerThread.start();
    emit startCopying();
}

void AudioExporter::updateProgress(float progress)
{
    m_progress = progress;
    if (m_progress >= 1) m_progress = 0;
    emit progressChanged();
}

Worker::Worker(const QString &path, AudioProject *project)
    : m_path(path), m_project(project)
{
    connect(FileManager::getInstance(), &FileManager::receivedFile, this, &Worker::receivedFile);
    connect(FileManager::getInstance(), &FileManager::savedFile,    this, &Worker::savedFile);
}

void Worker::startCopying()
{
    if (!m_project)
    {
        emit progressChanged(1);
        qCWarning(gmAudioExporter()) << "Error: No project selected!";
    }

    qCDebug(gmAudioExporter()) << "Exporting files to:" << m_path << "...";

    // Find all files to export
    collectFilesToExport();

    // Copy all the files
    qCDebug(gmAudioExporter()) << "Copying files ...";
    m_fileCount = m_musicFiles.size() + m_soundFiles.size() + m_radioFiles.size();

    if (!copyNext())
    {
        qCWarning(gmAudioExporter()) << "Error: Could not copy any files.";
    }
}

/**
 * Find all files in the project that should be exported
 * and save them in queues.
 */
void Worker::collectFilesToExport()
{
    if (Q_UNLIKELY(!m_project)) return;

    // Categories
    for (auto *category : m_project->categories())
    {
        if (category && category->isChecked() > 0)
        {
            // Scenarios
            for (auto scenario : category->scenarios())
            {
                if (scenario && scenario->isChecked() > 0)
                {
                    copyElements(scenario);
                }
            }
        }
    }
}

auto Worker::copyNext() -> bool
{
    if (m_fileCount > 0)
    {
        float percent = static_cast<float>(m_exportCount++) / m_fileCount;
        emit progressChanged(percent);
        qCDebug(gmAudioExporter()) << "   Progress:" << m_exportCount << "/" << m_fileCount << "(" << percent * 100 << "%)";

        if (copyNextMusic()) return true;

        if (copyNextSound()) return true;

        if (copyNextRadio()) return true;

        qCDebug(gmAudioExporter()) << "No more files to export.";
    }

    emit progressChanged(1);
    emit copiedFiles();
    return false;
}

bool Worker::copyNextMusic()
{
    if (m_musicFiles.isEmpty()) return false;

    return copyFile(m_musicFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Music)), "music");
}

bool Worker::copyNextSound()
{
    if (m_soundFiles.isEmpty()) return false;

    return copyFile(m_soundFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Sound)), "sounds");
}

bool Worker::copyNextRadio()
{
    if (m_radioFiles.isEmpty()) return false;

    return copyFile(m_radioFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Radio)), "radios");
}

bool Worker::copyFile(const QString &filePath, const QString &base, const QString &subfolder)
{
    if (filePath.isEmpty() || subfolder.isEmpty()) return false;

    auto oldPath = QString("%1%2").arg(base, filePath);
    m_newFilePath = QString("%1/%2%3").arg(m_path, subfolder, filePath);

    m_currentRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFile(m_currentRequestId, oldPath);

    return true;
}

/**
 * @brief File has been received, now save it under new path
 */
void Worker::receivedFile(int requestId, const QByteArray &data)
{
    if (requestId != m_currentRequestId) return;

    m_currentRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->saveFile(m_currentRequestId, m_newFilePath, data);
}

/**
 * @brief File was saved, start next operation
 */
void Worker::savedFile(int requestId)
{
    if (requestId != m_currentRequestId) return;

    copyNext();
}

void Worker::copyElements(AudioScenario *scenario)
{
    if (Q_UNLIKELY(!scenario)) return;

    for (auto *element : scenario->elements(true))
    {
        if (element && element->isChecked() > 0)
        {
            for (auto *file : element->files())
            {
                // Only export local files
                if (file->source() != 0) continue;

                switch (element->type())
                {
                case AudioElement::Music:
                    if (!m_musicFiles.contains(file->url())) m_musicFiles.enqueue(file->url());
                    break;
                case AudioElement::Sound:
                    if (!m_soundFiles.contains(file->url())) m_soundFiles.enqueue(file->url());
                    break;
                case AudioElement::Radio:
                    if (!m_radioFiles.contains(file->url())) m_radioFiles.enqueue(file->url());
                    break;
                }
            }
        }
    }
}
