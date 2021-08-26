#include "audioexporter.h"
#include "settings/settingsmanager.h"
#include "filesystem_new/file.h"
#include "logging.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

using namespace AsyncFuture;

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

    auto *worker = new Worker(m_path, m_project);

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
            for (auto *scenario : category->scenarios())
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

auto Worker::copyNextMusic() -> bool
{
    if (m_musicFiles.isEmpty()) return false;

    return copyFile(m_musicFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Music)), "music");
}

auto Worker::copyNextSound() -> bool
{
    if (m_soundFiles.isEmpty()) return false;

    return copyFile(m_soundFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Sound)), "sounds");
}

auto Worker::copyNextRadio() -> bool
{
    if (m_radioFiles.isEmpty()) return false;

    return copyFile(m_radioFiles.dequeue(), SettingsManager::getPath(AudioElement::typeToSettings(AudioElement::Radio)), "radios");
}

auto Worker::copyFile(const QString &filePath, const QString &base, const QString &subfolder) -> bool
{
    if (filePath.isEmpty() || subfolder.isEmpty()) return false;

    const auto oldPath = FileUtils::fileInDir(filePath, base);
    const auto newPath = FileUtils::fileInDir(FileUtils::fileInDir(filePath, subfolder), m_path);

    observe(Files::File::copyAsync(oldPath, newPath)).subscribe([this]() {
        copyNext();
    }, [this]() {
        copyNext();
    });
    return true;
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
