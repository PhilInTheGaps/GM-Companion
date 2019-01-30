#include "audiofilemanager.h"
#include "src/functions.h"
#include <QDebug>
#include <QSettings>
#include <QTemporaryFile>

AudioFileManager::AudioFileManager(GoogleDrive *google) : googleDrive(google)
{
    qDebug() << "Loading AudioFileManager ...";

    connect(this,   &AudioFileManager::projectsChanged,  this, &AudioFileManager::onProjectsChanged);

    connect(google, &GoogleDrive::receivedFolderContent, [ = ](int id, QList<GoogleFile>list) {
        qDebug() << "AudioFileManager:";
        qDebug() << "Request ID:" << id;
        qDebug() << "Radio Request ID:" << m_radioRequestID;
        qDebug() << "Project Request ID:" << m_projectsRequestID;

        if (id == m_projectsRequestID)
        {
            for (auto a : list)
            {
                if (a.fileName.endsWith(".audio"))
                {
                    m_projectFileRequestIDs.append(google->getFile(a.id));
                }
            }
        }
        else if (id == m_radioRequestID)
        {
            QString file = m_relativeRadioPath.right(m_relativeRadioPath.length() - m_relativeRadioPath.lastIndexOf('/') - 1);
            qDebug() << "AudioFileManager:" << file;

            for (auto a : list)
            {
                qDebug() << "AudioFileManager:" << a.fileName;

                if (a.fileName == file)
                {
                    m_radioRequestID = google->getFile(a.id);
                }
            }
        }
    });

    connect(google, &GoogleDrive::receivedFile, [ = ](int id, QByteArray data) {
        if (m_projectFileRequestIDs.contains(id))
        {
            QTemporaryFile f;
            f.setAutoRemove(false);

            if (f.open())
            {
                f.write(data);
                f.close();

                auto p = initProject(f.fileName(), 1);

                if (p)
                {
                    m_projects.append(p);
                    emit projectsChanged(m_projects);
                }
            }
        }
        else if (id == m_radioRequestID)
        {
            QTemporaryFile f;

            if (f.open())
            {
                f.write(data);
                f.close();

                emit radioPathChanged(QUrl::fromLocalFile(f.fileName()));
            }
        }
    });

    connect(google, static_cast<void (GoogleDrive::*)(QList<QUrl>, int, QStringList)>(&GoogleDrive::receivedFileUrls), this, &AudioFileManager::onMusicPathsChanged);
    connect(google, static_cast<void (GoogleDrive::*)(QList<QUrl>, int, QStringList)>(&GoogleDrive::receivedFileUrls), this, &AudioFileManager::onSoundPathsChanged);
    connect(google, static_cast<void (GoogleDrive::*)(QList<QUrl>, int, QStringList)>(&GoogleDrive::receivedFileUrls), this, &AudioFileManager::onRadioPathChanged);
}

/**
 * @brief Find audio project files
 * @param mode 0: Local, 1: Google Drive
 */
void AudioFileManager::findProjects(int mode)
{
    qDebug() << "AudioFileManager: Finding audio projects for audio tool ...";
    m_mode = mode;

    switch (mode)
    {
    case 0: // LOCAL

        for (QString s : getFiles(sManager.getSetting(Setting::audioPath)))
        {
            if (s.endsWith(".audio"))
            {
                auto p = initProject(sManager.getSetting(Setting::audioPath) + "/" + s, mode);

                if (p) m_projects.append(p);
            }
        }

        emit projectsChanged(m_projects);
        break;

    case 1: // Google Drive
        m_projectsRequestID = googleDrive->getFolderContent(sManager.getSetting(Setting::googlePath, "audio"));
        qDebug() << "AudioFileManager:" << m_projectsRequestID;
        break;
    }
}

/**
 * @brief Finds audio projects for AudioEditor
 * @param mode Currently not used, editor is only local for now
 */
void AudioFileManager::findEditorProjects(int mode)
{
    qDebug() << "AudioFileManager: Finding audio projects for audio editor ...";
    m_mode = mode;

    switch (mode)
    {
    default: // Only local for now, might change later

        for (QString s : getFiles(sManager.getSetting(Setting::audioPath)))
        {
            if (s.endsWith(".audio")) m_editorProjects.append(initProject(sManager.getSetting(Setting::audioPath) + "/" + s, mode));
        }

        emit editorProjectsChanged(m_editorProjects);
        break;
    }
}

/**
 * @brief Get the full path to an icon
 * @param icon Relative icon path
 * @param mode 0: Local, 1: Google Drive
 * @return Full path / url to icon
 */
QString AudioFileManager::iconPath(QString icon)
{
    if (icon.isEmpty()) return "";

    switch (m_mode)
    {
    case 0: // LOCAL
        return sManager.getSetting(Setting::resourcesPath) + icon;

    default:
        m_iconMap[icon] = "";
        return "";
    }
}

/**
 * @brief Set new fetched icon paths for all elemets
 */
void AudioFileManager::updateIconPaths()
{
    switch (m_mode)
    {
    case 1: // Google Drive
    {
        QStringList paths = m_iconMap.uniqueKeys();
        googleDrive->getFileUrls(paths, 3);

        connect(googleDrive, static_cast<void (GoogleDrive::*)(QList<FilePath> *, int)>(&GoogleDrive::receivedFileUrls), [ = ](QList<FilePath> *files, int mode) {
                if (mode == 3)
                {
                    for (auto a : *files)
                    {
                        m_iconMap[a.fullPath] = a.webConentLink;
                    }

                    for (auto a : m_projects)
                    {
                        for (auto b : a->categories())
                        {
                            for (auto c : b->scenarios())
                            {
                                for (auto d : c->elements())
                                {
                                    if (!d->hasIcon()) d->setIcon(m_iconMap[d->relativeIcon()]);
                                }
                            }
                        }
                    }
                }
            });
        break;
    }

    default: return;
    }
}

/**
 * @brief Get music paths
 * @param List of paths to fetch
 */
void AudioFileManager::fetchMusicPaths(QStringList paths)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QList<QUrl> urls;

        for (auto s : paths)
        {
            urls.append(QUrl::fromLocalFile(sManager.getSetting(Setting::musicPath) + s));
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
 * @brief Send out new music paths when they change
 * @param urls List of URLs to the songs
 * @param For verification that the request was really for music paths
 * @param args Currently only for debugging
 */
void AudioFileManager::onMusicPathsChanged(QList<QUrl>urls, int mode, QStringList args)
{
    if (mode == 0)
    {
        qDebug() << args;
        emit songPathsChanged(urls);
    }
}

/**
 * @brief Get the URLs to some sound files
 * @param paths List of sound file paths
 * @param args
 */
void AudioFileManager::fetchSoundPaths(QStringList paths, QStringList args)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QList<QUrl> urls;

        for (auto s : paths)
        {
            urls.append(QUrl::fromLocalFile(sManager.getSetting(Setting::soundPath) + s));
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
 * @brief Send out new sound paths when they change
 * @param urls List of URLs to sound files
 * @param mode For verification that request was for sound files
 * @param args Additional arguments
 */
void AudioFileManager::onSoundPathsChanged(QList<QUrl>urls, int mode, QStringList args)
{
    if (mode == 1)
    {
        emit soundPathsChanged(urls, args);
    }
}

/**
 * @brief Get the URLs of a radio playlist
 * @param path Path to the playlist
 */
void AudioFileManager::fetchRadioPath(QString path)
{
    switch (m_mode)
    {
    case 0: // Local
    {
        QUrl url = QUrl::fromLocalFile(sManager.getSetting(Setting::radioPath) + path);
        emit radioPathChanged(url);
        break;
    }

    case 1: // Google Drive
    {
        //        googleDrive->getFileUrls({ path }, 2);
        m_relativeRadioPath = path;
        m_radioRequestID    = googleDrive->getFolderContent(sManager.getSetting(Setting::googlePath, "radio"));
        break;
    }

    default: return;
    }
}

/**
 * @brief Send out new URL of the playlist
 * @param urls Should only contain one url, points to the playlist
 * @param mode For verification that request was for a radio playlist
 * @param args Additional arguments, for debugging
 */
void AudioFileManager::onRadioPathChanged(QList<QUrl>urls, int mode, QStringList args)
{
    if ((mode == 2) && (urls.size() > 0))
    {
        qDebug() << args;
        emit radioPathChanged(urls[0]);
    }
}

/**
 * @brief Initialize / load an audio project
 * @param path Path of project file
 * @return Pointer to an audio project
 */
AudioProject * AudioFileManager::initProject(QString path, int mode)
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
        qDebug() << "AudioFileManager Error: Could not read file" << path << "!";
        return nullptr;
    }
}

/**
 * @brief Create an initialize a project category
 * @param Name of the category
 * @param settings Pointer to QSettings instance used for this project
 * @return Pointer to audio category
 */
AudioCategory * AudioFileManager::initCategory(QJsonObject object)
{
    qDebug() << "AudioFileManager: Initializing category" << object.value("name").toString() << "...";
    QList<AudioScenario *> scenarios;

    auto scenarioArray = object.value("scenarios").toArray();
    auto name          = object.value("name").toString();

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
AudioScenario * AudioFileManager::initScenario(QJsonObject object)
{
    auto name = object.value("name").toString();

    qDebug() << "AudioFileManager: Initializing scenario" << name << "...";

    auto musicArray   = object.value("music_elements").toArray();
    auto soundArray   = object.value("sound_elements").toArray();
    auto radioArray   = object.value("radio_elements").toArray();
    auto spotifyArray = object.value("spotify_elements").toArray();

    return new AudioScenario(name, initMusicLists(musicArray), initSoundLists(soundArray), initRadios(radioArray), initSpotifyElements(spotifyArray));
}

/**
 * @brief Create and initialize all music list elements in a scenario
 * @param category Name of the category
 * @param scenario Name of the scenario
 * @param names List of the music list names
 * @param settings Pointer to QSettings instance used for this project
 * @return List with pointers to music list elements
 */
QList<MusicElement *>AudioFileManager::initMusicLists(QJsonArray array)
{
    qDebug() << "AudioFileManager: Initializing MusicLists ...";

    QList<MusicElement *> list;

    for (auto e : array)
    {
        auto o    = e.toObject();
        auto name = o.value("name").toString();

        MusicElement *element = new MusicElement(name);
        auto relativeIcon     = o.value("icon").toString();
        element->setIcon(iconPath(relativeIcon));
        element->setRelativeIcon(relativeIcon);
        element->setMode(o.value("mode").toInt());

        QStringList files;
        auto fileArray = o.value("files").toArray();

        for (auto f : fileArray)
        {
            files.append(f.toString());
        }

        element->setFiles(files);
        list.append(element);

        qDebug() << "   " << name;
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
QList<SoundElement *>AudioFileManager::initSoundLists(QJsonArray array)
{
    qDebug() << "AudioFileManager: Initializing SoundLists ...";

    QList<SoundElement *> list;

    for (auto e : array)
    {
        auto o    = e.toObject();
        auto name = o.value("name").toString();

        SoundElement *element = new SoundElement(name);
        auto relativeIcon     = o.value("icon").toString();
        element->setIcon(iconPath(relativeIcon));
        element->setRelativeIcon(relativeIcon);
        element->setMode(o.value("mode").toInt());

        QStringList files;
        auto fileArray = o.value("files").toArray();

        for (auto f : fileArray)
        {
            files.append(f.toString());
        }

        element->setFiles(files);
        list.append(element);

        qDebug() << "   " << name;
    }

    qDebug() << "AudioFileManager: Done. SoundList count:" << list.size();

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
QList<RadioElement *>AudioFileManager::initRadios(QJsonArray array)
{
    qDebug() << "AudioFileManager: Initializing Radios ...";

    QList<RadioElement *> list;

    for (auto e : array)
    {
        auto o    = e.toObject();
        auto name = o.value("name").toString();

        RadioElement *element = new RadioElement(name, o.value("local").toBool());

        QString relativeIcon = o.value("icon").toString();
        element->setIcon(iconPath(relativeIcon));
        element->setRelativeIcon(relativeIcon);
        element->setUrl(o.value("url").toString());

        list.append(element);
    }

    qDebug() << "AudioFileManager: Done. Radio count:" << list.size();

    return list;
}

/**
 * @brief Create and initialize all spotify elements of a scenario
 * @param category Name of the category
 * @param scenario Name of the scenario
 * @param names List of spotify element names
 * @param settings Pointer to QSettings instance used for this project
 * @return List of pointers to spotify elements
 */
QList<SpotifyElement *>AudioFileManager::initSpotifyElements(QJsonArray array)
{
    qDebug() << "AudioFileManager: Initializing SpotifyElements ...";

    QList<SpotifyElement *> list;

    for (auto e : array)
    {
        auto o    = e.toObject();
        auto name = o.value("name").toString();

        SpotifyElement *element = new SpotifyElement(name, o.value("id").toString());
        QString relativeIcon    = o.value("icon").toString();
        element->setIcon(iconPath(relativeIcon));
        element->setRelativeIcon(relativeIcon);

        list.append(element);
    }

    qDebug() << "AudioFileManager: Done. SpotifyElement count:" << list.size();

    return list;
}

void AudioFileManager::findMissingFiles(QStringList files, QString basePath)
{
    qDebug() << "AudioFileManager: Finding missing files ...";
    FileFinder *finder = new FileFinder(files, basePath);

    finder->moveToThread(&workerThread);

    connect(this,   &AudioFileManager::startFindingMissingFiles, finder, &FileFinder::findMissingFiles);
    connect(finder, &FileFinder::finished,                       this,   [ = ](QList<bool>missing) { missingFilesFound(missing); delete finder; });
    workerThread.start();

    emit startFindingMissingFiles();
}

/**
 * @brief Find out if there are files missing in the file list
 */
void FileFinder::findMissingFiles()
{
    qDebug() << "Started FileFinder!";
    QList<bool> missingFiles;

    for (QString s : m_files)
    {
        if (QFile(m_basePath + s).exists())
        {
            missingFiles.append(false);
        }
        else
        {
            missingFiles.append(true);
        }
    }

    emit finished(missingFiles);
}

/**
 * @brief Save an AudioProject to disc
 * @param project Pointer to AudioProject to save
 */
void AudioFileManager::saveProject(AudioProject *project)
{
    if (!project) return;

    qDebug() << "AudioFileManager: Saving project:" << project->name() << "...";

    if (!QFile(sManager.getSetting(Setting::audioPath) + "/" + project->name() + ".audio").exists())
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

    qDebug() << "    Done.";
    project->setSaved(true);

    QFile f(sManager.getSetting(Setting::audioPath) + "/" + project->name() + ".audio");

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
    }
}

/**
 * @brief Save a category to a JsonObject
 * @param category Pointer to AudioCategory to save
 * @return QJsonObject with saved data
 */
QJsonObject AudioFileManager::saveCategory(AudioCategory *category)
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
QJsonObject AudioFileManager::saveScenario(AudioScenario *scenario)
{
    QJsonObject object;

    if (scenario)
    {
        object.insert("name",             scenario->name());
        object.insert("music_elements",   saveMusicElements(scenario));
        object.insert("sound_elements",   saveSoundElements(scenario));
        object.insert("radio_elements",   saveRadioElements(scenario));
        object.insert("spotify_elements", saveSpotifyElements(scenario));
    }

    return object;
}

/**
 * @brief Save all music elements to JsonObjects
 * @param scenario Scenario of music elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioFileManager::saveMusicElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto e : scenario->musicElements())
    {
        QJsonObject object;
        object.insert("name", e->name());
        object.insert("icon", e->relativeIcon());
        object.insert("mode", e->mode());

        // Files
        QJsonArray files;

        for (auto f : e->files())
        {
            files.append(f);
        }
        object.insert("files", files);

        array.append(object);
    }

    return array;
}

/**
 * @brief Save all sound elements to JsonObjects
 * @param scenario Scenario of sound elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioFileManager::saveSoundElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto e : scenario->soundElements())
    {
        QJsonObject object;
        object.insert("name", e->name());
        object.insert("icon", e->relativeIcon());
        object.insert("mode", e->mode());

        // Files
        QJsonArray files;

        for (auto f : e->files())
        {
            files.append(f);
        }
        object.insert("files", files);

        array.append(object);
    }

    return array;
}

/**
 * @brief Save all radio elements to JsonObjects
 * @param scenario Scenario of radio elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioFileManager::saveRadioElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto e : scenario->radioElements())
    {
        QJsonObject object;
        object.insert("name",  e->name());
        object.insert("icon",  e->relativeIcon());
        object.insert("local", e->local());
        object.insert("url",   e->url().toString());
        array.append(object);
    }

    return array;
}

/**
 * @brief Save all spotify elements to JsonObjects
 * @param scenario Scenario of spotify elements to be saved
 * @return Array of QJsonObjects with saved data
 */
QJsonArray AudioFileManager::saveSpotifyElements(AudioScenario *scenario)
{
    QJsonArray array;

    for (auto e : scenario->spotifyElements())
    {
        QJsonObject object;
        object.insert("name", e->name());
        object.insert("icon", e->relativeIcon());
        object.insert("id",   e->id());
        array.append(object);
    }

    return array;
}
