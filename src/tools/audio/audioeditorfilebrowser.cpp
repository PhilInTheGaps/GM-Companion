#include "audioeditorfilebrowser.h"
#include "src/functions.h"

#include <QDebug>

AudioEditorFileBrowser::AudioEditorFileBrowser(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Editor File Browser ...";

    m_type = -1;
}

/**
 * @brief Set the type of the current element
 * @param type Music: 0, Sounds: 1, Radio: 2
 */
void AudioEditorFileBrowser::setType(int type)
{
    if (type == m_type) return;

    qDebug() << "AudioEditorFileBrowser: Changing type:" << type << "...";
    m_type = type;
    emit typeChanged();

    switch (type)
    {
    case 0: // Music
        m_basePath = sManager.getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        m_basePath = sManager.getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        m_basePath = sManager.getSetting(Setting::radioPath);
        break;
    }

    m_relativeFolders.clear();
    updateFolders();
    updateFiles();
}

/**
 * @brief Calculate the current path
 * @return The absolute current path
 */
QString AudioEditorFileBrowser::currentPath()
{
    QString path;

    for (QString f : m_relativeFolders)
    {
        path += "/" + f;
    }

    return path;
}

/**
 * @brief Set the current folder
 * @param New relative folder
 */
void AudioEditorFileBrowser::setCurrentFolder(QString folder)
{
    if (folder != "")
    {
        m_relativeFolders.append(folder);

        updateFolders();
        updateFiles();
    }
}

/**
 * @brief Go one folder back
 */
void AudioEditorFileBrowser::folderBack()
{
    if (m_relativeFolders.size() > 0)
    {
        m_relativeFolders.pop_back();
        updateFolders();
        updateFiles();
    }
}

/**
 * @brief Go back to the base path
 */
void AudioEditorFileBrowser::home()
{
    m_relativeFolders.clear();
    updateFolders();
    updateFiles();
}

/**
 * @brief Update the list of subfolders in current path
 */
void AudioEditorFileBrowser::updateFolders()
{
    m_folders.clear();

    QString tempPath = currentPath();

    for (QString f : getFolders(m_basePath + tempPath))
    {
        if (!f.contains("."))
        {
            m_folders.append(f);
        }
    }
    emit foldersChanged();
}

/**
 * @brief Update the list of files in current path
 */
void AudioEditorFileBrowser::updateFiles()
{
    m_files.clear();
    m_filePaths.clear();

    QString tempPath = currentPath();

    for (QString f : getFiles(m_basePath + tempPath))
    {
        if (!f.endsWith("."))
        {
            m_files.append(f);
            m_filePaths.append(tempPath + "/" + f);
        }
    }

    emit filesChanged();
}

/**
 * @brief Add all files from current path to the current element
 */
void AudioEditorFileBrowser::addAllFiles()
{
    emit addFiles(m_filePaths);
}
