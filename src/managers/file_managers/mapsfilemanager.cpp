#include "mapsfilemanager.h"
#include "functions.h"

#include <QImage>
#include <QImageReader>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MapsFileManager::MapsFileManager(GoogleDrive *google, QObject *parent) : QObject(parent), googleDrive(google)
{
    qDebug() << "Loading MapsFileManager ...";

    connect(google, qOverload<int, QList<GoogleFile> >(&GoogleDrive::receivedFileUrls), this, &MapsFileManager::mapsReceived);
}

/**
 * @brief Find all available map categories and maps inside them
 * @param mode Local: 0, GoogleDrive: 1
 */
void MapsFileManager::findMaps(int mode)
{
    qDebug() << "MapsFileManager: Finding maps ...";
    m_mode = mode;

    switch (mode)
    {
    case 0: // Local
    {
        QString basePath = SettingsManager::getPath("mapsPath");

        // Get Categories
        for (QString f : getFolders(basePath))
        {
            if (!f.contains("."))
            {
                QList<Map *> maps;
                QStringList  markerFiles;

                // Get maps and markers
                for (QString m : getFiles(basePath + "/" + f))
                {
                    auto path = basePath + "/" + f + "/" + m;

                    if (m.endsWith(".json")) // Marker file
                    {
                        markerFiles.append(m);
                    }
                    else if (!QImageReader::imageFormat(path).isEmpty()) // Map
                    {
                        Map *map = new Map(m, "/" + f + "/" + m);
                        map->setPath(path);
                        maps.append(map);
                    }
                }

                // Add markers to maps
                for (auto m : markerFiles)
                {
                    for (int i = 0; i < maps.length(); i++)
                    {
                        auto map        = maps[i];
                        QString mapName = m;
                        mapName.replace(".json", "");

                        if (map->name() == mapName)
                        {
                            auto  path = basePath + "/" + f + "/" + m;
                            QFile f(path);

                            if (f.open(QIODevice::ReadOnly))
                            {
                                auto content = f.readAll();
                                f.close();

                                auto markers = QJsonDocument::fromJson(content).object().value("markers").toArray();

                                for (auto marker : markers)
                                {
                                    QString title       = marker.toObject().value("title").toString();
                                    qreal   x           = marker.toObject().value("x").toDouble();
                                    qreal   y           = marker.toObject().value("y").toDouble();
                                    QString description = marker.toObject().value("description").toString();
                                    QString color       = marker.toObject().value("color").toString();
                                    QString icon        = marker.toObject().value("icon").toString();
                                    map->addMarker(new MapMarker(title, description, x, y, icon, color));
                                }

                                maps.replace(i, map);
                            }

                            break;
                        }
                    }
                }

                m_categories.append(new MapCategory(f, maps));
            }
        }

        break;
    }

    case 1: // Google Drive
        connect(googleDrive, &GoogleDrive::receivedSubfolderIDs, [ = ](int reqId, QList<GoogleFile>files) {
            if (!m_requestIDs.contains(reqId)) return;

            for (auto f : files)
            {
                auto category = new MapCategory(f.fileName, {});
                category->setCloudId(f.id);
                m_categories.append(category);
            }

            emit mapsChanged(m_categories);
            m_requestIDs.removeOne(reqId);
        });

        m_requestIDs.append(googleDrive->getSubfolderIDs(SettingsManager::getSetting("maps", "", "Google")));
        break;

    default: break;
    }

    // Addon maps
    m_categories.append(getAddonMaps());

    emit mapsChanged(m_categories);
}

/**
 * @brief Get a list of MapCategories with maps from addons
 * @return List of MapCategories
 */
QList<MapCategory *>MapsFileManager::getAddonMaps()
{
    QList<MapCategory *> list;

    for (QString path : QStringList({ QDir::homePath() + "/.gm-companion/addons", ":/addons" }))
    {
        for (QString addon : getFolders(path))
        {
            if (SettingsManager::getInstance()->getIsAddonEnabled(addon) && !addon.contains("."))
            {
                if (QDir(path + "/" + addon + "/maps").exists())
                {
                    QSettings settings(path + "/" + addon + "/addon.ini", QSettings::IniFormat);
                    auto categoryName = settings.value("name", addon).toString();

                    QList<Map *> maps;

                    // Get maps
                    for (QString m : getFiles(path + "/" + addon + "/maps"))
                    {
                        auto p = path + "/" + addon + "/maps/" + m;

                        if (!QImageReader::imageFormat(p).isEmpty())
                        {
                            Map *map = new Map(m, "/" + addon + "/maps/" + m);
                            map->setPath(path);
                            maps.append(map);
                        }
                    }

                    list.append(new MapCategory(categoryName, maps));
                }
            }
        }
    }

    return list;
}

/**
 * @brief Find the URLs of maps in GoogleDrive mode
 * @param index Index of the category to find maps in
 */
void MapsFileManager::findMapPaths(int index)
{
    qDebug() << "MapsFileManager: Finding maps paths of category at" << index << "...";

    if ((m_mode == 1) && (index < m_categories.size()))
    {
        auto c = m_categories[index];

        if (c && (c->maps().size() < 1) && (c->requestId() == -1))
        {
            c->setRequestId(googleDrive->getFileUrls(c->cloudId()));
        }
    }
}

/**
 * @brief Update map URLs when they were found
 * @param reqId Request ID to find correct category
 * @param files List with info on the maps
 */
void MapsFileManager::mapsReceived(int reqId, QList<GoogleFile>files)
{
    qDebug() << "MapsFileManager: Received some maps!";

    for (auto c : m_categories)
    {
        if (c && (c->requestId() == reqId))
        {
            auto maps = c->maps();

            for (auto f : files)
            {
                if (!f.fileName.endsWith(".json"))
                {
                    auto map = new Map(f.fileName, f.weblink);
                    map->setPath(f.weblink);
                    maps.append(map);
                }
            }

            c->setMaps(maps);
            emit mapsChanged(m_categories);
            return;
        }
    }
}

/**
 * @brief Save all marker properties to a json file
 * @param map Pointer to map with changed markers
 */
void MapsFileManager::saveMarkers(Map *map, QString category)
{
    if (!map) return;

    QJsonArray array;

    for (auto m : map->markers()->elements())
    {
        QJsonObject o;
        o.insert("title",       m->name());
        o.insert("description", m->description());
        o.insert("icon",        m->icon());
        o.insert("color",       m->color());
        o.insert("x",           m->x());
        o.insert("y",           m->y());
        array.append(o);
    }

    QJsonObject root;
    root.insert("markers", array);

    auto doc = QJsonDocument(root);

    QFile f(SettingsManager::getPath("mapsPath") + "/" + category + "/" + map->name() + ".json");

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson());
        f.close();
    }
}
