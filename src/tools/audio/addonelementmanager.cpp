#include "addonelementmanager.h"
#include "src/functions.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AddonElementManager::AddonElementManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Addon Element Manager ...";

    findAddons();
}

void AddonElementManager::setFolder(QString folder)
{
    m_currentSpotifyFolder = folder;
    findSpotifyPlaylists();
}

// Get all enabled addons
void AddonElementManager::findAddons()
{
    QStringList paths = { QDir::homePath() + "/.gm-companion/addons", ":/addons" };

    for (QString path : paths)
    {
        for (QString addon : getFolders(path))
        {
            if (!addon.startsWith(".") && sManager.getIsAddonEnabled(addon))
            {
                m_addonPaths.append(path + "/" + addon);
                QString spotifyFile = path + "/" + addon + "/spotify.json";

                if (QFile(spotifyFile).exists()) m_spotifyFiles.append(spotifyFile);
            }
        }
    }

    findSpotifyFolders();
}

// Read all Spotify files and get folders
void AddonElementManager::findSpotifyFolders()
{
    for (QString file : m_spotifyFiles)
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QString content = f.readAll();
        f.close();

        auto doc     = QJsonDocument::fromJson(content.toLocal8Bit());
        auto root    = doc.object();
        auto folders = root.value("data").toArray();

        for (auto folder : folders)
        {
            QString folderName = folder.toObject().value("folder").toString();

            if (!m_spotifyFolders.contains(folderName)) m_spotifyFolders.append(folderName);
        }
    }
}

void AddonElementManager::resetChecked()
{
    for (int i = 0; i < m_addSpotifyPlaylists.size(); i++)
    {
        m_addSpotifyPlaylists[i] = false;
    }

    emit elementsChanged();
}

void AddonElementManager::findSpotifyPlaylists()
{
    m_spotifyPlaylistNames.clear();
    m_spotifyPlaylistURIs.clear();
    m_addSpotifyPlaylists.clear();

    if (!m_currentSpotifyFolder.isEmpty())
    {
        for (QString file : m_spotifyFiles)
        {
            QFile f(file);
            f.open(QIODevice::ReadOnly);
            QString content = f.readAll();
            f.close();

            auto doc     = QJsonDocument::fromJson(content.toLocal8Bit());
            auto root    = doc.object();
            auto folders = root.value("data").toArray();

            for (auto folder : folders)
            {
                QString folderName = folder.toObject().value("folder").toString();

                if (folderName == m_currentSpotifyFolder)
                {
                    auto playlists = folder.toObject().value("playlists").toArray();

                    for (auto playlist : playlists)
                    {
                        auto name = playlist.toObject().value("name").toString();

                        if (!m_spotifyPlaylistNames.contains(name))
                        {
                            auto uri = playlist.toObject().value("uri").toString();
                            m_spotifyPlaylistURIs.append(uri);
                            m_spotifyPlaylistNames.append(name);
                            m_addSpotifyPlaylists.append(false);
                        }
                    }
                }
            }
        }
    }

    emit elementsChanged();
}
