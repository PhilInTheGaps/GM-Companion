#include "foldertree.h"
#include <QDebug>

FolderTree::FolderTree(QStringList ids, QString rootName, QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing FolderTree ...";

    m_root = new Folder(rootName);
    m_root->setId("ROOT");

    m_music = new Folder("Music");
    m_music->setId(ids[0]);
    m_root->addChild(m_music);

    m_sounds = new Folder("Sounds");
    m_sounds->setId(ids[1]);
    m_root->addChild(m_sounds);

    m_radio = new Folder("Radio");
    m_radio->setId(ids[2]);
    m_root->addChild(m_radio);

    m_maps = new Folder("Maps");
    m_maps->setId(ids[4]);
    m_root->addChild(m_maps);

    m_shop = new Folder("Shops");
    m_shop->setId(ids[5]);
    m_root->addChild(m_shop);

    m_resources = new Folder("Resources");
    m_resources->setId(ids[8]);
    m_root->addChild(m_resources);
}

/**
 * @brief Print the FolderTree for debugging purposes
 */
void FolderTree::print()
{
    m_root->printSubfolders();
}

/**
 * @brief Get a pointer to the folder with the Google ID
 * @param id Google ID of the requested folder
 * @return Pointer to requested folder, nullptr if folder could not be found
 */
Folder * FolderTree::getFolder(QString id)
{
    if (m_root->getId() == id)
    {
        return m_root;
    }
    else
    {
        return getFolder(id, m_root);
    }
}

/**
 * @brief Get a pointer to a folder
 * @param id Google ID of the requested folder
 * @param root Root folder to search in
 * @return Pointer to requested folder, nullptr if folder could not be found
 */
Folder * FolderTree::getFolder(QString id, Folder *root)
{
    for (Folder *f : root->children())
    {
        if (f)
        {
            if (f->getId() == id)
            {
                return f;
            }
            else
            {
                Folder *f2 = getFolder(id, f);

                if (f2 != nullptr)
                {
                    return f2;
                }
            }
        }
    }

    return nullptr;
}

/**
 * @brief Get a pointer to a folder by looking for the internal ID
 * @param internalID Internal ID (integer) of the requested folder
 * @return Pointer to requested folder, nullptr if folder could not be found
 */
Folder * FolderTree::getFolder(int internalID)
{
    if (m_root->getInternalId() == internalID)
    {
        return m_root;
    }
    else
    {
        return getFolder(internalID, m_root);
    }
}

/**
 * @brief Get a pointer to a folder by looking for the internal ID
 * @param internalID Internal ID (integer) of the requested folder
 * @param root Root folder to look in
 * @return Pointer to requested folder, nullptr if folder could not be found
 */
Folder * FolderTree::getFolder(int internalID, Folder *root)
{
    for (Folder *f : root->children())
    {
        if (f)
        {
            if (f->getInternalId() == internalID)
            {
                return f;
            }
            else
            {
                Folder *f2 = getFolder(internalID, f);

                if (f2 != nullptr)
                {
                    return f2;
                }
            }
        }
    }

    return nullptr;
}

/**
 * @brief Get a pointer to one of the default folders (Audio, Music, etc.)
 * @param f Default folder to get pointer to
 * @return Pointer to DefaultFolder, nullptr if folder is invalid
 */
Folder * FolderTree::getDefaultFolder(DefaultFolder f)
{
    switch (f)
    {
    case DefaultFolder::Root:
        return m_root;

    case DefaultFolder::Music:
        return m_music;

    case DefaultFolder::Sounds:
        return m_sounds;

    case DefaultFolder::Radio:
        return m_radio;

    case DefaultFolder::Audio:
        return m_audio;

    case DefaultFolder::Maps:
        return m_maps;

    case DefaultFolder::Shop:
        return m_shop;

    case DefaultFolder::Characters:
        return m_characters;

    case DefaultFolder::Notes:
        return m_notes;

    case DefaultFolder::Resources:
        return m_resources;
    }

    return nullptr;
}

/**
 * @brief Print the names of all subfolders for debugging
 * @param level The folder level for printing folder trees
 * @param recursive If subfolders of subfolders should be printed as well
 */
void Folder::printSubfolders(int level, bool recursive)
{
    if (recursive)
    {
        for (Folder *f : m_children)
        {
            if (f) f->printSubfolders(level + 1);
        }
    }

    QString tabs;

    for (int i = 0; i < level; i++)
    {
        tabs.append("   ");
    }

    qDebug().noquote() << tabs << m_name << m_internalId;
}

/**
 * @brief Get a child folder by name
 * @param name Name of the requested folder
 * @return Pointer to requested folder, nullptr if folder could not be found
 */
Folder * Folder::getChild(QString name)
{
    for (Folder *f : m_children)
    {
        if (f->getName() == name) return f;
    }

    return nullptr;
}

/**
 * @brief Print all folders that do not have a proper id yet
 */
void FolderTree::printMissingFolders()
{
    for (Folder *f : m_root->children())
    {
        if (f) f->printMissingSubfolders();
    }
}

/**
 * @brief Print all subfolders that do not have a proper id yet
 */
void Folder::printMissingSubfolders()
{
    for (Folder *f : m_children)
    {
        f->printMissingSubfolders();
    }

    if (!hasId()) qDebug() << "FolderTree: Missing ID of" << m_name;
}

/**
 * @brief Set if a folder and all subfolders should be ignored (hidden)
 * @param ignore True if folder should be ignored
 * @return Amount of changed folders
 */
int Folder::ignore(bool ignore)
{
    int count = 1;

    for (auto a : m_children)
    {
        count += a->ignore(ignore);
    }

    m_isIgnored = ignore;

    return count;
}
