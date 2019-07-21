#include "audioexporter.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

AudioExporter::AudioExporter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Exporter ...";
}

/**
 * @brief Find all categories
 */
void AudioExporter::updateCategories()
{
    qDebug() << "AudioExporter: Updating Categories ...";

    m_categoryNames.clear();
    emit categoriesChanged(); // To clear the qml list

    if (!m_project) return;

    m_categoryNames = m_project->categoryNames();

    if (m_project->categories().size() > 0)
    {
        m_category = m_project->categories()[0];
    }
    else
    {
        m_category = nullptr;
    }

    updateScenarios();
    emit categoriesChanged();
}

/**
 * @brief Find all scenarios in current category
 */
void AudioExporter::updateScenarios()
{
    qDebug() << "AudioExporter: Finding Scenarios ...";

    m_scenarioNames.clear();

    if (m_category && m_category->isExport())
    {
        m_scenarioNames = m_category->scenarioNames();

        if (m_category->scenarios().size() > 0)
        {
            m_scenario = m_category->scenarios()[0];
        }
        else
        {
            m_scenario = nullptr;
        }
    }
    else
    {
        m_scenario = nullptr;
    }

    updateElements();
    emit scenariosChanged();
}

/**
 * @brief Find all elements in current scenario
 */
void AudioExporter::updateElements()
{
    m_elementNames.clear();

    if (m_scenario && m_scenario->isExport())
    {
        m_elementNames += m_scenario->musicElementNames();
        m_elementNames += m_scenario->soundElementNames();
        m_elementNames += m_scenario->radioElementNames();
    }

    emit elementsChanged();
}

/**
 * @brief The the current category
 * @param index The index of the category
 */
void AudioExporter::setCategory(int index)
{
    if (m_project && (index < m_project->categories().size()))
    {
        m_category = m_project->categories()[index];
    }
    else
    {
        m_category = nullptr;
        qWarning() << "AudioExporter: Could not set category, index out of range.";
    }

    updateScenarios();
}

/**
 * @brief Set if a category is enabled for exporting
 * @param index Index of the category
 * @param enabled True if a category should be exported
 */
void AudioExporter::setCategoryEnabled(int index, bool enabled)
{
    if (m_project && (index < m_project->categories().size()))
    {
        m_project->categories()[index]->setExport(enabled);

        if (m_project->categories()[index] == m_category) updateScenarios();
    }
}

/**
 * @brief Get if a category is enabled for export
 * @param index Index of the category
 * @return True if a category will be exported
 */
bool AudioExporter::isCategoryEnabled(int index) const
{
    if (m_project && (index < m_project->categories().size()))
    {
        return m_project->categories()[index]->isExport();
    }
    else
    {
        return false;
    }
}

/**
 * @brief Set the current scenario
 * @param index Index of the scenario
 */
void AudioExporter::setScenario(int index)
{
    if (m_project && m_category && (index < m_category->scenarios().size()))
    {
        m_scenario = m_category->scenarios()[index];
    }
    else
    {
        m_scenario = nullptr;
        qWarning() << "AudioExporter: Could not set scenario, index out of range.";
    }

    updateElements();
}

/**
 * @brief Set if a scenario will be exported
 * @param index Index of the scenario
 * @param enabled True if scenario will be exported
 */
void AudioExporter::setScenarioEnabled(int index, bool enabled)
{
    if (m_project && m_category && (index < m_category->scenarios().size()))
    {
        m_category->scenarios()[index]->setExport(enabled);

        if (m_category->scenarios()[index] == m_scenario) updateElements();
    }
}

/**
 * @brief Get if a scenario will be exported
 * @param index Index of the scenario
 * @return True if scenario will be exported
 */
bool AudioExporter::isScenarioEnabled(int index) const
{
    if (m_project && m_category && (index < m_category->scenarios().size()))
    {
        return m_category->scenarios()[index]->isExport();
    }
    else
    {
        return false;
    }
}

/**
 * @brief Set if an element will be exported
 * @param index Index of the element
 * @param enabled True if element will be exported
 */
void AudioExporter::setElementEnabled(int index, bool enabled)
{
    if (m_project && m_category && m_scenario && (index < m_scenario->elements().size()))
    {
        m_scenario->elements()[index]->setExport(enabled);
    }
}

/**
 * @brief Get if an element will be exported
 * @param index Index of the element
 * @return True if element will be exported
 */
bool AudioExporter::isElementEnabled(int index) const
{
    if (m_project && m_category && m_scenario && (index < m_scenario->elements().size()))
    {
        return m_scenario->elements()[index]->isExport();
    }
    else
    {
        return false;
    }
}

/**
 * @brief Export all files in project that are marked as enabled
 */
void AudioExporter::exportFiles()
{
    QString path   = m_path == "" ? getDefaultPath() : m_path;
    Worker *worker = new Worker(path, m_project);

    worker->moveToThread(&workerThread);
    connect(worker, &Worker::copiedFiles,         [ = ]() { delete worker; });
    connect(worker, &Worker::progressChanged,     this,   &AudioExporter::updateProgress);
    connect(this,   &AudioExporter::startCopying, worker, &Worker::copyFiles);
    workerThread.start();
    emit startCopying();
}

/**
 * @brief Copy all files in project to their new locations
 */
void Worker::copyFiles()
{
    if (!m_project)
    {
        emit progressChanged(1);
        qDebug() << "AudioExporter: No project selected!";
    }

    // Create path if it does not exist
    if (!QDir(m_path).exists())
    {
        QDir d = QDir(m_path);
        d.mkpath(m_path);
    }

    qDebug() << "AudioExporter: Exporting files to:" << m_path << "...";

    // Find all files to export
    QStringList musicFiles;
    QStringList soundFiles;
    QStringList radioFiles;

    // Categories
    for (auto c : m_project->categories())
    {
        if (c && c->isExport())
        {
            // Scenarios
            for (auto s : c->scenarios())
            {
                if (s && s->isExport())
                {
                    // Music Elements
                    for (auto e : s->musicElements())
                    {
                        if (e && e->isExport())
                        {
                            for (auto f : e->files())
                            {
                                if (!musicFiles.contains(f)) musicFiles.append(f);
                            }
                        }
                    }

                    // Sound Elements
                    for (auto e : s->soundElements())
                    {
                        if (e && e->isExport())
                        {
                            for (auto f : e->files())
                            {
                                if (!soundFiles.contains(f)) soundFiles.append(f);
                            }
                        }
                    }

                    // Radio Elements
                    for (auto e : s->radioElements())
                    {
                        if (e && e->isExport() && e->local())
                        {
                            if (!radioFiles.contains(e->url().toString())) radioFiles.append(e->url().toString());
                        }
                    }
                }
            }
        }
    }

    // Copy all the files
    qDebug() << "Copying ...";
    int fileCount = musicFiles.size() + soundFiles.size() + radioFiles.size();

    for (int i = 0; i < fileCount; i++)
    {
        qDebug() << "   Progress:" << i + 1 << "/" << fileCount;
        emit progressChanged(static_cast<float>(i + 1) / fileCount);

        QString file;
        QString base;
        QString expBase;

        if (i < musicFiles.size())
        {
            file    = musicFiles[i];
            base    = sManager.getSetting(Setting::musicPath);
            expBase = m_path + "/music";
        }
        else if (i < musicFiles.size() + soundFiles.size())
        {
            file    = soundFiles[i - musicFiles.size()];
            base    = sManager.getSetting(Setting::soundPath);
            expBase = m_path + "/sounds";
        }
        else
        {
            file    = radioFiles[i - musicFiles.size() - soundFiles.size()];
            base    = sManager.getSetting(Setting::radioPath);
            expBase = m_path + "/radio";
        }

        QFile   f(base + file);
        QString folder = file.left(file.lastIndexOf('/'));

        if (f.exists())
        {
            if (!QDir(expBase + folder).exists())
            {
                QDir d;
                d.mkpath(expBase + folder);
            }

            qDebug() << "       " << base + file;
            qDebug() << "       " << expBase + file;

            if (!QFile(expBase + file).exists())
            {
                f.copy(expBase + file);
            }
            else
            {
                qDebug() << "File already exists!";
            }
        }
        else
        {
            qDebug() << "File" << f.fileName() << "not found!";
        }
    }

    emit progressChanged(1);
    qDebug() << "Done ...";
}
