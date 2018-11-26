#include "audioeditor.h"
#include "src/functions.h"

#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFile>

AudioEditor::AudioEditor(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Editor ...";

    sManager = new SettingsManager;
}

QString AudioEditor::getResourcesPath()
{
    return sManager->getSetting(Setting::resourcesPath);
}

void AudioEditor::updateProjectList()
{
    QString path = sManager->getSetting(Setting::audioPath);

    m_projectList.clear();

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".audio"))
        {
            m_projectList.append(file.replace(".audio", ""));
        }
    }

    emit projectListChanged();
}

void AudioEditor::setCurrentProject(QString project)
{
    m_currentProject = project;

    m_scenarioList.clear();
    m_musicLists.clear();
    m_soundLists.clear();
    m_radios.clear();
    m_spotifyPlaylists.clear();

    emit scenarioListChanged();
    emit elementListChanged();

    updateCategoryList();
}

void AudioEditor::createProject(QString project)
{
    if (project != "")
    {
        QString path = sManager->getSetting(Setting::audioPath) + "/" + project + ".audio";

        if (!QFile(path).exists())
        {
            QSettings settings(path, QSettings::IniFormat);
            settings.setValue("version",           1);
            settings.setValue("ProjectName", project);

            m_projectList.append(project);

            projectListChanged();
        }
    }
}

void AudioEditor::updateCategoryList()
{
    m_categoryList.clear();

    if (m_currentProject != "")
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        m_categoryList = settings.value("categories").toStringList();
    }

    emit categoryListChanged();
}

void AudioEditor::setCurrentCategory(QString category)
{
    m_currentCategory = category;

    updateScenarioList();
}

void AudioEditor::createCategory(QString category)
{
    if ((category != "") && (m_currentProject != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        if (!m_categoryList.contains(category))
        {
            m_categoryList.append(category);

            settings.setValue("categories", m_categoryList);

            emit categoryListChanged();
        }
    }
}

void AudioEditor::updateScenarioList()
{
    m_scenarioList.clear();

    if ((m_currentProject != "") && (m_currentCategory != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(m_currentCategory);

        m_scenarioList = settings.value("scenarios").toStringList();

        settings.endGroup();
    }

    emit scenarioListChanged();
}

void AudioEditor::setCurrentScenario(QString scenario)
{
    m_currentScenario = scenario;

    updateElementList();
}

void AudioEditor::createScenario(QString scenario)
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (scenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        if (!m_scenarioList.contains(scenario))
        {
            settings.beginGroup(m_currentCategory);

            m_scenarioList.append(scenario);
            settings.setValue("scenarios", m_scenarioList);

            settings.endGroup();

            emit scenarioListChanged();
        }
    }
}

void AudioEditor::updateElementList()
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(m_currentCategory);

        m_musicLists       = settings.value(m_currentScenario + "_music").toStringList();
        m_soundLists       = settings.value(m_currentScenario + "_sounds").toStringList();
        m_radios           = settings.value(m_currentScenario + "_radios").toStringList();
        m_spotifyPlaylists = settings.value(m_currentScenario + "_spotify").toStringList();

        settings.endGroup();

        emit elementListChanged();
    }
}

void AudioEditor::sortElements()
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(m_currentCategory);

        m_musicLists       = settings.value(m_currentScenario + "_music").toStringList();
        m_soundLists       = settings.value(m_currentScenario + "_sounds").toStringList();
        m_radios           = settings.value(m_currentScenario + "_radios").toStringList();
        m_spotifyPlaylists = settings.value(m_currentScenario + "_spotify").toStringList();

        m_musicLists.sort();
        m_soundLists.sort();
        m_radios.sort();
        m_spotifyPlaylists.sort();

        settings.setValue(m_currentScenario + "_music",         m_musicLists);
        settings.setValue(m_currentScenario + "_sounds",        m_soundLists);
        settings.setValue(m_currentScenario + "_radios",            m_radios);
        settings.setValue(m_currentScenario + "_spotify", m_spotifyPlaylists);

        settings.endGroup();

        emit elementListChanged();
    }
}

// Move element in list
void AudioEditor::moveElement(QString element, int type, int positions)
{
    QStringList *elements;

    switch (type) {
    case 0:
        elements = &m_musicLists;
        break;

    case 1:
        elements = &m_soundLists;
        break;

    case 2:
        elements = &m_radios;
        break;

    case 3:
        elements = &m_spotifyPlaylists;
        break;
    }

    int index = elements->indexOf(element);

    if ((index + positions >= 0) && (index + positions <= elements->size()))
    {
        elements->removeAt(index);
        elements->insert(index + positions, element);

        emit elementListChanged();
    }
}

void AudioEditor::createList(QString listName, int type, QStringList args)
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != "") && (listName != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        QString suffix1;
        QString suffix2;
        QStringList *list;

        switch (type) {
        case 0: // Music List
            suffix1 = "_music";
            suffix2 = suffix1;
            list    = &m_musicLists;
            break;

        case 1: // Sound List
            suffix1 = "_sounds";
            suffix2 = suffix1;
            list    = &m_soundLists;
            break;

        case 2: // Radio
            suffix1 = "_radios";
            suffix2 = "_radio";
            list    = &m_radios;
            break;

        case 3: // Spotify
            suffix1 = "_spotify";
            suffix2 = suffix1;
            list    = &m_spotifyPlaylists;
        }

        if (!list->contains(listName))
        {
            list->append(listName);

            settings.beginGroup(m_currentCategory);
            settings.setValue(m_currentScenario + suffix1, *list);
            settings.endGroup();

            settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + listName + suffix2);
            settings.setValue("name", listName);

            if ((type == 3) && (args.size() > 0))
            {
                settings.setValue("id", args[0]);
            }

            settings.endGroup();

            emit elementListChanged();
        }
    }
}

void AudioEditor::saveProject()
{
    if ((m_currentProject != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.setValue("categories", m_categoryList);

        if ((m_currentCategory != "") && (m_currentScenario != ""))
        {
            settings.beginGroup(m_currentCategory);

            settings.setValue(m_currentScenario + "_music",         m_musicLists);
            settings.setValue(m_currentScenario + "_sounds",        m_soundLists);
            settings.setValue(m_currentScenario + "_radios",            m_radios);
            settings.setValue(m_currentScenario + "_spotify", m_spotifyPlaylists);

            settings.endGroup();
        }
    }
}

void AudioEditor::setCurrentList(QString list, int type)
{
    m_currentFileNames.clear();
    m_currentFilePaths.clear();
    m_currentFileMissing.clear();

    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != "") && (list != ""))
    {
        QString suffix;
        QString fileArrayName;
        QString fileArrayFile;

        switch (type) {
        case 0: // Music List
            suffix            = "_music";
            fileArrayName     = "songs";
            fileArrayFile     = "song";
            m_currentBasePath = sManager->getSetting(Setting::musicPath);
            break;

        case 1: // Sound List
            suffix            = "_sounds";
            fileArrayName     = "sounds";
            fileArrayFile     = "sound";
            m_currentBasePath = sManager->getSetting(Setting::soundPath);
            break;

        case 2: // Radio
            suffix            = "_radio";
            fileArrayName     = "radios";
            fileArrayFile     = "radio";
            m_currentBasePath = sManager->getSetting(Setting::radioPath);
            break;

        case 3: // Spotify
            suffix        = "_spotify";
            fileArrayName = "playlists";
            fileArrayFile = "playlist";
        }

        m_currentList = list;

        QString   path = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + list + suffix);

        m_currentListMode = settings.value("mode", 0).toInt();

        QString iconPath = settings.value("icon").toString();
        m_currentListIcon = iconPath;

        m_local = settings.value("local", false).toBool();
        m_url   = settings.value("url", "").toString();

        m_spotifyID = settings.value("id", "").toString();

        // Add every list element
        int count = settings.beginReadArray(fileArrayName);

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            m_currentFileNames.append(settings.value(fileArrayFile, "UNKNOWN FILE").toStringList().at(0));
            m_currentFilePaths.append(settings.value(fileArrayFile, "UNKNOWN PATH").toStringList().at(1));
            m_currentFileMissing.append(!QFile(m_currentBasePath + "/" + settings.value(fileArrayFile, "UNKNOWN FILE").toStringList().at(1)).exists());
        }

        settings.endArray();
        settings.endGroup();

        emit listChanged();
    }
}

void AudioEditor::addFile(QString name, QString path)
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != "") && (m_currentList != ""))
    {
        m_currentFileNames.append(name);
        m_currentFilePaths.append(path);
        m_currentFileMissing.append(false);

        emit listChanged();
    }
}

void AudioEditor::addFiles(QStringList names, QStringList paths, QList<bool>missing)
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != "") && (m_currentList != ""))
    {
        m_currentFileNames.append(names);
        m_currentFilePaths.append(paths);

        if (!missing.isEmpty()) m_currentFileMissing.append(missing);

        emit listChanged();
    }
}

void AudioEditor::removeFile(int index)
{
    if ((m_currentProject != "") && (m_currentCategory != "") && (m_currentScenario != "") && (m_currentList != "") && (m_currentFileNames.size() > index))
    {
        m_currentFileNames.removeAt(index);
        m_currentFilePaths.removeAt(index);
        m_currentFileMissing.removeAt(index);
    }
}

void AudioEditor::saveList(int type)
{
    QString   projectPath = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
    QSettings settings(projectPath, QSettings::IniFormat);

    QString suffix;
    QString fileArrayName;
    QString fileArrayFile;

    switch (type) {
    case 0: // Music List
        suffix        = "_music";
        fileArrayName = "songs";
        fileArrayFile = "song";
        break;

    case 1: // Sound List
        suffix        = "_sounds";
        fileArrayName = "sounds";
        fileArrayFile = "sound";
        break;

    case 2: // Radio
        suffix        = "_radio";
        fileArrayName = "radios";
        fileArrayFile = "radio";
        break;

    case 3: // Spotify
        suffix        = "_spotify";
        fileArrayName = "playlists";
        fileArrayFile = "playlist";
    }

    settings.beginGroup(m_currentCategory + "_" + m_currentScenario + "_" + m_currentList + suffix);

    if (type == 3)
    {
        settings.setValue("id", m_spotifyID);
    }
    else if (type == 2)
    {
        settings.setValue("url",     m_url);
        settings.setValue("local", m_local);
    }
    else
    {
        settings.beginWriteArray(fileArrayName);

        for (int i = 0; i < m_currentFileNames.size(); i++)
        {
            settings.setArrayIndex(i);
            QStringList file = { m_currentFileNames.at(i), m_currentFilePaths.at(i) };
            settings.setValue(fileArrayFile, file);
        }

        settings.endArray();

        settings.setValue("mode", m_currentListMode);
    }

    settings.setValue("icon", m_currentListIcon);

    settings.endGroup();
}

void AudioEditor::deleteList(QString list, int type)
{
    QString suffix1;
    QString suffix2;
    QStringList *pList;

    switch (type) {
    case 0: // Music List
        suffix1 = "_music";
        suffix2 = suffix1;
        pList   = &m_musicLists;
        break;

    case 1: // Sound List
        suffix1 = "_sounds";
        suffix2 = suffix1;
        pList   = &m_soundLists;
        break;

    case 2: // Radio
        suffix1 = "_radios";
        suffix2 = "_radio";
        pList   = &m_radios;
        break;

    case 3: // Spotify
        suffix1 = "_spotify";
        suffix2 = suffix1;
        pList   = &m_spotifyPlaylists;
    }

    pList->removeAll(list);

    QString   projectPath = sManager->getSetting(Setting::audioPath) + "/" + m_currentProject + ".audio";
    QSettings settings(projectPath, QSettings::IniFormat);

    settings.beginGroup(m_currentCategory);
    settings.setValue(m_currentScenario + suffix1, *pList);
    settings.endGroup();

    settings.remove(m_currentCategory + "_" + m_currentScenario + "_" + list + suffix2);

    m_currentFileNames.clear();
    m_currentFilePaths.clear();
    m_currentFileMissing.clear();
    m_currentList.clear();

    updateElementList();

    emit listChanged();
}

// Move song in list
void AudioEditor::moveFile(int index, int positions)
{
    QString name = m_currentFileNames.at(index);

    m_currentFileNames.removeAt(index);
    m_currentFileNames.insert(index + positions, name);

    QString path = m_currentFilePaths.at(index);

    m_currentFilePaths.removeAt(index);
    m_currentFilePaths.insert(index + positions, path);

    bool missing = m_currentFileMissing.at(index);

    m_currentFileMissing.removeAt(index);
    m_currentFileMissing.insert(index + positions, missing);
}

void AudioEditor::removeMissingFiles()
{
    bool removedFiles = false;

    for (int i = 0; i < m_currentFilePaths.size(); i++)
    {
        if (m_currentFileMissing[i])
        {
            m_currentFileNames.removeAt(i);
            m_currentFilePaths.removeAt(i);
            m_currentFileMissing.removeAt(i);
            i--;
            removedFiles = true;
        }
    }

    if (removedFiles) emit listChanged();
}

void AudioEditor::setURL(QString url)
{
    m_url = url;

    if (m_local) emit urlChanged();
}

void AudioEditor::replaceMissingFolder(int index, QString folder, int type)
{
    QString path = folder.replace("file://", "");

    path = path.replace(m_currentBasePath, "") + "/";

    QString relPath = path + m_currentFileNames[index];
    m_currentFilePaths[index] = relPath;

    bool newFileExists = QFile(m_currentBasePath + relPath).exists();
    m_currentFileMissing[index] = !newFileExists;

    // If new path was correct, try to set this path for all other missing files
    // as well
    if (newFileExists)
    {
        for (int i = 0; i < m_currentFileNames.size(); i++)
        {
            if (m_currentFileMissing[i] && QFile(m_currentBasePath + path + m_currentFileNames[i]).exists())
            {
                m_currentFilePaths[i]   = path + m_currentFileNames[i];
                m_currentFileMissing[i] = false;
            }
        }
    }

    saveList(type);
    m_lastListIndex = index;
    emit listChanged();
}
