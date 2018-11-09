#include "audioeditorfilebrowser.h"
#include "src/functions.h"

#include <QDebug>

AudioEditorFileBrowser::AudioEditorFileBrowser(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Editor File Browser ...";
}

void AudioEditorFileBrowser::setType(int type)
{
    m_type = type;

    switch (type) {
    case 0: // Music
        m_basePath = sManager.getSetting(Setting::musicPath);
        break;

    case 1: // Sound
        m_basePath = sManager.getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        m_basePath = sManager.getSetting(Setting::radioPath);
        break;

    default:
        break;
    }

    m_relativeFolders.clear();
    updateFolders();
    updateFiles();
}

int AudioEditorFileBrowser::getType()
{
    return m_type;
}

// Calculate current path
QString AudioEditorFileBrowser::m_currentPath()
{
    QString path;

    for (QString f : m_relativeFolders)
    {
        path += "/" + f;
    }

    return path;
}

void AudioEditorFileBrowser::setCurrentFolder(QString folder)
{
    if (folder != "")
    {
        m_relativeFolders.append(folder);

        updateFolders();
        updateFiles();
    }
}

void AudioEditorFileBrowser::folderBack()
{
    if (m_relativeFolders.size() > 0)
    {
        m_relativeFolders.pop_back();
        updateFolders();
        updateFiles();
    }
}

void AudioEditorFileBrowser::updateFolders()
{
    m_folders.clear();
    QString currentPath = m_currentPath();

    for (QString f : getFolders(m_basePath + currentPath))
    {
        if (!f.contains("."))
        {
            m_folders.append(f);
            m_folderPaths.append(currentPath + "/" + f);
        }
    }

    emit foldersChanged();
}

void AudioEditorFileBrowser::updateFiles()
{
    m_files.clear();
    m_filePaths.clear();

    QString currentPath = m_currentPath();

    for (QString f : getFiles(m_basePath + currentPath))
    {
        if (!f.endsWith("."))
        {
            m_files.append(f);
            m_filePaths.append(currentPath + "/" + f);
        }
    }

    emit filesChanged();
}

QStringList AudioEditorFileBrowser::getFolderList()
{
    return m_folders;
}

QStringList AudioEditorFileBrowser::getFolderPaths()
{
    return m_folderPaths;
}

QStringList AudioEditorFileBrowser::getFileList()
{
    return m_files;
}

QStringList AudioEditorFileBrowser::getFilePaths()
{
    return m_filePaths;
}
