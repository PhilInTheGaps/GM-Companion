#include "audioeditor.h"
#include "gm-companion/functions.h"

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

    l_projectList.clear();

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".audio"))
        {
            l_projectList.append(file.replace(".audio", ""));
        }
    }

    emit projectListChanged();
}

QStringList AudioEditor::getProjectList()
{
    return l_projectList;
}

void AudioEditor::setCurrentProject(QString project)
{
    l_currentProject = project;

    l_scenarioList.clear();
    l_musicLists.clear();
    l_soundLists.clear();
    l_radios.clear();

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

            l_projectList.append(project);

            projectListChanged();
        }
    }
}

void AudioEditor::updateCategoryList()
{
    l_categoryList.clear();

    if (l_currentProject != "")
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        l_categoryList = settings.value("categories").toStringList();
    }

    emit categoryListChanged();
}

QStringList AudioEditor::getCategoryList()
{
    return l_categoryList;
}

void AudioEditor::setCurrentCategory(QString category)
{
    l_currentCategory = category;

    updateScenarioList();
}

void AudioEditor::createCategory(QString category)
{
    if ((category != "") && (l_currentProject != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        if (!l_categoryList.contains(category))
        {
            l_categoryList.append(category);

            settings.setValue("categories", l_categoryList);

            emit categoryListChanged();
        }
    }
}

void AudioEditor::updateScenarioList()
{
    l_scenarioList.clear();

    if ((l_currentProject != "") && (l_currentCategory != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(l_currentCategory);

        l_scenarioList = settings.value("scenarios").toStringList();

        settings.endGroup();
    }

    emit scenarioListChanged();
}

QStringList AudioEditor::getScenarioList()
{
    return l_scenarioList;
}

void AudioEditor::setCurrentScenario(QString scenario)
{
    l_currentScenario = scenario;

    updateElementList();
}

void AudioEditor::createScenario(QString scenario)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (scenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        if (!l_scenarioList.contains(scenario))
        {
            settings.beginGroup(l_currentCategory);

            l_scenarioList.append(scenario);
            settings.setValue("scenarios", l_scenarioList);

            settings.endGroup();

            emit scenarioListChanged();
        }
    }
}

void AudioEditor::updateElementList()
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(l_currentCategory);

        l_musicLists       = settings.value(l_currentScenario + "_music").toStringList();
        l_soundLists       = settings.value(l_currentScenario + "_sounds").toStringList();
        l_radios           = settings.value(l_currentScenario + "_radios").toStringList();
        m_spotifyPlaylists = settings.value(l_currentScenario + "_spotify").toStringList();

        settings.endGroup();

        emit elementListChanged();
    }
}

void AudioEditor::sortElements()
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(l_currentCategory);

        l_musicLists       = settings.value(l_currentScenario + "_music").toStringList();
        l_soundLists       = settings.value(l_currentScenario + "_sounds").toStringList();
        l_radios           = settings.value(l_currentScenario + "_radios").toStringList();
        m_spotifyPlaylists = settings.value(l_currentScenario + "_spotify").toStringList();

        l_musicLists.sort();
        l_soundLists.sort();
        l_radios.sort();
        m_spotifyPlaylists.sort();

        settings.setValue(l_currentScenario + "_music",         l_musicLists);
        settings.setValue(l_currentScenario + "_sounds",        l_soundLists);
        settings.setValue(l_currentScenario + "_radios",            l_radios);
        settings.setValue(l_currentScenario + "_spotify", m_spotifyPlaylists);

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
        elements = &l_musicLists;
        break;

    case 1:
        elements = &l_soundLists;
        break;

    case 2:
        elements = &l_radios;
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

void AudioEditor::createList(QString listName, int type)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (listName != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        QString suffix1;
        QString suffix2;
        QStringList *list;

        switch (type) {
        case 0: // Music List
            suffix1 = "_music";
            suffix2 = suffix1;
            list    = &l_musicLists;
            break;

        case 1: // Sound List
            suffix1 = "_sounds";
            suffix2 = suffix1;
            list    = &l_soundLists;
            break;

        case 2: // Radio
            suffix1 = "_radios";
            suffix2 = "_radio";
            list    = &l_radios;
            break;

        case 3: // Spotify
            suffix1 = "_spotify";
            suffix2 = suffix1;
            list    = &m_spotifyPlaylists;
        }

        if (!list->contains(listName))
        {
            list->append(listName);

            settings.beginGroup(l_currentCategory);
            settings.setValue(l_currentScenario + suffix1, *list);
            settings.endGroup();

            settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + listName + suffix2);
            settings.setValue("name", listName);
            settings.endGroup();

            emit elementListChanged();
        }
    }
}

QStringList AudioEditor::getMusicLists()
{
    return l_musicLists;
}

QStringList AudioEditor::getSoundLists()
{
    return l_soundLists;
}

QStringList AudioEditor::getRadios()
{
    return l_radios;
}

void AudioEditor::saveProject()
{
    if ((l_currentProject != ""))
    {
        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.setValue("categories", l_categoryList);

        if ((l_currentCategory != "") && (l_currentScenario != ""))
        {
            settings.beginGroup(l_currentCategory);

            settings.setValue(l_currentScenario + "_music",         l_musicLists);
            settings.setValue(l_currentScenario + "_sounds",        l_soundLists);
            settings.setValue(l_currentScenario + "_radios",            l_radios);
            settings.setValue(l_currentScenario + "_spotify", m_spotifyPlaylists);

            settings.endGroup();
        }
    }
}

void AudioEditor::setCurrentList(QString list, int type)
{
    l_currentFileNames.clear();
    l_currentFilePaths.clear();
    l_currentFileMissing.clear();

    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (list != ""))
    {
        QString suffix;
        QString fileArrayName;
        QString fileArrayFile;

        switch (type) {
        case 0: // Music List
            suffix            = "_music";
            fileArrayName     = "songs";
            fileArrayFile     = "song";
            l_currentBasePath = sManager->getSetting(Setting::musicPath);
            break;

        case 1: // Sound List
            suffix            = "_sounds";
            fileArrayName     = "sounds";
            fileArrayFile     = "sound";
            l_currentBasePath = sManager->getSetting(Setting::soundPath);
            break;

        case 2: // Radio
            suffix            = "_radio";
            fileArrayName     = "radios";
            fileArrayFile     = "radio";
            l_currentBasePath = sManager->getSetting(Setting::radioPath);
            break;

        case 3: // Spotify
            suffix        = "_spotify";
            fileArrayName = "playlists";
            fileArrayFile = "playlist";
        }

        l_currentList = list;

        QString   path = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
        QSettings settings(path, QSettings::IniFormat);

        settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + list + suffix);

        l_currentListMode = settings.value("mode", 0).toInt();

        QString iconPath = settings.value("icon").toString();
        l_currentListIcon = iconPath;

        l_local = settings.value("local", false).toBool();
        l_url   = settings.value("url", "").toString();

        m_spotifyID = settings.value("id", "").toString();

        // Add every list element
        int count = settings.beginReadArray(fileArrayName);

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            l_currentFileNames.append(settings.value(fileArrayFile, "UNKNOWN FILE").toStringList().at(0));
            l_currentFilePaths.append(settings.value(fileArrayFile, "UNKNOWN PATH").toStringList().at(1));
            l_currentFileMissing.append(!QFile(l_currentBasePath + "/" + settings.value(fileArrayFile, "UNKNOWN FILE").toStringList().at(1)).exists());
        }

        settings.endArray();
        settings.endGroup();

        emit listChanged();
    }
}

void AudioEditor::addFile(QString name, QString path)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (l_currentList != ""))
    {
        l_currentFileNames.append(name);
        l_currentFilePaths.append(path);
        l_currentFileMissing.append(false);

        emit listChanged();
    }
}

void AudioEditor::addFiles(QStringList names, QStringList paths, QList<bool>missing)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (l_currentList != ""))
    {
        l_currentFileNames.append(names);
        l_currentFilePaths.append(paths);
        l_currentFileMissing.append(missing);

        emit listChanged();
    }
}

void AudioEditor::removeFile(int index)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (l_currentList != "") && (l_currentFileNames.size() > index))
    {
        l_currentFileNames.removeAt(index);
        l_currentFilePaths.removeAt(index);
        l_currentFileMissing.removeAt(index);
    }
}

void AudioEditor::saveList(int type)
{
    QString   projectPath = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
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

    settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + l_currentList + suffix);

    if (type == 3)
    {
        settings.setValue("id", m_spotifyID);
    }
    else if (type == 2)
    {
        settings.setValue("url",     l_url);
        settings.setValue("local", l_local);
    }
    else
    {
        settings.beginWriteArray(fileArrayName);

        for (int i = 0; i < l_currentFileNames.size(); i++)
        {
            settings.setArrayIndex(i);
            QStringList file = { l_currentFileNames.at(i), l_currentFilePaths.at(i) };
            settings.setValue(fileArrayFile, file);
        }

        settings.endArray();

        settings.setValue("mode", l_currentListMode);
    }

    settings.setValue("icon", l_currentListIcon);

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
        pList   = &l_musicLists;
        break;

    case 1: // Sound List
        suffix1 = "_sounds";
        suffix2 = suffix1;
        pList   = &l_soundLists;
        break;

    case 2: // Radio
        suffix1 = "_radios";
        suffix2 = "_radio";
        pList   = &l_radios;
        break;

    case 3: // Spotify
        suffix1 = "_spotify";
        suffix2 = suffix1;
        pList   = &m_spotifyPlaylists;
    }

    pList->removeAll(list);

    QString   projectPath = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
    QSettings settings(projectPath, QSettings::IniFormat);

    settings.beginGroup(l_currentCategory);
    settings.setValue(l_currentScenario + suffix1, *pList);
    settings.endGroup();

    settings.remove(l_currentCategory + "_" + l_currentScenario + "_" + list + suffix2);

    l_currentFileNames.clear();
    l_currentFilePaths.clear();
    l_currentFileMissing.clear();
    l_currentList.clear();

    updateElementList();

    emit listChanged();
}

void AudioEditor::setCurrentListMode(int mode)
{
    l_currentListMode = mode;
}

void AudioEditor::setCurrentListIcon(QString icon)
{
    l_currentListIcon = icon;
}

// Move song in list
void AudioEditor::moveFile(int index, int positions)
{
    QString name = l_currentFileNames.at(index);

    l_currentFileNames.removeAt(index);
    l_currentFileNames.insert(index + positions, name);

    QString path = l_currentFilePaths.at(index);

    l_currentFilePaths.removeAt(index);
    l_currentFilePaths.insert(index + positions, path);

    bool missing = l_currentFileMissing.at(index);

    l_currentFileMissing.removeAt(index);
    l_currentFileMissing.insert(index + positions, missing);
}

void AudioEditor::removeMissingFiles()
{
    bool removedFiles = false;

    for (int i = 0; i < l_currentFilePaths.size(); i++)
    {
        if (l_currentFileMissing[i])
        {
            l_currentFileNames.removeAt(i);
            l_currentFilePaths.removeAt(i);
            l_currentFileMissing.removeAt(i);
            i--;
            removedFiles = true;
        }
    }

    if (removedFiles) emit listChanged();
}

void AudioEditor::setURL(QString url)
{
    l_url = url;

    if (l_local) emit urlChanged();
}

void AudioEditor::setLocal(bool local)
{
    l_local = local;
}

void AudioEditor::replaceMissingFolder(int index, QString folder, int type)
{
    QString path = folder.replace("file://", "");

    path  = path.replace(l_currentBasePath, "");
    path += "/" + l_currentFileNames[index];

    l_currentFilePaths[index] = path;

    l_currentFileMissing[index] = !QFile(l_currentBasePath + path).exists();

    saveList(type);
    l_lastListIndex = index;
    emit listChanged();
}

QStringList AudioEditor::getCurrentFileNames()
{
    return l_currentFileNames;
}

QStringList AudioEditor::getCurrentFilePaths()
{
    return l_currentFilePaths;
}

QString AudioEditor::getCurrentListIcon()
{
    return l_currentListIcon;
}

int AudioEditor::getCurrentListMode()
{
    return l_currentListMode;
}

bool AudioEditor::getLocal()
{
    return l_local;
}

QString AudioEditor::getURL()
{
    return l_url;
}
