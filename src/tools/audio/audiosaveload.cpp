#include "audiosaveload.h"
#include "functions.h"
#include <QDebug>
#include <QSettings>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QImage>
#include <QBuffer>
#include <QPainter>

#include "filesource/localaudiofilesource.h"

AudioSaveLoad::AudioSaveLoad(int mode, GoogleDrive *google)
    : googleDrive(google), m_mode(mode)
{
    qDebug() << "Loading AudioSaveLoad ...";

    localSource = new LocalAudioFileSource(this);

    connect(this,        &AudioSaveLoad::foundProjects,         this, &AudioSaveLoad::onFoundProjects);

    connect(localSource, &LocalAudioFileSource::foundProjects,  this, &AudioSaveLoad::foundProjects);
    connect(localSource, &LocalAudioFileSource::foundIconPaths, this, &AudioSaveLoad::onFoundIconPaths);
}

AudioSaveLoad::~AudioSaveLoad()
{
    workerThread.quit();
    workerThread.wait();

    localSource->deleteLater();
}

/**
 * @brief Find audio project files
 * @param mode 0: Local, 1: Google Drive
 */
void AudioSaveLoad::findProjects(int mode)
{
    qDebug() << "AudioSaveLoad: Finding audio projects for audio tool ...";
    m_mode = mode;

    switch (mode)
    {
    case 0: // LOCAL
        localSource->findProjects();
        break;

    case 1: // Google Drive
        // TODO
        break;
    }
}

/**
 * @brief Finds audio projects for AudioEditor
 * @param mode Currently not used, editor is only local for now
 */
void AudioSaveLoad::findEditorProjects(int mode)
{
    qDebug() << "AudioSaveLoad: Finding audio projects for audio editor ...";
    m_mode = mode;

    switch (mode)
    {
    default: // Only local for now, might change later
        localSource->findProjects(true);
        break;
    }
}

void AudioSaveLoad::findIconPaths(QList<AudioProject *>projects, bool editor)
{
    QStringList icons;

    qDebug() << "Finding file icon paths ..." << projects.length();

    for (auto project : projects)
    {
        if (!project) continue;

        for (auto element : project->elements())
        {
            if (element && !icons.contains(element->relativeIcon()))
            {
                icons.append(element->relativeIcon());
            }
        }
    }

    findIconPaths(icons, editor);
}

void AudioSaveLoad::findIconPaths(QStringList icons, bool editor)
{
    switch (m_mode)
    {
    case 0: // LOCAL
        localSource->findIconPaths(icons, editor);
        break;

    default:

        // TODO
        break;
    }
}

/**
 * @brief Get music paths
 * @param List of paths to fetch
 */
void AudioSaveLoad::fetchMusicPaths(QStringList paths)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QList<QUrl> urls;

        for (auto s : paths)
        {
            urls.append(QUrl::fromLocalFile(SettingsManager::getPath("musicPath") + s));
        }

        emit songPathsChanged(urls);
        break;
    }

    case 1: // Google Drive
    {
        googleDrive->getFileUrls(paths, 0);
        break;
    }

    default: return;
    }
}

/**
 * @brief Get the URLs to some sound files
 * @param paths List of sound file paths
 * @param args
 */
void AudioSaveLoad::fetchSoundPaths(QStringList paths, QStringList args)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QList<QUrl> urls;

        for (auto s : paths)
        {
            urls.append(QUrl::fromLocalFile(SettingsManager::getPath("soundPath") + s));
        }

        emit soundPathsChanged(urls, args);
        break;
    }

    case 1: // Google Drive
    {
        googleDrive->getFileUrls(paths, 1, args);
        break;
    }

    default: return;
    }
}

/**
 * @brief Get the URLs of a radio playlist
 * @param path Path to the playlist
 */
void AudioSaveLoad::fetchRadioPath(QString path)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QUrl url = QUrl::fromLocalFile(SettingsManager::getPath("radioPath") + path);
        emit radioPathChanged(url);
        break;
    }

    case 1: // Google Drive
    {
        //        googleDrive->getFileUrls({ path }, 2);
        m_relativeRadioPath = path;
        m_radioRequestID    = googleDrive->getFolderContent(SettingsManager::getSetting("radio", "", "Google"));
        break;
    }

    default: return;
    }
}

/**
 * @brief Initialize / load an audio project
 * @param path Path of project file
 * @return Pointer to an audio project
 */
AudioProject * AudioSaveLoad::initProject(QString path)
{
    QFile f(path);

    if (f.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        f.close();

        QJsonObject root = doc.object();
        QString     name = root.value("name").toString();

        if (name.isEmpty()) return nullptr;

        int version = root.value("version").toInt();
        QList<AudioCategory *> categories;

        for (auto c : root.value("categories").toArray())
        {
            categories.append(initCategory(c.toObject()));
        }

        return new AudioProject(name, version, categories);
    }
    else
    {
        qDebug() << "AudioSaveLoad Error: Could not read file" << path << "!";
        return nullptr;
    }
}

/**
 * @brief Create an initialize a project category
 * @param Name of the category
 * @param settings Pointer to QSettings instance used for this project
 * @return Pointer to audio category
 */
AudioCategory * AudioSaveLoad::initCategory(QJsonObject object)
{
    qDebug() << "AudioSaveLoad: Initializing category" << object.value("name").toString() << "...";
    QList<AudioScenario *> scenarios;

    auto scenarioArray = object.value("scenarios").toArray();
    auto name          = object.value("name").toString();

    if (name.isEmpty()) return nullptr;

    for (auto s : scenarioArray)
    {
        scenarios.append(initScenario(s.toObject()));
    }

    return new AudioCategory(name, scenarios);
}

/**
 * @brief Create and initialize an audio scenario
 * @param category Name of the category this scenario belongs to
 * @param name Name of the scenario
 * @param settings Pointer to QSettings instance used for the project
 * @return Pointer to audio scenario
 */
AudioScenario * AudioSaveLoad::initScenario(QJsonObject object)
{
    auto name = object.value("name").toString();

    qDebug() << "AudioSaveLoad: Initializing scenario" << name << "...";

    auto musicArray = object.value("music_elements").toArray();
    auto soundArray = object.value("sound_elements").toArray();
    auto radioArray = object.value("radio_elements").toArray();

    auto scenarioArray = object.value("scenarios").toArray();
    QList<AudioScenario *> scenarios;

    for (auto s : scenarioArray)
    {
        scenarios.append(initScenario(s.toObject()));
    }

    return new AudioScenario(name, initMusicLists(musicArray), initSoundLists(soundArray), initRadios(radioArray), scenarios);
}

/**
 * @brief Create and initialize all music list elements in a scenario
 * @param category Name of the category
 * @param scenario Name of the scenario
 * @param names List of the music list names
 * @param settings Pointer to QSettings instance used for this project
 * @return List with pointers to music list elements
 */
QList<MusicElement *>AudioSaveLoad::initMusicLists(QJsonArray array)
{
    qDebug() << "AudioSaveLoad: Initializing MusicLists ...";

    QList<MusicElement *> list;

    for (auto e : array)
    {
        auto element = new MusicElement(e.toObject().value("name").toString());
        initAudioElement(element, e.toObject());
        list.append(element);
    }

    qDebug() << "Done. MusicList count:" << list.size();

    return list;
}

/**
 * @brief Create and initialize all sound lists in a scenario
 * @param category Name of the category
 * @param scenario Name of the scenario
 * @param names List of sound list names
 * @param settings Pointer to QSettings instance used for this project
 * @return List of pointers to sound list elements
 */
QList<SoundElement *>AudioSaveLoad::initSoundLists(QJsonArray array)
{
    qDebug() << "AudioSaveLoad: Initializing SoundLists ...";

    QList<SoundElement *> list;

    for (auto e : array)
    {
        auto element = new SoundElement(e.toObject().value("name").toString());
        initAudioElement(element, e.toObject());
        list.append(element);
    }

    qDebug() << "AudioSaveLoad: Done. SoundList count:" << list.size();

    return list;
}

/**
 * @brief Create and initialize all radios in a scenario
 * @param category Name of the category
 * @param scenario Name of the scenario
 * @param names List of radio names
 * @param settings Pointer to QSettings instance used for this project
 * @return List of pointers to radio elements
 */
QList<RadioElement *>AudioSaveLoad::initRadios(QJsonArray array)
{
    qDebug() << "AudioSaveLoad: Initializing Radios ...";

    QList<RadioElement *> list;

    for (auto e : array)
    {
        auto element = new RadioElement(e.toObject().value("name").toString());
        initAudioElement(element, e.toObject());
        list.append(element);
    }

    qDebug() << "AudioSaveLoad: Done. Radio count:" << list.size();

    return list;
}

void AudioSaveLoad::initAudioElement(AudioElement *element, QJsonObject object)
{
    auto name         = object.value("name").toString();
    auto relativeIcon = object.value("icon").toString();

    element->setRelativeIcon(relativeIcon);
    element->setMode(object.value("mode").toInt());

    QList<AudioFile *> files;
    auto fileArray = object.value("files").toArray();

    for (auto file : fileArray)
    {
        AudioFile *audioFile = new AudioFile(file.toObject().value("url").toString(), file.toObject().value("source").toInt());
        files.append(audioFile);
    }

    element->setFiles(files);
    qDebug() << "   " << name;
}

void AudioSaveLoad::findMissingFiles(QList<AudioFile *>files, QString basePath)
{
    qDebug() << "AudioSaveLoad: Finding missing files ...";

    FileFinder *finder = new FileFinder(files, basePath);

    finder->moveToThread(&workerThread);

    connect(this,   &AudioSaveLoad::startFindingMissingFiles, finder, &FileFinder::findMissingFiles);
    connect(finder, &FileFinder::finished,                    this,   [ = ]() {
        disconnect(this, &AudioSaveLoad::startFindingMissingFiles, finder, &FileFinder::findMissingFiles);

        if (finder) delete finder;
    });
    workerThread.start();

    emit startFindingMissingFiles();
}

/**
 * @brief Find out if there are files missing in the file list
 */
void FileFinder::findMissingFiles()
{
    qDebug() << "Started FileFinder!" << m_files.count();

    for (auto object : m_files)
    {
        if (!object) continue;
        auto file = static_cast<AudioFile *>(object);

        if (file->source() != 0)
        {
            file->setMissing(false);
        }
        else
        {
            file->setMissing(!QFile(m_basePath + file->url()).exists());
        }
    }

    emit finished();
}

/**
 * @brief Save an AudioProject to disc
 * @param project Pointer to AudioProject to save
 */
void AudioSaveLoad::saveProject(AudioProject *project)
{
    if (!project) return;

    qDebug() << "AudioSaveLoad: Saving project:" << project->name() << "...";

    if (!QFile(SettingsManager::getPath("audioPath") + "/" + project->name() + ".audio").exists())
    {
        qDebug() << "    Project does not yet exist on disk.";
        project->setSaved(false);
    }

    if (project->isSaved())
    {
        qDebug() << "    Project does not need to be saved, no changes were made.";
        return;
    }

    // Save basic info
    QJsonObject root;
    root.insert("name",    project->name());
    root.insert("version", project->version());

    // Save Categories
    QJsonArray categories;

    for (auto c : project->categories())
    {
        categories.append(saveCategory(c));
    }

    root.insert("categories", categories);

    // Write to disc
    QJsonDocument doc(root);

    QFile f(SettingsManager::getPath("audioPath") + "/" + project->name() + ".audio");

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();

        if (project->wasRenamed())
        {
            QFile f2(SettingsManager::getPath("audioPath") + "/" + project->oldName() + ".audio");
            QFileInfo f2i(f2);

            // Safety clause, don't delete file if a project was renamed and
            // one with the old name was created
            if (f2.exists() && (f2i.lastModified().secsTo(QDateTime::currentDateTime()) > 5))
            {
                if (!f2.remove())
                {
                    qDebug() << "AudioSaveLoad: Could not delete old project file.";
                }
            }

            project->setWasRenamed(false);
        }

        project->setSaved(true);
        qDebug() << "    Done.";
    }
    else
    {
        qDebug() << "AudioSaveLoad: Error: Could not save project to disk.";
    }
}

void AudioSaveLoad::deleteProject(AudioProject *project)
{
    if (!project) return;

    switch (m_mode)
    {
    case 0:
        localSource->deleteProject(project);
        break;

    default:
        qWarning() << "Error: deleteProject() for mode" << m_mode << "not implemented.";
    }
}

/**
 * @brief Save a category to a JsonObject
 * @param category Pointer to AudioCategory to save
 * @return QJsonObject with saved data
 */
QJsonObject AudioSaveLoad::saveCategory(AudioCategory *category)
{
    QJsonObject object;

    if (category)
    {
        object.insert("name", category->name());

        QJsonArray scenarios;

        for (auto s : category->scenarios())
        {
            scenarios.append(saveScenario(s));
        }

        object.insert("scenarios", scenarios);
    }

    return object;
}

/**
 * @brief Save a scenario to a JsonObject
 * @param scenario AudioScenario to save
 * @return QJsonObject with saved data
 */
QJsonObject AudioSaveLoad::saveScenario(AudioScenario *scenario)
{
    QJsonObject object;

    if (scenario)
    {
        object.insert("name",           scenario->name());
        object.insert("music_elements", saveMusicElements(scenario));
        object.insert("sound_elements", saveSoundElements(scenario));
        object.insert("radio_elements", saveRadioElements(scenario));

        QJsonArray a;

        for (auto s : scenario->scenarios()) a.append(saveScenario(s));

        object.insert("scenarios", a);
    }

    return object;
}

/**
 * @brief Save all music elements to JsonObjects
 * @param scenario Scenario of music elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioSaveLoad::saveMusicElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto element : scenario->musicElements())
    {
        array.append(saveAudioElement(element));
    }

    return array;
}

/**
 * @brief Save all sound elements to JsonObjects
 * @param scenario Scenario of sound elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioSaveLoad::saveSoundElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto element : scenario->soundElements())
    {
        array.append(saveAudioElement(element));
    }

    return array;
}

/**
 * @brief Save all radio elements to JsonObjects
 * @param scenario Scenario of radio elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioSaveLoad::saveRadioElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto element : scenario->radioElements())
    {
        array.append(saveAudioElement(element));
    }

    return array;
}

QJsonObject AudioSaveLoad::saveAudioElement(AudioElement *element)
{
    QJsonObject object;

    object.insert("name", element->name());
    object.insert("icon", element->relativeIcon());
    object.insert("mode", element->mode());

    // Files
    QJsonArray files;

    for (auto f : element->files())
    {
        files.append(QJsonObject{
            { "url", f->url() },
            { "source", f->source() }
        });
    }
    object.insert("files", files);

    return object;
}

void AudioSaveLoad::onFoundProjects(QList<AudioProject *>projects, bool forEditor)
{
    if (forEditor)
    {
        for (auto project : m_editorProjects) project->deleteLater();

        m_editorProjects = projects;
    }
    else
    {
        for (auto project : m_projects) project->deleteLater();

        m_projects = projects;
    }
}

void AudioSaveLoad::onFoundIconPaths(QMap<QString, QString>iconMap, bool editor)
{
    for (auto project : editor ? m_editorProjects : m_projects)
    {
        for (auto element : project->elements())
        {
            if (element && iconMap.contains(element->relativeIcon()))
            {
                element->icon()->setBackground(iconMap[element->relativeIcon()]);
            }
        }
    }

    iconGenerator.generateIcons(editor ? m_editorProjects : m_projects);
}
