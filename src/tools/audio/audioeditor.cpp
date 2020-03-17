#include "audioeditor.h"
#include "functions.h"
#include "youtubeutils.h"

#include <algorithm>
#include <QDebug>
#include <QQmlContext>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QMediaPlayer>

AudioEditor::AudioEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) :
    QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Audio Editor ...";

    sManager            = new SettingsManager;
    addonElementManager = new AddonElementManager;
    audioExporter       = new AudioExporter;
    fileBrowser         = new AudioEditorFileBrowser(engine, this);
    unsplashParser      = new UnsplashParser(engine);

    elementModel = new AudioElementModelModel;
    elementModel->insert(new AudioElementModel);
    qmlEngine->rootContext()->setContextProperty("editorElementModel", elementModel);

    fileModel = new AudioFileModel;
    qmlEngine->rootContext()->setContextProperty("editorFileModel", fileModel);

    connect(fileManager->getAudioSaveLoad(), &AudioSaveLoad::foundProjects, [ = ](QList<AudioProject *>list, bool isEditor) {
        if (!isEditor) return;

        qDebug() << "AudioEditor: Projects changed!";
        m_projects = list;
        emit projectsChanged();

        setCurrentProject(0);
    });

    connect(this, &AudioEditor::currentScenarioChanged, [ = ]() {
        if (!m_currentProject || !m_currentProject->currentCategory()
            || !m_currentProject->currentCategory()->currentScenario())
        {
            elementModel->element(0)->setElements({});
            elementModel->clear();
            return;
        }

        updateElementModel();
        emit subscenariosChanged();
    });

    connect(addonElementManager, &AddonElementManager::exportElements, [ = ](QList<MusicElement *>elements, bool subscenario, int scenarioIndex) {
        if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentScenario())
        {
            if (subscenario && (scenarioIndex >= 0) && (scenarioIndex < m_currentProject->currentCategory()->currentScenario()->scenarios().length()))
            {
                for (auto e : elements)
                {
                    if (e)
                    {
                        m_currentProject->currentCategory()->currentScenario()->scenarios()[scenarioIndex]->addMusicElement(e);
                    }
                }
            }
            else
            {
                for (auto e : elements)
                {
                    if (e)
                    {
                        m_currentProject->currentCategory()->currentScenario()->addMusicElement(e);
                    }
                }
            }

            madeChanges();
            updateElementModel();
        }
    });

    fileManager->getAudioSaveLoad()->findEditorProjects(fileManager->getModeInt());
}

/**
 * @brief Get a list of all project names
 * @return List of project names
 */
QStringList AudioEditor::projectNames()
{
    if (m_projects.isEmpty()) return {};

    QStringList list;

    for (auto a : m_projects) if (a) list.append(a->name());

    return list;
}

/**
 * @brief Set the current project
 * @param index Index of project in project list
 */
void AudioEditor::setCurrentProject(int index)
{
    qDebug() << "AudioEditor: Change project:" << index;

    if ((index > -1) && (index < m_projects.size()))
    {
        m_currentProject = m_projects[index];
        audioExporter->setProject(m_currentProject);

        qDebug() << "       Emitting project changed";
        emit currentProjectChanged();
        qDebug() << "       Emitting category changed";
        emit currentCategoryChanged();
        qDebug() << "       Emitting scenario changed";
        emit currentScenarioChanged();
        qDebug() << "       Done";
    }
    else
    {
        m_currentProject = nullptr;
    }
}

/**
 * @brief Set the current project
 * @param project New active project
 */
void AudioEditor::setCurrentProject(AudioProject *project)
{
    m_currentProject = project;
    audioExporter->setProject(project);
    emit currentProjectChanged();
    emit currentCategoryChanged();
    emit currentScenarioChanged();
}

/**
 * @brief Get the index of the active project
 * @return Index of active project in project list
 */
int AudioEditor::projectIndex() const
{
    if (!m_currentProject || (m_projects.size() < 1)) return -1;

    return m_projects.indexOf(m_currentProject);
}

/**
 * @brief Create a new project
 * @param name Name of the new project
 */
void AudioEditor::createProject(QString name)
{
    if (name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating project" << name << "...";

    AudioProject *project = new AudioProject(name, 4, {});
    project->setCurrentCategory(nullptr);
    m_projects.append(project);
    emit projectsChanged();

    setCurrentProject(project);

    madeChanges();

    qDebug() << "Created project" << name << ".";
}

/**
 * @brief Rename the current project
 * @param name New name
 */
void AudioEditor::renameProject(QString name)
{
    if (!m_currentProject || name.isEmpty()) return;

    qDebug() << "AudioEditor: Renaming project" << m_currentProject->name() << "to" << name << "...";

    if (!m_currentProject->wasRenamed())
    {
        m_currentProject->setWasRenamed(true);
        m_currentProject->setOldName(m_currentProject->name());
    }

    m_currentProject->setName(name);
    emit projectsChanged();
    madeChanges();
}

void AudioEditor::deleteProject()
{
    if (!m_currentProject) return;

    qDebug() << "AudioEditor: Deleting current project:" << m_currentProject->name();

    auto project = m_currentProject;
    m_projects.removeOne(project);
    setCurrentProject(0);

    fileManager->getAudioSaveLoad()->deleteProject(project);
    emit projectsChanged();
}

/**
 * @brief Set the current category
 * @param name Name of new active category
 */
void AudioEditor::setCurrentCategory(QString name)
{
    if (name.isEmpty())
    {
        m_currentProject->setCurrentCategory(nullptr);
    }
    else
    {
        qDebug() << "AudioEditor: Setting current category" << name << "(" << m_currentProject->name() + ") ...";
        m_currentProject->setCurrentCategory(name);
    }

    emit currentCategoryChanged();
    emit currentScenarioChanged();

    if (!m_currentProject->currentCategory() || (m_currentProject->currentCategory()->scenarios().count() == 0))
    {
        clearCurrentElement();
    }
}

/**
 * @brief Get the index of the current category
 * @return Index of current category in list
 */
int AudioEditor::categoryIndex() const
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return -1;

    return m_currentProject->categoryIndex();
}

/**
 * @brief Create a new category
 * @param name Name of the new category
 */
void AudioEditor::createCategory(QString name)
{
    if (!m_currentProject || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating category" << name << "...";

    if (!m_currentProject->categoryNames().contains(name))
    {
        AudioCategory *category = new AudioCategory(name, {});
        m_currentProject->addCategory(category);
        emit currentProjectChanged();

        setCurrentCategory(name);

        madeChanges();
    }
}

/**
 * @brief Rename the current category
 * @param name New name
 */
void AudioEditor::renameCategory(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Renaming category" << m_currentProject->currentCategory()->name() << "to" << name << "...";

    m_currentProject->currentCategory()->setName(name);
    emit currentProjectChanged();
    madeChanges();
}

void AudioEditor::deleteCategory()
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return;

    qDebug() << "AudioEditor: Deleting current category:" << m_currentProject->currentCategory()->name();

    m_currentProject->deleteCategory(m_currentProject->currentCategory());

    emit currentProjectChanged();

    if (m_currentProject->categories().count() > 0)
    {
        setCurrentCategory(m_currentProject->categories()[0]->name());
    }
    else
    {
        setCurrentCategory("");
    }

    madeChanges();
}

/**
 * @brief Get a list of all scenario names in current category
 * @return List of scenario names
 */
QStringList AudioEditor::scenarioNames() const
{
    if (!m_currentProject || !m_currentProject->currentCategory())
    {
        return {};
    }
    else
    {
        return m_currentProject->currentCategory()->scenarioNames();
    }
}

/**
 * @brief Get a list of all subscenario names in current scenario
 * @return List of subscenario names
 */
QStringList AudioEditor::subscenarioNames() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario())
    {
        return {};
    }
    else
    {
        QStringList l = { tr("No Subscenario") };
        l.append(m_currentProject->currentCategory()->currentScenario()->scenarioNames());
        return l;
    }
}

/**
 * @brief Set the current scenario
 * @param name Name of the scenario
 */
void AudioEditor::setCurrentScenario(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || name.isEmpty())
    {
        clearCurrentElement();
        return;
    }

    if (name.isEmpty())
    {
        m_currentProject->currentCategory()->setCurrentScenario(nullptr);
    }
    else
    {
        qDebug() << "AudioEditor: Setting current scenario" << name << "...";
        m_currentProject->currentCategory()->setCurrentScenario(name);
    }

    emit currentScenarioChanged();

    if (!m_currentProject->currentCategory()
        || !m_currentProject->currentCategory()->currentScenario()
        || (m_currentProject->currentCategory()->scenarios().count() == 0))
    {
        clearCurrentElement();
    }
}

/**
 * @brief Get the index of the current scenario
 * @return Index of current scenario in list
 */
int AudioEditor::scenarioIndex() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return -1;

    return m_currentProject->currentCategory()->scenarioIndex();
}

/**
 * @brief Create a new scenario
 * @param name Name of the new scenario
 */
void AudioEditor::createScenario(QString name, bool subscenario)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating scenario" << name << "...";

    if (subscenario && !m_currentProject->currentCategory()->currentScenario()->scenarioNames().contains(name))
    {
        AudioScenario *scenario = new AudioScenario(name, {}, {}, {}, {});

        m_currentProject->currentCategory()->currentScenario()->addScenario(scenario);
        emit currentScenarioChanged();

        madeChanges();
    }
    else if (!m_currentProject->currentCategory()->scenarioNames().contains(name))
    {
        AudioScenario *scenario = new AudioScenario(name, {}, {}, {}, {});
        m_currentProject->currentCategory()->addScenario(scenario);
        emit currentCategoryChanged();

        setCurrentScenario(name);

        madeChanges();
    }
}

/**
 * @brief Rename the current scenario
 * @param name New name
 */
void AudioEditor::renameScenario(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Renaming scenario" << m_currentProject->currentCategory()->currentScenario()->name() << "to" << name << "...";

    m_currentProject->currentCategory()->currentScenario()->setName(name);
    emit currentCategoryChanged();
    madeChanges();
}

void AudioEditor::deleteScenario()
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Deleting current scenario:" << m_currentProject->currentCategory()->currentScenario()->name();

    m_currentProject->currentCategory()->deleteScenario(m_currentProject->currentCategory()->currentScenario());

    emit currentCategoryChanged();

    if (m_currentProject->currentCategory()->scenarios().count() > 0)
    {
        setCurrentScenario(m_currentProject->currentCategory()->scenarios()[0]->name());
    }
    else
    {
        setCurrentScenario("");
    }

    madeChanges();
}

void AudioEditor::deleteSubScenario(QString name)
{
    deleteElement(name, 4);
}

/**
 * @brief Put elements of current scenario in element model
 */
void AudioEditor::updateElementModel()
{
    qDebug() << "AudioEditor: Updating element model ...";

    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) elementModel->setElements({});

    elementModel->clear();
    elementModel->element(0)->setElements(m_currentProject->currentCategory()->currentScenario()->elements());

    for (auto s : m_currentProject->currentCategory()->currentScenario()->scenarios())
    {
        auto model = new AudioElementModel;
        model->setName(s->name());
        model->setElements(s->elements());
        elementModel->append(model);
    }
}

/**
 * @brief Load an element to the editor
 * @param name Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 */
void AudioEditor::loadElement(QString name, int type, QString subscenario)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Loading element" << name << "of type" << type << "...";

    AudioElement *element;
    QString basePath;
    AudioScenario *scenario;
    m_subscenario = subscenario;

    if (subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(subscenario);
    }

    switch (type)
    {
    case 0: // Music
        element  = scenario->musicElement(name);
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        element  = scenario->soundElement(name);
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        element  = scenario->radioElement(name);
        basePath = sManager->getSetting(Setting::radioPath);
        break;

    case 4: // Subscenario
        m_name = name;
        m_type = type;
        emit currentElementChanged();
        return;

    default:
        return;
    }

    m_name = name;
    m_type = element->type();
    m_icon = element->relativeIcon();
    m_mode = element->mode();

    fileModel->setElements(element->files());
    emit currentElementChanged();

    // Tell AudioSaveLoad to find out if files are missing
    fileManager->getAudioSaveLoad()->findMissingFiles(element->files(), basePath);
}

void AudioEditor::clearCurrentElement()
{
    m_name.clear();
    m_subscenario.clear();
    m_icon.clear();
    m_id.clear();
    fileModel->clear();
    m_mode = 0;
    m_type = -1;

    emit currentElementChanged();
}

/**
 * @brief Get the index of a subscenario
 * @param scenario Name of the subscenario
 * @return Index
 */
int AudioEditor::getSubscenarioIndex(QString scenario)
{
    if (scenario.isEmpty()) return -1;

    auto scenarios = m_currentProject->currentCategory()->currentScenario()->scenarios();

    for (int i = 0; i < scenarios.length(); i++)
    {
        if (scenarios[i]->name() == scenario)
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Create a new music element
 * @param name Name of the element
 */
void AudioEditor::createMusicElement(QString name, bool subscenario, int scenarioIndex)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating music element" << name << "...";

    // In Subscenario
    if (subscenario && (scenarioIndex > 0) && !m_currentProject->currentCategory()->currentScenario()->musicElementNames().contains(name))
    {
        MusicElement *element = new MusicElement(name);
        m_currentProject->currentCategory()->currentScenario()->scenarios()[scenarioIndex - 1]->addMusicElement(element);
        emit currentScenarioChanged();
        madeChanges();
    }

    // Default
    else if (!m_currentProject->currentCategory()->currentScenario()->musicElementNames().contains(name))
    {
        MusicElement *element = new MusicElement(name);
        m_currentProject->currentCategory()->currentScenario()->addMusicElement(element);
        emit currentScenarioChanged();
        madeChanges();
    }
}

/**
 * @brief Create a new sound element
 * @param name Name of the element
 */
void AudioEditor::createSoundElement(QString name, bool subscenario, int scenarioIndex)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating sound element" << name << "...";

    if (subscenario && (scenarioIndex > 0) && !m_currentProject->currentCategory()->currentScenario()->soundElementNames().contains(name))
    {
        SoundElement *element = new SoundElement(name);
        m_currentProject->currentCategory()->currentScenario()->scenarios()[scenarioIndex - 1]->addSoundElement(element);
        emit currentScenarioChanged();
        madeChanges();
    }
    else if (!m_currentProject->currentCategory()->currentScenario()->soundElementNames().contains(name))
    {
        SoundElement *element = new SoundElement(name);
        m_currentProject->currentCategory()->currentScenario()->addSoundElement(element);
        emit currentScenarioChanged();

        madeChanges();
    }
}

/**
 * @brief Create a new radio element
 * @param name Name of the element
 */
void AudioEditor::createRadioElement(QString name, bool subscenario, int scenarioIndex)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating radio" << name << "...";

    if (subscenario && (scenarioIndex > 0) && !m_currentProject->currentCategory()->currentScenario()->radioElementNames().contains(name))
    {
        RadioElement *element = new RadioElement(name);
        m_currentProject->currentCategory()->currentScenario()->scenarios()[scenarioIndex - 1]->addRadioElement(element);
        emit currentScenarioChanged();
        madeChanges();
    }
    else if (!m_currentProject->currentCategory()->currentScenario()->radioElementNames().contains(name))
    {
        RadioElement *element = new RadioElement(name);
        m_currentProject->currentCategory()->currentScenario()->addRadioElement(element);
        emit currentScenarioChanged();

        madeChanges();
    }
}

/**
 * @brief Sort the elements in current scenario alphabetically
 */
void AudioEditor::sortElements()
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Sorting elements ...";

    m_currentProject->currentCategory()->currentScenario()->sortElements();

    for (auto s : m_currentProject->currentCategory()->currentScenario()->scenarios())
    {
        s->sortElements();
    }

    updateElementModel();

    madeChanges();
}

/**
 * @brief Move element in list
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param positions Amount of positions to move element by
 */
void AudioEditor::moveElement(QString element, int type, int positions)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Moving element" << element << "of type" << type << "by" << positions << "steps ...";

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }


    switch (type) {
    case 0: // Music
        scenario->moveMusicElement(element, positions);
        break;

    case 1: // Sound
        scenario->moveSoundElement(element, positions);
        break;

    case 2: // Radio
        scenario->moveRadioElement(element, positions);
        break;

    case 4: // Subscenario
        scenario->moveScenario(element, positions);
        emit subscenariosChanged();
        break;

    default:
        break;
    }

    updateElementModel();

    madeChanges();
}

/**
 * @brief Delete an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 */
void AudioEditor::deleteElement(QString element, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Deleting element" << element << "of type" << type << "...";

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    switch (type) {
    case 0: // Music
        scenario->removeMusicElement(element);
        break;

    case 1: // Music
        scenario->removeSoundElement(element);
        break;

    case 2: // Music
        scenario->removeRadioElement(element);
        break;

    case 4: // Subscenario
        scenario->removeScenario(element);
        emit subscenariosChanged();
        break;

    default:
        break;
    }

    updateElementModel();

    madeChanges();
}

/**
 * @brief Save the current project
 */
void AudioEditor::saveProject()
{
    qDebug() << "AudioEditor: Saving project ...";

    if (m_projects.size() > 0)
    {
        for (auto p : m_projects)
        {
            fileManager->getAudioSaveLoad()->saveProject(p);
        }
    }

    m_isSaved = true;
    emit isSavedChanged();
    emit showInfoBar(tr("Saved!"));
}

/**
 * @brief Project was edited and contains unsaved changes
 */
void AudioEditor::madeChanges()
{
    if (!m_currentProject) return;

    m_currentProject->setSaved(false);
    m_isSaved = false;
    emit isSavedChanged();
}

/**
 * @brief Add an URL to an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param url New URL
 * @param mode Web: 0, Spotify: 1
 */
void AudioEditor::addUrl(QString elementName, int type, QString url, int mode, QString title)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Adding URL to element" << elementName << ":" << url << "( Mode:" << mode << ") ...";

    auto element = getElement(elementName, type);

    if (element && !url.isEmpty())
    {
        int source = 1;

        if (mode == 1) source = 2;

        auto audioFile = new AudioFile(url, source, title);

        if (type == 2) // Radio
        {
            element->setFiles({ audioFile });
            fileModel->setElements({ audioFile });
        }
        else
        {
            auto files = element->files();
            files.append(audioFile);
            element->setFiles(files);
            fileModel->append(audioFile);
        }

        emit currentElementChanged();
        emit fileIndexChanged(m_fileIndex);

        madeChanges();
    }
}

void AudioEditor::addYtUrl(QString elementName, int type, QString videoUrl)
{
    qDebug() << "Adding YouTube URL:" << videoUrl;

    auto videoId = YouTubeUtils::parseVideoId(videoUrl);
    auto element = getElement(elementName, type);

    if (element && YouTubeUtils::validateVideoId(videoId))
    {
        auto audioFile = new AudioFile(videoUrl, 3, "");

        auto files = element->files();
        files.append(audioFile);
        element->setFiles(files);
        fileModel->append(audioFile);

        emit currentElementChanged();
        emit fileIndexChanged(m_fileIndex);

        madeChanges();
    }
}

/**
 * @brief Add a file to an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param path Path of the file to add
 */
void AudioEditor::addFile(QString elementName, int type, QStringList path, QString filename)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Adding file to element" << elementName << "of type" << type << ":" << path << filename;

    auto pathString = "/" + path.join("/") + "/" + filename;
    auto element    = getElement(elementName, type);

    if (element && !path.isEmpty())
    {
        auto audioFile = new AudioFile(pathString, 0);

        if (type == 2) // Radio
        {
            element->setFiles({ audioFile });
            fileModel->setElements({ audioFile });
        }
        else
        {
            auto files = element->files();
            files.append(audioFile);
            element->setFiles(files);
            fileModel->append(audioFile);
        }

        emit currentElementChanged();
        emit fileIndexChanged(m_fileIndex);

        madeChanges();
    }
}

/**
 * @brief Add a list of files to the current element
 * @param files List of file paths to add
 */
void AudioEditor::addFiles(QStringList files)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Adding a list of files:";

    auto element = getElement(m_name, m_type);

    if (element)
    {
        fileModel->setElements(element->files());

        for (auto file : files)
        {
            if (!file.isEmpty())
            {
                qDebug() << "   " << file;
                fileModel->append(new AudioFile(file, 0));
            }
        }

        if (!files.isEmpty())
        {
            QList<AudioFile *> elementFiles;

            for (auto file : fileModel->elements())
            {
                elementFiles.append(static_cast<AudioFile *>(file));
            }
            element->setFiles(elementFiles);

            emit currentElementChanged();
            emit fileIndexChanged(m_fileIndex);

            madeChanges();
        }
    }
}

/**
 * @brief Remove a file from an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param index Index of the file to remove
 * @param findMissing If list of missing files should be updated after removing
 */
void AudioEditor::removeFile(QString name, int type, int index, bool findMissing)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Deleting file in element" << name << "of type" << type << "at index" << index << "...";

    // Get scenario
    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    AudioElement *element = nullptr;
    QString basePath;
    QList<AudioFile *> files;

    // Set basePath and find element
    switch (type)
    {
    case 0: // Music
        element  = scenario->musicElement(name);
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        element  = scenario->soundElement(name);
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    default: return;
    }

    // Remove file from list
    if (element)
    {
        files = element->files();

        if ((index < files.size()) && (index > -1))
        {
            files.removeAt(index);
            fileModel->remove(fileModel->elements()[index]);

            element->setFiles(files);
            emit currentElementChanged();

            madeChanges();

            if (findMissing)
            {
                // Tell AudioSaveLoad to find out if files are missing
                fileManager->getAudioSaveLoad()->findMissingFiles(files, basePath);
            }
        }
    }
}

/**
 * @brief Remove all missing files from an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 */
void AudioEditor::removeMissingFiles(QString name, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Removing missing files in element" << name << "of type" << type << "...";

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    AudioElement *element = nullptr;
    QString basePath;
    QList<AudioFile> list;

    switch (type)
    {
    case 0: // Music
        element  = scenario->musicElement(name);
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        element  = scenario->soundElement(name);
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    default: return;
    }

    if (element)
    {
        for (int i = 0; i < fileModel->elements().count(); i++)
        {
            if (static_cast<AudioFile *>(fileModel->elements()[i])->missing() && (i < element->files().count()))
            {
                removeFile(name, type, i, false);
            }
        }

        // fileManager->getAudioSaveLoad()->findMissingFiles(m_files, basePath);

        madeChanges();
    }
}

/**
 * @brief Move a file in an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param index Index of the file to be moved
 * @param positions Positions to move the file by
 */
void AudioEditor::moveFile(QString name, int type, int index, int positions)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Moving file in element" << name << "of type" << type << "at index" << index << "by" << positions << "positions ...";

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    AudioElement *element = nullptr;
    QList<AudioFile *> files;

    switch (type)
    {
    case 0: // Music
        element = scenario->musicElement(name);
        files   = element->files();
        break;

    case 1: // Sounds
        element = scenario->soundElement(name);
        files   = element->files();
        break;

    default: return;
    }

    if (element && (index + positions < files.size()) && (index + positions > -1))
    {
        files.move(index, index + positions);
        fileModel->moveRow(QModelIndex(), index, QModelIndex(), index + positions);
        element->setFiles(files);

        emit currentElementChanged();

        madeChanges();
    }
}

/**
 * @brief Change where a file is located
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param index Index of the file to change
 * @param folder New folder where file can be found
 */
void AudioEditor::replaceFileFolder(QString name, int type, int index, QString folder)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Replacing path of element" << name << "of type" << type << "at index" << index << "with" << folder << "...";

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    AudioElement *element = nullptr;

    switch (type)
    {
    case 0: // Music
        element = scenario->musicElement(name);
        break;

    case 1: // Sounds
        element = scenario->soundElement(name);
        break;

    default: return;
    }

    if (element)
    {
        if (index < fileModel->elements().count())
        {
            QString path          = static_cast<AudioFile *>(fileModel->elements()[index])->url();
            QString folderPath    = path.left(path.lastIndexOf('/'));
            QString newFolderPath = folder.right(folder.length() - folder.indexOf(basePath(element->type())));
            newFolderPath = newFolderPath.replace(basePath(element->type()), "");

            QList<AudioFile *> audioFiles;
            auto elementFiles = element->files();

            for (int i = 0; i < fileModel->elements().size(); i++)
            {
                auto file = static_cast<AudioFile *>(fileModel->elements()[i]);

                if (file->missing() && file->url().contains(folderPath) && (i < element->files().count()))
                {
                    file->setUrl(file->url().replace(folderPath, newFolderPath));
                    elementFiles[i] = file;
                }

                audioFiles.append(file);
            }

            element->setFiles(elementFiles);
            emit currentElementChanged();

            fileManager->getAudioSaveLoad()->findMissingFiles(audioFiles, basePath(element->type()));
            madeChanges();
        }
    }
}

/**
 * @brief Set the icon of an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param path Path to the new icon
 */
void AudioEditor::setIcon(QString element, int type, QString path)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Setting icon of" << element << "of type" << type << ":" << path << "...";

    AudioElement *e = getElement(element, type);

    if (!e) return;

    if (path.startsWith("http:") || path.startsWith("https:"))
    {
        e->setRelativeIcon(path);
        e->icon()->setBackground(path);
        madeChanges();
    }
    else if (e->relativeIcon() != path)
    {
        e->setRelativeIcon(path);
        e->icon()->setBackground(resourcesPath() + path);

        madeChanges();
    }
}

/**
 * @brief Change the name of an element
 * @param element Old name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param name New name
 */
void AudioEditor::setName(QString element, int type, QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Changing name of" << element << "of type" << type << ":" << name << "...";

    // Subscenario
    if (type == 4)
    {
        for (auto s : m_currentProject->currentCategory()->currentScenario()->scenarios())
        {
            if (s && (s->name() == element))
            {
                s->setName(name);
                madeChanges();
                emit currentScenarioChanged();
                return;
            }
        }
    }

    // Audio Element
    AudioElement *e = getElement(element, type);

    if (e)
    {
        m_name = name;
        e->setName(name);

        madeChanges();
    }
}

void AudioEditor::setSubscenario(QString element, int type, int index)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || (index < 0) ||
        (index - 1 > m_currentProject->currentCategory()->currentScenario()->scenarios().length())) return;

    qDebug() << "AudioEditor: Setting subscenario of" << element << "of type" << type << ":" << index << "...";

    AudioElement  *e = getElement(element, type);
    AudioScenario *scenario = nullptr, *scenario2 = nullptr;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    qDebug() << "Scenario1:" << scenario->name();

    if (index == 0)
    {
        scenario2     = m_currentProject->currentCategory()->currentScenario();
        m_subscenario = "";
    }
    else
    {
        scenario2     = m_currentProject->currentCategory()->currentScenario()->scenarios()[index - 1];
        m_subscenario = scenario2->name();
    }

    qDebug() << "Scenario2:" << scenario2->name();

    if (e && scenario && scenario2)
    {
        switch (type)
        {
        case 0: // Music
            scenario2->addMusicElement(static_cast<MusicElement *>(e));
            scenario->removeMusicElement(e->name(), false);
            break;

        case 1: // Sound
            scenario2->addSoundElement(static_cast<SoundElement *>(e));
            scenario->removeSoundElement(e->name(), false);
            break;

        case 2: // Radio
            scenario2->addRadioElement(static_cast<RadioElement *>(e));
            scenario->removeRadioElement(e->name(), false);
            break;

        default: return;
        }

        updateElementModel();
        madeChanges();
    }
}

/**
 * @brief Set the mode of an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @param mode New mode, depends on the element
 */
void AudioEditor::setMode(QString element, int type, int mode)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Set mode of" << element << "of type" << type << ":" << mode << "...";

    AudioElement *e = getElement(element, type);

    if (e)
    {
        e->setMode(mode);
        m_mode = mode;
        emit currentElementChanged();

        madeChanges();
    }
}

/**
 * @brief Get the base path where files are located in
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @return Path to the files
 */
QString AudioEditor::basePath(int type)
{
    switch (type)
    {
    case 0: // Music
        return sManager->getSetting(Setting::musicPath);

    case 1: // Sounds
        return sManager->getSetting(Setting::soundPath);

    case 2: // Radio
        return sManager->getSetting(Setting::radioPath);

    default: return "";
    }
}

/**
 * @brief Get an element by name and type
 * @param name Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2
 * @return Pointer to the requested element, nullptr if element is not found
 */
AudioElement * AudioEditor::getElement(QString name, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return nullptr;

    AudioScenario *scenario;

    if (m_subscenario.isEmpty())
    {
        scenario = m_currentProject->currentCategory()->currentScenario();
    }
    else
    {
        scenario = m_currentProject->currentCategory()->currentScenario()->scenario(m_subscenario);
    }

    switch (type)
    {
    case 0: // Music
        return scenario->musicElement(name);

    case 1: // Sound
        return scenario->soundElement(name);

    case 2: // Radio
        return scenario->radioElement(name);

    default: return nullptr;
    }
}
