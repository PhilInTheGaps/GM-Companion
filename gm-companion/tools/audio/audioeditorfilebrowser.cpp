#include "audioeditorfilebrowser.h"
#include "gm-companion/functions.h"

#include <QDebug>

AudioEditorFileBrowser::AudioEditorFileBrowser(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Editor File Browser ...";
}

void AudioEditorFileBrowser::setType(int type)
{
    l_type = type;

    switch (type) {
    case 0: // Music
        l_basePath = sManager.getSetting(Setting::musicPath);
        break;

    case 1: // Sound
        l_basePath = sManager.getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        l_basePath = sManager.getSetting(Setting::radioPath);
        break;

    default:
        break;
    }

    l_relativeFolders.clear();
    updateFolders();
    updateFiles();
}

int AudioEditorFileBrowser::getType()
{
    return l_type;
}

// Calculate current path
QString AudioEditorFileBrowser::l_currentPath()
{
    QString path;

    for (QString f : l_relativeFolders)
    {
        path += "/" + f;
    }

    return path;
}

void AudioEditorFileBrowser::setCurrentFolder(QString folder)
{
    if (folder != "")
    {
        l_relativeFolders.append(folder);

        updateFolders();
        updateFiles();
    }
}

void AudioEditorFileBrowser::folderBack()
{
    if (l_relativeFolders.size() > 0)
    {
        l_relativeFolders.pop_back();
        updateFolders();
        updateFiles();
    }
}

void AudioEditorFileBrowser::updateFolders()
{
    l_folders.clear();
    QString currentPath = l_currentPath();

    for (QString f : getFolders(l_basePath + currentPath))
    {
        if (!f.contains("."))
        {
            l_folders.append(f);
            l_folderPaths.append(currentPath + "/" + f);
        }
    }

    emit foldersChanged();
}

void AudioEditorFileBrowser::updateFiles()
{
    l_files.clear();
    l_filePaths.clear();

    QString currentPath = l_currentPath();

    for (QString f : getFiles(l_basePath + currentPath))
    {
        if (!f.endsWith("."))
        {
            l_files.append(f);
            l_filePaths.append(currentPath + "/" + f);
        }
    }

    emit filesChanged();
}

QStringList AudioEditorFileBrowser::getFolderList()
{
    return l_folders;
}

QStringList AudioEditorFileBrowser::getFolderPaths()
{
    return l_folderPaths;
}

QStringList AudioEditorFileBrowser::getFileList()
{
    return l_files;
}

QStringList AudioEditorFileBrowser::getFilePaths()
{
    return l_filePaths;
}
