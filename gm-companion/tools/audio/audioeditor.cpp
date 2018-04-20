#include "audioeditor.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QSettings>

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

        l_musicLists = settings.value(l_currentScenario + "_music").toStringList();
        l_soundLists = settings.value(l_currentScenario + "_sounds").toStringList();
        l_radios     = settings.value(l_currentScenario + "_radios").toStringList();

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

            settings.setValue(l_currentScenario + "_music",  l_musicLists);
            settings.setValue(l_currentScenario + "_sounds", l_soundLists);
            settings.setValue(l_currentScenario + "_radios",     l_radios);

            settings.endGroup();
        }
    }
}

void AudioEditor::setCurrentList(QString list, int type)
{
    l_currentFileNames.clear();
    l_currentFilePaths.clear();

    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (list != ""))
    {
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

        int count = settings.beginReadArray(fileArrayName);

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);

            l_currentFileNames.append(settings.value(fileArrayFile, "UNKNOWN FILE").toStringList().at(0));
            l_currentFilePaths.append(settings.value(fileArrayFile, "UNKNOWN PATH").toStringList().at(1));
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

        emit listChanged();
    }
}

void AudioEditor::addFiles(QStringList names, QStringList paths)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (l_currentList != ""))
    {
        l_currentFileNames.append(names);
        l_currentFilePaths.append(paths);

        emit listChanged();
    }
}

void AudioEditor::removeFile(int index)
{
    if ((l_currentProject != "") && (l_currentCategory != "") && (l_currentScenario != "") && (l_currentList != "") && (l_currentFileNames.size() > index))
    {
        l_currentFileNames.removeAt(index);
        l_currentFilePaths.removeAt(index);

        // Because of Performance reasons, don't emit listChanged()
        // The table row is removed through qml
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
    }

    settings.beginGroup(l_currentCategory + "_" + l_currentScenario + "_" + l_currentList + suffix);

    if (type == 2)
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
    QString suffix;
    QStringList *pList;

    switch (type) {
    case 0: // Music List
        suffix = "_music";
        pList  = &l_musicLists;
        break;

    case 1: // Sound List
        suffix = "_sounds";
        pList  = &l_soundLists;
        break;

    case 2: // Radio
        suffix = "_radios";
        pList  = &l_radios;
        break;
    }

    l_musicLists.removeAll(list);

    QString   projectPath = sManager->getSetting(Setting::audioPath) + "/" + l_currentProject + ".audio";
    QSettings settings(projectPath, QSettings::IniFormat);

    settings.beginGroup(l_currentCategory);
    settings.setValue(l_currentScenario + "_music", *pList);
    settings.endGroup();

    settings.remove(l_currentCategory + "_" + l_currentScenario + "_" + list + suffix);

    l_currentFileNames.clear();
    l_currentFilePaths.clear();
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
}

void AudioEditor::removeMissingFiles(int type)
{
    QString basePath;

    switch (type) {
    case 0: // Music List
        basePath = sManager->getSetting(Setting::musicPath);
        break;

    case 1: // Sound List
        basePath = sManager->getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        basePath = sManager->getSetting(Setting::radioPath);
        break;
    }

    bool removedFiles = false;

    for (int i = 0; i < l_currentFilePaths.size(); i++)
    {
        if (!QFile(basePath + "/" + l_currentFilePaths.at(i)).exists())
        {
            l_currentFileNames.removeAt(i);
            l_currentFilePaths.removeAt(i);
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
