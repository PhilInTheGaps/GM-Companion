#include "audioeditor.h"
#include "src/functions.h"

#include <algorithm>
#include <QDebug>
#include <QQmlContext>
#include <QSettings>
#include <QDir>
#include <QFile>

AudioEditor::AudioEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) :
    QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Audio Editor ...";

    sManager            = new SettingsManager;
    addonElementManager = new AddonElementManager;
    audioExporter       = new AudioExporter;
    fileBrowser         = new AudioEditorFileBrowser;
    unsplashParser      = new UnsplashParser(engine);

    elementModel = new AudioElementModel;
    qmlEngine->rootContext()->setContextProperty("editorElementModel", elementModel);

    connect(fileBrowser,                        &AudioEditorFileBrowser::addFiles,        this, &AudioEditor::addFiles);

    connect(fileManager->getAudioFileManager(), &AudioFileManager::editorProjectsChanged, [ = ](QList<AudioProject *>list) {
        qDebug() << "AudioEditor: Projects changed!";
        m_projects = list;
        emit projectsChanged();

        setCurrentProject(0);
    });

    connect(this, &AudioEditor::currentScenarioChanged, [ = ]() {
        if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario())
        {
            elementModel->setElements({});
            return;
        }

        updateElementModel();
    });

    connect(addonElementManager, &AddonElementManager::exportElements, [ = ](QList<SpotifyElement *>elements) {
        if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentScenario())
        {
            for (auto e : elements)
            {
                if (e)
                {
                    m_currentProject->currentCategory()->currentScenario()->addSpotifyElement(e);
                }
            }

            madeChanges();
            updateElementModel();
        }
    });

    connect(fileManager->getAudioFileManager(), &AudioFileManager::missingFilesFound, this, &AudioEditor::foundMissingFiles);

    fileManager->getAudioFileManager()->findEditorProjects(fileManager->getModeInt());
}

/**
 * @brief Get a list of all project names
 * @return List of project names
 */
QStringList AudioEditor::projectNames()
{
    if (m_projects.isEmpty()) return {};

    QStringList list;

    for (auto a : m_projects) list.append(a->name());

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
        emit currentProjectChanged();
        emit currentCategoryChanged();
        emit currentScenarioChanged();
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

    AudioProject *project = new AudioProject(name, 2, {});
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

/**
 * @brief Set the current category
 * @param name Name of new active category
 */
void AudioEditor::setCurrentCategory(QString name)
{
    qDebug() << "AudioEditor: Setting current category" << name << "...";
    m_currentProject->setCurrentCategory(name);
    emit currentCategoryChanged();
    emit currentScenarioChanged();
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
 * @brief Set the current scenario
 * @param name Name of the scenario
 */
void AudioEditor::setCurrentScenario(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Setting current scenario" << name << "...";

    m_currentProject->currentCategory()->setCurrentScenario(name);
    emit currentScenarioChanged();
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
void AudioEditor::createScenario(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating scenario" << name << "...";

    if (!m_currentProject->currentCategory()->scenarioNames().contains(name))
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

/**
 * @brief Put elements of current scenario in element model
 */
void AudioEditor::updateElementModel()
{
    qDebug() << "AudioEditor: Updating element model ...";

    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) elementModel->setElements({});

    elementModel->setElements(m_currentProject->currentCategory()->currentScenario()->elements());
}

/**
 * @brief Load an element to the editor
 * @param name Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 */
void AudioEditor::loadElement(QString name, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Loading element" << name << "of type" << type << "...";

    AudioElement *element;
    QString basePath;
    auto    scenario = m_currentProject->currentCategory()->currentScenario();

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

    case 3: // Spotify
        element = scenario->spotifyElement(name);
        break;

    default:
        return;
    }

    if (type == 2) // Radio
    {
        auto r = static_cast<RadioElement *>(element);
        m_local = r->local();
        m_url   = r->url().toString();
    }
    else if (type == 3) // Spotify
    {
        auto s = static_cast<SpotifyElement *>(element);
        m_id = s->id();
    }

    m_name  = name;
    m_type  = element->type();
    m_icon  = element->relativeIcon();
    m_files = element->files();
    m_mode  = element->mode();
    emit currentElementChanged();

    // Tell AudioFileManager to find out if files are missing
    m_missing.clear();

    for (auto s : m_files) m_missing.append(false);
    emit missingChanged();

    fileManager->getAudioFileManager()->findMissingFiles(m_files, basePath);
}

/**
 * @brief Update the list of missing files if they changed
 * @param missing List contains if file at the index is missing
 */
void AudioEditor::foundMissingFiles(QList<bool>missing)
{
    qDebug() << "AudioEditor: Found some missing files.";

    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    switch (m_type)
    {
    case 0: // Music

        if (m_currentProject->currentCategory()->currentScenario()->musicElement(m_name)->files().size() != missing.size()) return;

        break;

    case 1: // Sounds

        if (m_currentProject->currentCategory()->currentScenario()->soundElement(m_name)->files().size() != missing.size()) return;

        break;

    case 2: // Radio

        if (m_currentProject->currentCategory()->currentScenario()->radioElement(m_name)->files().size() != missing.size()) return;

        break;

    case 3: // Spotify

        if (m_currentProject->currentCategory()->currentScenario()->spotifyElement(m_name)->files().size() != missing.size()) return;

        break;

    default:
        return;
    }

    m_missing = missing;
    emit missingChanged();
}

/**
 * @brief Create a new music element
 * @param name Name of the element
 */
void AudioEditor::createMusicElement(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating music element" << name << "...";

    if (!m_currentProject->currentCategory()->currentScenario()->musicElementNames().contains(name))
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
void AudioEditor::createSoundElement(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating sound element" << name << "...";

    if (!m_currentProject->currentCategory()->currentScenario()->soundElementNames().contains(name))
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
void AudioEditor::createRadioElement(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating radio" << name << "...";

    if (!m_currentProject->currentCategory()->currentScenario()->radioElementNames().contains(name))
    {
        RadioElement *element = new RadioElement(name, false);
        m_currentProject->currentCategory()->currentScenario()->addRadioElement(element);
        emit currentScenarioChanged();

        madeChanges();
    }
}

/**
 * @brief Create a new spotify element
 * @param name Name of the element
 */
void AudioEditor::createSpotifyElement(QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Creating spotify element" << name << "...";

    if (!m_currentProject->currentCategory()->currentScenario()->spotifyElementNames().contains(name))
    {
        SpotifyElement *element = new SpotifyElement(name, "");
        m_currentProject->currentCategory()->currentScenario()->addSpotifyElement(element);
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
    updateElementModel();

    madeChanges();
}

/**
 * @brief Move element in list
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param positions Amount of positions to move element by
 */
void AudioEditor::moveElement(QString element, int type, int positions)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Moving element" << element << "of type" << type << "by" << positions << "steps ...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

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

    case 3: // Spotify
        scenario->moveSpotifyElement(element, positions);
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
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 */
void AudioEditor::deleteElement(QString element, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Deleting element" << element << "of type" << type << "...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

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

    case 3: // Music
        scenario->removeSpotifyElement(element);
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
            fileManager->getAudioFileManager()->saveProject(p);
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
 * @brief Add a file to an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param path Path of the file to add
 */
void AudioEditor::addFile(QString element, int type, QString path)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Adding file to element" << element << "of type" << type << ":" << path;

    if (type == 2) // Radio
    {
        setLocal(element, type, true);
        setUrl(element, type, path);
        return;
    }

    auto e = getElement(m_name, m_type);

    if (e && !path.isEmpty())
    {
        m_files = e->files();
        m_files.append(path);
        e->setFiles(m_files);
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

    auto e = getElement(m_name, m_type);

    if (e)
    {
        m_files = e->files();

        for (auto f : files)
        {
            if (!f.isEmpty())
            {
                qDebug() << "   " << f;
                m_files.append(f);
            }
        }

        if (!files.isEmpty())
        {
            e->setFiles(m_files);
            emit currentElementChanged();
            emit fileIndexChanged(m_fileIndex);

            madeChanges();
        }
    }
}

/**
 * @brief Remove a file from an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param index Index of the file to remove
 * @param findMissing If list of missing files should be updated after removing
 */
void AudioEditor::removeFile(QString element, int type, int index, bool findMissing)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Deleting file in element" << element << "of type" << type << "at index" << index << "...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

    AudioElement *e = nullptr;
    QString basePath;
    QStringList list;

    switch (type)
    {
    case 0: // Music
        e        = scenario->musicElement(element);
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        e        = scenario->soundElement(element);
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    default: return;
    }

    if (e)
    {
        list = e->files();

        if ((index < list.size()) && (index > 0))
        {
            list.removeAt(index);
            e->setFiles(list);
            m_files = list;
            emit currentElementChanged();

            madeChanges();

            if (findMissing)
            {
                // Tell AudioFileManager to find out if files are missing
                m_missing.clear();

                for (auto s : m_files) m_missing.append(false);
                emit missingChanged();

                fileManager->getAudioFileManager()->findMissingFiles(m_files, basePath);
            }
        }
    }
}

/**
 * @brief Remove all missing files from an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 */
void AudioEditor::removeMissingFiles(QString element, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Removing missing files in element" << element << "of type" << type << "...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();
    AudioElement *e     = nullptr;
    QString basePath;
    QStringList list;

    switch (type)
    {
    case 0: // Music
        e        = scenario->musicElement(element);
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        e        = scenario->soundElement(element);
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    default: return;
    }

    if (e)
    {
        list = e->files();

        for (int i = 0; i < list.size(); i++)
        {
            if ((i < m_missing.size()) && m_missing[i])
            {
                removeFile(element, type, i, false);
            }
        }

        // Tell AudioFileManager to find out if files are missing
        m_missing.clear();

        for (auto s : m_files) m_missing.append(false);
        emit missingChanged();

        fileManager->getAudioFileManager()->findMissingFiles(m_files, basePath);

        madeChanges();
    }
}

/**
 * @brief Move a file in an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param index Index of the file to be moved
 * @param positions Positions to move the file by
 */
void AudioEditor::moveFile(QString element, int type, int index, int positions)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Moving file in element" << element << "of type" << type << "at index" << index << "by" << positions << "positions ...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

    AudioElement *e = nullptr;
    QStringList   list;

    switch (type)
    {
    case 0: // Music
        e    = scenario->musicElement(element);
        list = e->files();
        break;

    case 1: // Sounds
        e    = scenario->soundElement(element);
        list = e->files();
        break;

    default: return;
    }

    if (e && (index + positions < list.size()) && (index + positions > 0))
    {
        list.move(index, index + positions);
        e->setFiles(list);
        m_files = list;
        emit currentElementChanged();

        madeChanges();
    }
}

/**
 * @brief Change where a file is located
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param index Index of the file to change
 * @param folder New folder where file can be found
 */
void AudioEditor::replaceFileFolder(QString element, int type, int index, QString folder)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Replacing path of element" << element << "of type" << type << "at index" << index << "with" << folder << "...";

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

    AudioElement *e = nullptr;
    QStringList   list;

    switch (type)
    {
    case 0: // Music
        e = scenario->musicElement(element);
        break;

    case 1: // Sounds
        e = scenario->soundElement(element);
        break;

    default: return;
    }

    if (e)
    {
        list = e->files();

        if (index < list.size())
        {
            QString path          = list[index];
            QString folderPath    = path.left(path.lastIndexOf('/'));
            QString newFolderPath = folder.right(folder.length() - folder.indexOf(basePath(e->type())));
            newFolderPath = newFolderPath.replace(basePath(e->type()), "");

            for (int i = 0; i < list.size(); i++)
            {
                if ((i < m_missing.size()) && m_missing[i] && list[i].contains(folderPath))
                {
                    list[i] = list[i].replace(folderPath, newFolderPath);
                }
            }

            e->setFiles(list);
            m_files = list;
            emit currentElementChanged();

            // Tell AudioFileManager to find out if files are missing
            m_missing.clear();

            for (auto s : m_files) m_missing.append(false);
            emit missingChanged();

            fileManager->getAudioFileManager()->findMissingFiles(m_files, basePath(e->type()));

            madeChanges();
        }
    }
}

/**
 * @brief Set the icon of an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param path Path to the new icon
 */
void AudioEditor::setIcon(QString element, int type, QString path)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return;

    qDebug() << "AudioEditor: Setting icon of" << element << "of type" << type << ":" << path << "...";

    AudioElement *e = getElement(element, type);

    if (path.startsWith("http:") || path.startsWith("https:"))
    {
        e->setRelativeIcon(path);
        e->setIcon(path);
        madeChanges();
    }
    else if (e->relativeIcon() != path)
    {
        e->setRelativeIcon(path);
        e->setIcon(resourcesPath() + path);

        madeChanges();
    }
}

/**
 * @brief Change the name of an element
 * @param element Old name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param name New name
 */
void AudioEditor::setName(QString element, int type, QString name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || name.isEmpty()) return;

    qDebug() << "AudioEditor: Changing name of" << element << "of type" << type << ":" << name << "...";

    AudioElement *e = getElement(element, type);

    if (e)
    {
        m_name = name;
        e->setName(name);

        madeChanges();
    }
}

/**
 * @brief Set the mode of an element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
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
 * @brief Set the URL of an element (currently only radio element)
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param url New URL
 */
void AudioEditor::setUrl(QString element, int type, QString url)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || (type != 2)) return;

    qDebug() << "AudioEditor: Set url of radio" << element << ":" << url << "...";

    auto e = m_currentProject->currentCategory()->currentScenario()->radioElement(element);
    e->setUrl(url);
    m_url = url;

    emit currentElementChanged();
    madeChanges();
}

/**
 * @brief Set if a (radio) element is found locally
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param local True if it is a local file
 */
void AudioEditor::setLocal(QString element, int type, bool local)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || (type != 2)) return;

    qDebug() << "AudioEditor: Set radio" << element << "local:" << local << "...";

    auto e = m_currentProject->currentCategory()->currentScenario()->radioElement(element);
    e->setLocal(local);
    m_local = local;

    emit currentElementChanged();
    madeChanges();
}

/**
 * @brief Set the (spotify) ID of a (spotify) element
 * @param element Name of the element
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @param id New ID
 */
void AudioEditor::setId(QString element, int type, QString id)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario() || (type != 3)) return;

    qDebug() << "AudioEditor: Set id of spotify element" << element << ":" << id << "...";

    auto e = m_currentProject->currentCategory()->currentScenario()->spotifyElement(element);
    e->setId(id);
    m_id = id;

    emit currentElementChanged();
    madeChanges();
}

/**
 * @brief Get the base path where files are located in
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
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
 * @param type Music: 0, Sounds: 1, Radio: 2, Spotify: 3
 * @return Pointer to the requested element, nullptr if element is not found
 */
AudioElement * AudioEditor::getElement(QString name, int type)
{
    if (!m_currentProject || !m_currentProject->currentCategory() ||
        !m_currentProject->currentCategory()->currentScenario()) return nullptr;

    const auto scenario = m_currentProject->currentCategory()->currentScenario();

    switch (type)
    {
    case 0: // Music
        return scenario->musicElement(name);

    case 1: // Sound
        return scenario->soundElement(name);

    case 2: // Radio
        return scenario->radioElement(name);

    case 3: // Spotify
        return scenario->spotifyElement(name);

    default: return nullptr;
    }
}
