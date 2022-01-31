#include "audioeditor.h"
#include "logging.h"
#include "../thumbnails/audiothumbnailgenerator.h"
#include "utils/utils.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QQmlContext>
#include <utility>

using namespace AsyncFuture;

AudioEditor::AudioEditor(QQmlApplicationEngine *engine, QNetworkAccessManager *networkManager, QObject *parent) :
    AbstractTool(parent), a_isSaved(true), qmlEngine(engine), networkManager(networkManager)
{
    qCDebug(gmAudioEditor) << "Loading Audio Editor ...";

    addonElementManager = new AddonElementManager(this);
    audioExporter       = new AudioExporter(this);
    fileBrowser         = new AudioEditorFileBrowser(engine, this);
    unsplashParser      = new UnsplashParser(engine, this);
    ytClient            = new YouTube::Videos::VideoClient(networkManager, this);

    engine->rootContext()->setContextProperty("audio_exporter", audioExporter);
    engine->rootContext()->setContextProperty("audio_addon_element_manager", addonElementManager);
    engine->rootContext()->setContextProperty("audio_editor_file_browser", fileBrowser);

    fileModel = new AudioFileModel(this);
    qmlEngine->rootContext()->setContextProperty("audio_editor_file_model", fileModel);

    connect(this,                &AudioEditor::currentScenarioChanged, this, &AudioEditor::onCurrentScenarioChanged);
//    connect(addonElementManager, &AddonElementManager::exportElements, this, &AudioEditor::onExportElements);
}

void AudioEditor::loadData()
{
    if (m_isDataLoaded) return;

    setIsLoading(true);
    m_isDataLoaded = true;

    observe(AudioSaveLoad::findProjectsAsync()).subscribe([this](QVector<AudioProject*> projects) {
        onFoundProjects(projects);
    });
}

void AudioEditor::onFoundProjects(QVector<AudioProject *>list)
{
    qCDebug(gmAudioEditor) << "Projects changed!";
    m_projects = std::move(list);

    for (auto *project : m_projects)
    {
        if (project)
        {
            project->setParent(this);
            connect(project, &AudioProject::isSavedChanged, this, &AudioEditor::onProjectSavedChanged);
        }
    }

    emit projectsChanged();
    setCurrentProject(0);

    setIsLoading(false);
}

/**
 * @brief Set the current project
 * @param index Index of project in project list
 */
void AudioEditor::setCurrentProject(int index)
{
    qCDebug(gmAudioEditor) << "Change project:" << index;

    if (Utils::isInBounds(m_projects, index))
    {
        setCurrentProject(m_projects[index]);
    }
    else
    {
        setCurrentProject(nullptr);
    }
}

/**
 * @brief Set the current project
 * @param project New active project
 */
void AudioEditor::setCurrentProject(AudioProject *project)
{
    if (m_currentProject == project) return;

    m_currentProject = project;
    audioExporter->setProject(project);
    emit currentProjectChanged();
    emit currentCategoryChanged();
    emit currentScenarioChanged();

    loadFirstElement();
}

/**
 * @brief Get the index of the active project
 * @return Index of active project in project list
 */
auto AudioEditor::projectIndex() const -> int
{
    if (!m_currentProject || m_projects.isEmpty()) return -1;

    return m_projects.indexOf(m_currentProject);
}

/**
 * @brief Create a new project
 * @param name Name of the new project
 */
void AudioEditor::createProject(const QString& name)
{
    if (name.isEmpty()) return;

    qCDebug(gmAudioEditor) << "Creating project" << name << "...";

    auto *project = new AudioProject(name, 4, {}, this);
    project->isSaved(false);
    m_projects.append(project);
    emit projectsChanged();

    connect(project, &AudioProject::isSavedChanged, this, &AudioEditor::onProjectSavedChanged);

    setCurrentProject(project);
    madeChanges();
}

/**
 * @brief Rename the current project
 * @param name New name
 */
void AudioEditor::renameProject(const QString& name)
{
    if (!m_currentProject || name.isEmpty()) return;

    qCDebug(gmAudioEditor) << "Renaming project" << m_currentProject->name() << "to" << name << "...";

    if (!m_currentProject->wasRenamed())
    {
        m_currentProject->wasRenamed(true);
        m_currentProject->oldName(m_currentProject->name());
    }

    m_currentProject->setName(name);
    emit projectsChanged();
}

/**
 * @brief Delete the current project
 */
void AudioEditor::deleteProject()
{
    if (!m_currentProject) return;

    qCDebug(gmAudioEditor) << "Deleting current project:" << m_currentProject->name() << "...";

    // If the current element is a child of the project unload it first.
    if (m_currentProject && m_currentProject->currentScenario()->getScenario(m_currentElement))
    {
        clearCurrentElement();
    }

    auto project = m_currentProject;
    m_projects.removeOne(project);
    setCurrentProject(0);

    AudioSaveLoad::deleteProject(project);
    emit projectsChanged();
}

/**
 * @brief Set the current category
 * @param name Name of new active category
 */
void AudioEditor::setCurrentCategory(int index)
{
    if (!Utils::isInBounds(m_currentProject->categories(), index))
    {
        setCurrentCategory(nullptr);
    }
    else
    {
        setCurrentCategory(m_currentProject->categories()[index]);
    }
}

/**
 * @brief Set category as the current category
 */
void AudioEditor::setCurrentCategory(AudioCategory *category)
{
    if (!m_currentProject || category == m_currentProject->currentCategory()) return;

    if (category)
    {
        qCDebug(gmAudioEditor) << "Setting current category" << category->name() << "(" << m_currentProject->name() << ") ...";
    }

    if (!m_currentProject->setCurrentCategory(category) && category)
    {
        qCWarning(gmAudioEditor()) << "Error: Could not switch to category" << category->name();
    }

    emit currentCategoryChanged();
    emit currentScenarioChanged();
    emit currentElementChanged();

    loadFirstElement();
}

/**
 * @brief Create a new category
 * @param name Name of the new category
 */
void AudioEditor::createCategory(const QString& name)
{
    if (!m_currentProject || name.isEmpty()) return;

    qCDebug(gmAudioEditor) << "Creating category" << name << "...";

    if (!m_currentProject->categoryNames().contains(name))
    {
        auto *category = new AudioCategory(name, m_currentProject->name(), {}, m_currentProject);

        if (!m_currentProject->addCategory(category, true))
        {
            qCWarning(gmAudioEditor()) << "Error: Could not create category" << category->name();
        }
    }
}

/**
 * @brief Rename the current category
 * @param name New name
 */
void AudioEditor::renameCategory(const QString& name)
{
    if (!categoryExists() || name.isEmpty()) return;

    auto *category = m_currentProject->currentCategory();
    if (!category) return;

    qCDebug(gmAudioEditor) << "Renaming category" << category->name() << "to" << name << "...";

    category->setName(name);
    emit currentProjectChanged();
}

/**
 * @brief Delete the current category from the project.
 */
void AudioEditor::deleteCategory()
{
    if (!categoryExists()) return;

    auto *category = m_currentProject->currentCategory();
    auto *scenario = category->currentScenario();

    qCDebug(gmAudioEditor) << "Deleting current category:" << category->name() << "...";

    // If the current element is a child of the current scenario or a subscenario, unload it first.
    if (scenario && scenario->getScenario(m_currentElement))
    {
        clearCurrentElement();
    }

    if (!m_currentProject->deleteCategory(category))
    {
        qCWarning(gmAudioEditor()) << "Error: Could not delete category" << category->name();
    }

    auto index = m_currentProject->categories().isEmpty() ? -1 : 0;
    setCurrentCategory(index);
}

/**
 * @brief Set the current scenario
 * @param name Name of the scenario
 */
void AudioEditor::setCurrentScenario(int index)
{
    if (!categoryExists()) return;

    auto *category = m_currentProject->currentCategory();

    if (!Utils::isInBounds(category->scenarios(), index))
    {
        setCurrentScenario(nullptr);
    }
    else
    {
        setCurrentScenario(category->scenarios()[index]);
    }   
}

/**
 * @brief Set scenario as the current scenario
 */
void AudioEditor::setCurrentScenario(AudioScenario *scenario)
{
    if (!categoryExists() || scenario == m_currentProject->currentScenario()) return;

    auto *category = m_currentProject->currentCategory();

    if (scenario)
    {
        qCDebug(gmAudioEditor) << "Setting current scenario" << scenario->name() << "...";
    }

    if (!category->setCurrentScenario(scenario) && scenario)
    {
        qCWarning(gmAudioEditor()) << "Error: Could not switch to scenario" << scenario->name();
    }

    emit currentScenarioChanged();

    loadFirstElement();
}

/**
 * @brief When the current scenario changes, start generating icons
 */
void AudioEditor::onCurrentScenarioChanged()
{
    AudioThumbnailGenerator::generateThumbnails(m_currentProject->currentScenario());
}

/**
 * @brief When a project emits "isSavedChanged()"
 */
void AudioEditor::onProjectSavedChanged()
{
    auto *project = qobject_cast<AudioProject*>(sender());
    if (project && !project->isSaved())
    {
        madeChanges();
    }
}

/**
 * @brief Create a new scenario
 * @param name Name of the new scenario
 */
void AudioEditor::createScenario(const QString& name, bool isSubscenario)
{
    if (!categoryExists() || name.isEmpty()) return;

    qCDebug(gmAudioEditor) << "Creating scenario" << name << "...";

    auto *category = m_currentProject->currentCategory();

    if (isSubscenario)
    {
        if (!category->currentScenario()->scenarioNames().contains(name))
        {
            auto *scenario = new AudioScenario(name, category->currentScenario()->path(), {}, {}, {}, {}, category->currentScenario());

            if (!category->currentScenario()->addScenario(scenario))
            {
                qCWarning(gmAudioEditor()) << "Error: Could not create subscenario" << name;
            }
        }
    }
    else if (!category->scenarioNames().contains(name))
    {
        auto *scenario = new AudioScenario(name, category->path(), {}, {}, {}, {}, category);

        if (!category->addScenario(scenario, true))
        {
            qCWarning(gmAudioEditor()) << "Error: Could not create scenario" << name;
        }
    }
}

/**
 * @brief Rename the current scenario
 * @param name New name
 */
void AudioEditor::renameScenario(const QString& name)
{
    if (!scenarioExists() || name.isEmpty()) return;

    auto *scenario = m_currentProject->currentScenario();

    qCDebug(gmAudioEditor) << "Renaming scenario" << scenario->name() << "to" << name << "...";

    scenario->setName(name);
}

/**
 * @brief Delete the current scenario.
 */
void AudioEditor::deleteScenario()
{
    if (!scenarioExists()) return;

    auto *scenario = m_currentProject->currentScenario();

    qCDebug(gmAudioEditor) << "Deleting current scenario:" << scenario->name() << "...";

    // If the current element is a child of the scenario or a subscenario, unload it first.
    if (scenario->getScenario(m_currentElement))
    {
        clearCurrentElement();
    }

    if (!m_currentProject->currentCategory()->deleteScenario(scenario))
    {
        qCWarning(gmAudioEditor()) << "Error: Could not delete scenario" << scenario->name();
    }

    auto index = m_currentProject->currentCategory()->scenarios().isEmpty() ? -1 : 0;
    setCurrentScenario(index);
}

/**
 * @brief Delete a subscenario
 */
void AudioEditor::deleteSubScenario(AudioScenario *subscenario)
{
    if (!subscenario || !scenarioExists()) return;

    auto *scenario = m_currentProject->currentScenario();

    qCDebug(gmAudioEditor) << "Deleting subscenario:" << subscenario->name() << "...";

    if (!scenario->deleteScenario(subscenario))
    {
        qCWarning(gmAudioEditor()) << "Error: Could not delete subscenario" << subscenario->name();
    }

    if (m_currentElement.isNull())
    {
        if (!loadFirstElement(scenario))
        {
            emit currentElementChanged();
        }
    }
}

/**
 * @brief Change the position of a subscenario
 */
void AudioEditor::moveSubscenario(AudioScenario *subscenario, int steps)
{
    if (!subscenario || !scenarioExists()) return;

    auto *scenario = m_currentProject->currentScenario();

    qCDebug(gmAudioEditor) << "Moving subscenario:" << subscenario->name() << "by" << steps << "...";

    if (!scenario->moveScenario(subscenario, steps))
    {
        qCDebug(gmAudioEditor()) << "Could not move subscenario.";
    }
}

/**
 * @brief Load an element to the editor
 */
void AudioEditor::loadElement(QObject* element)
{
    if (!scenarioExists() || !element || m_currentElement == element) return;

    m_currentElement = qobject_cast<AudioElement*>(element);
    emit currentElementChanged();

    if (m_currentElement.isNull()) return;

    qCDebug(gmAudioEditor) << "Loading element" << QString(*m_currentElement) << "...";

    fileModel->setElements(m_currentElement->files());
    emit currentElementChanged();

    for (auto *file : m_currentElement->files())
    {
        // Fetch video info
        if (file->source() == 3 && file->title().isEmpty())
        {
            auto *video = ytClient->getVideo(file->url());
            connect(video, &YouTube::Videos::Video::ready, file, [file, video]() {
                file->title(video->title());
                video->deleteLater();
            });
        }
    }

    // Tell AudioSaveLoad to find out if files are missing
    AudioSaveLoad::findMissingFilesAsync(m_currentElement->files(), basePath());
}

/**
 * @brief Load the first element of a scenario.
 */
auto AudioEditor::loadFirstElement(AudioScenario* scenario) -> bool
{
    if (!scenario)
    {
        if (!scenarioExists())
        {
            clearCurrentElement();
            return false;
        }

        scenario = m_currentProject->currentScenario();
    }

    auto elements = scenario->elements(true);

    if (!elements.isEmpty())
    {
        loadElement(elements.first());
        return true;
    }

    clearCurrentElement();
    return false;
}

/**
 * @brief Set the current element to null and clear the file model.
 */
void AudioEditor::clearCurrentElement()
{
    fileModel->clear();
    m_currentElement = nullptr;
    emit currentElementChanged();
}

void AudioEditor::createElement(const QString &name, AudioElement::Type type, int subscenario)
{
    if (!scenarioExists() || name.isEmpty()) return;

    qCDebug(gmAudioEditor()) << "Creating element of type" << type << "...";

    auto *scenario = m_currentProject->currentScenario();
    if (!scenario) return;

    auto path = scenario->path();

    // index -1 means no subscenario
    if (Utils::isInBounds(scenario->scenarios(), subscenario))
    {
        scenario = scenario->scenarios()[subscenario];
    }

    auto *element = new AudioElement(name, type, path, scenario);
    scenario->addElement(element);
}

/**
 * @brief Sort the elements in current scenario alphabetically
 */
void AudioEditor::sortElements()
{
    if (!m_currentProject || !m_currentProject->currentScenario()) return;

    qCDebug(gmAudioEditor) << "Sorting elements ...";
    m_currentProject->currentScenario()->sortElements(true);
}

/**
 * @brief Delete the current element and remove it from the scenario
 */
void AudioEditor::deleteCurrentElement()
{
    if (m_currentElement.isNull()) return;

    qCDebug(gmAudioEditor) << "Deleting current element (" << QString(*m_currentElement) << ") ...";

    if (scenarioExists())
    {
        auto *scenario = m_currentProject->currentScenario()->getScenario(m_currentElement);

        if (scenario)
        {
            scenario->removeElement(m_currentElement);

            if (!loadFirstElement(scenario))
            {
                loadFirstElement(m_currentProject->currentScenario());
            }
        }
        else
        {
            qCWarning(gmAudioEditor()) << "Error: Could not delete element, it is not part of the current scenario.";
        }
    }
    else
    {
        qCWarning(gmAudioEditor()) << "Error: Could not delete element, no scenario selected.";
    }

    //m_currentElement.clear();
    emit currentElementChanged();
}

/**
 * @brief Move element in list
 * @param element The element to be moved
 * @param positions Amount of positions to move element by
 */
void AudioEditor::moveElement(AudioElement *element, int positions)
{
    if (!scenarioExists() || !element) return;

    qCDebug(gmAudioEditor) << "Moving element" << QString(*element) << "by" << positions << "steps ...";

    if (!m_currentProject->currentScenario()->moveElement(element, positions))
    {
        qCDebug(gmAudioEditor()) << "Could not move element.";
    }
}

/**
 * @brief Save the current project
 */
void AudioEditor::saveProject()
{
    qCDebug(gmAudioEditor) << "Saving projects ...";

    if (!m_projects.empty())
    {
        for (auto p : m_projects)
        {
            if (p == m_currentProject)
            {
                emit showInfoBar(tr("Saving ..."));

                observe(AudioSaveLoad::saveProject(p)).subscribe([this]() {
                    emit showInfoBar(tr("Saved!"));
                }, [this]() {
                    emit showInfoBar(tr("Error: Could not save project!"));
                });
            }
            else
            {
                AudioSaveLoad::saveProject(p);
            }
        }
    }

    isSaved(true);
}

/**
 * @brief Project was edited and contains unsaved changes
 */
void AudioEditor::madeChanges()
{
    isSaved(false);
}

/**
 * @brief Add an AudioFile to the current element
 */
auto AudioEditor::addAudioFile(AudioFile *audioFile) -> bool
{
    if (m_currentElement->type() == AudioElement::Radio)
    {
        m_currentElement->setFiles({ audioFile });
        fileModel->setElements({ audioFile });
    }
    else if (m_currentElement->addFile(audioFile))
    {
        fileModel->append(audioFile);
    }
    else
    {
        return false;
    }

    emit fileIndexChanged(m_fileIndex);
    return true;
}

/**
 * @brief Add an URL to the current element
 * @param url New URL
 * @param mode Web: 0, Spotify: 1
 */
auto AudioEditor::addUrl(const QString &url, int mode, const QString &title) -> bool
{
    if (!m_currentElement || !scenarioExists() || url.isEmpty()) return false;

    qCDebug(gmAudioEditor) << "Adding URL to element" << QString(*m_currentElement) << ":" << url << "( Mode:" << mode << ") ...";

    auto *audioFile = new AudioFile(url, mode == 1 ? 2 : 1, title, m_currentElement);

    if (!addAudioFile(audioFile)) return false;

    // TODO: Fetch spotify info

    return true;
}

/**
 * @brief Add a youtube url to the current element
 */
auto AudioEditor::addYtUrl(const QString &videoUrl) -> bool
{
    if (!m_currentElement || !scenarioExists() || videoUrl.isEmpty()) return false;

    qCDebug(gmAudioEditor) << "Adding YouTube URL to element" << QString(*m_currentElement) << ":" << videoUrl;

    auto *audioFile = new AudioFile(videoUrl, 3, "", m_currentElement);
    if (!addAudioFile(audioFile)) return false;

    auto *video = ytClient->getVideo(videoUrl);
    connect(video, &YouTube::Videos::Video::ready, audioFile, [audioFile, video]() {
        audioFile->title(video->title());
        video->deleteLater();
    });

    return true;
}

/**
 * @brief Add a file to an element
 * @param path Path of the file to add
 */
auto AudioEditor::addFile(QStringList path, const QString &filename) -> bool
{
    if (!m_currentElement || !scenarioExists() || path.isEmpty()) return false;

    qCDebug(gmAudioEditor) << "Adding file to element" << QString(*m_currentElement) << ":" << path << filename;

    path.append(filename);
    auto pathString = "/" + FileUtils::dirFromFolders(path);
    return addAudioFile(new AudioFile(pathString, 0, "", m_currentElement));
}

/**
 * @brief Add a list of files to the current element
 * @param files List of file paths to add
 */
void AudioEditor::addFiles(const QStringList &files)
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Adding a list of files to element" << QString(*m_currentElement) << "...";

    for (const auto &file : files)
    {
        addAudioFile(new AudioFile(file, 0, "", m_currentElement));
    }
}

/**
 * @brief Remove a file from an element
 * @param index Index of the file to remove
 * @param findMissing If list of missing files should be updated after removing
 */
void AudioEditor::removeFile(int index, bool findMissing)
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Deleting file in element" << QString(*m_currentElement) << "at index" << index << "...";

    if (m_currentElement->removeFile(index))
    {
        fileModel->remove(index);
        emit currentElementChanged();

        if (findMissing)
        {
            // Tell AudioSaveLoad to find out if files are missing
            AudioSaveLoad::findMissingFilesAsync(m_currentElement->files(), basePath());
        }
    }
}

/**
 * @brief Remove all missing files from the current element
 */
void AudioEditor::removeMissingFiles()
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Removing missing files in element" << QString(*m_currentElement) << "...";

    auto files = m_currentElement->files();
    QList<int> indices;

    for (int i = files.length() - 1; i > 0; i--)
    {
        auto file = files[i];

        if (file && file->missing())
        {
            indices.prepend(i);
        }
    }

    for (int index : indices)
    {
        removeFile(index, false);
    }
}

/**
 * @brief Move a file in the current element
 * @param index Index of the file to be moved
 * @param positions Positions to move the file by
 */
void AudioEditor::moveFile(int index, int positions)
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Moving file in element" << QString(*m_currentElement) << "at index" << index << "by" << positions << "positions ...";

    if (m_currentElement->moveFile(index, positions))
    {
        fileModel->moveRow(QModelIndex(), index, QModelIndex(), index + positions);
        emit currentElementChanged();
    }
}

/**
 * @brief Change where a file is located
 * @param index Index of the file to change
 * @param folder New folder where file can be found
 */
void AudioEditor::replaceFileFolder(int index, const QString& folder)
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Replacing path of element" << QString(*m_currentElement) << "at index" << index << "with" << folder << "...";

    auto files = m_currentElement->files();
    QList<AudioFile*> missingFiles;

    if (Utils::isInBounds(files, index))
    {
        auto path = files[index]->url();
        auto folderPath  = FileUtils::dirFromPath(path);
        auto newFolderPath = folder;
        newFolderPath = newFolderPath.replace(basePath(), "");

        for (auto file : files)
        {
            if (file && file->missing() && file->url().contains(folderPath))
            {
                file->url(file->url().replace(folderPath, newFolderPath));
                missingFiles.append(file);
            }
        }

        AudioSaveLoad::findMissingFilesAsync(missingFiles, basePath());
    }
}

/**
 * @brief Set the subscenario of the current element. (Index == -1 -> no subscenario)
 */
void AudioEditor::setSubscenario(int index)
{
    if (!m_currentElement || !scenarioExists()) return;

    qCDebug(gmAudioEditor) << "Setting subscenario of" << QString(*m_currentElement) << ":" << index << "...";

    auto *currentScenario = m_currentProject->currentScenario();
    auto *origSubScenario = currentScenario->getScenario(m_currentElement);
    auto subScenarios = currentScenario->scenarios();

    AudioScenario *newScenario = nullptr;

    if (index < 0)
    {
        newScenario = currentScenario;
    }
    else if (Utils::isInBounds(subScenarios, index))
    {
        newScenario = subScenarios[index];
    }
    else {
        qCWarning(gmAudioEditor()) << "Error: Could not set subscenario, index out of bounds:" << index;
        return;
    }

    if (newScenario && newScenario != origSubScenario)
    {
        origSubScenario->removeElement(m_currentElement, false);
        newScenario->addElement(m_currentElement);
    }
    else
    {
        qCDebug(gmAudioEditor()) << "No changes made, subscenario is the same as before.";
    }
}

/**
 * @brief Get the base path the files of the current element are located at.
 */
auto AudioEditor::basePath() const -> QString
{
    if (!m_currentElement) return "";

    return SettingsManager::getPath(AudioElement::typeToSettings(m_currentElement->type()));
}

/**
 * @brief Check if the current category and project are not null.
 */
auto AudioEditor::categoryExists() const -> bool
{
    return m_currentProject && m_currentProject->currentCategory();
}

/**
 * @brief Check if the current scenario and project are not null.
 */
auto AudioEditor::scenarioExists() const -> bool
{
    return m_currentProject && m_currentProject->currentScenario();
}
