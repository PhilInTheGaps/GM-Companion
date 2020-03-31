#include "map.h"
#include "logging.h"
#include "utils/utils.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>

Map::Map(QString name, QString path, QObject *parent)
    : QObject(parent), m_name(name), m_path(path)
{
    m_markers = new MapMarkerModel;

    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFile, context, [ = ](int id, QByteArray data) {
        if (id != requestId) return;

        QPixmap pixmap;
        pixmap.loadFromData(data);

        m_data = Utils::stringFromImage(pixmap);
        emit dataChanged();

        loadMarkers();

        delete context;
    });

    FileManager::getInstance()->getFile(requestId, path);
}

Map::~Map()
{
    m_markers->deleteLater();
}

void Map::saveMarkers()
{
    QJsonArray markerArray;

    for (auto marker : m_markers->elements())
    {
        QJsonObject markerObject;
        markerObject.insert("title",       marker->name());
        markerObject.insert("description", marker->description());
        markerObject.insert("icon",        marker->icon());
        markerObject.insert("color",       marker->color());
        markerObject.insert("x",           marker->x());
        markerObject.insert("y",           marker->y());
        markerArray.append(markerObject);
    }

    QJsonObject root({ { "markers", markerArray } });

    FileManager::getInstance()->saveFile(m_path + ".json", QJsonDocument(root).toJson());
}

void Map::loadMarkers()
{
    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFile, context, [ = ](int id, QByteArray data) {
        if (id != requestId) return;

        auto markers = QJsonDocument::fromJson(data).object()["markers"].toArray();

        for (auto marker : markers)
        {
            QString title       = marker.toObject()["title"].toString();
            qreal   x           = marker.toObject()["x"].toDouble();
            qreal   y           = marker.toObject()["y"].toDouble();
            QString description = marker.toObject()["description"].toString();
            QString color       = marker.toObject()["color"].toString();
            QString icon        = marker.toObject()["icon"].toString();
            addMarker(new MapMarker(title, description, x, y, icon, color));
        }
        emit markersChanged();
        delete context;
    });

    FileManager::getInstance()->getFile(requestId, m_path + ".json");
}

void Map::deleteMarker(int index)
{
    if ((index > -1) && (index < m_markers->elements().size()))
    {
        m_markers->removeAt(index);
    }
}

MapCategory::MapCategory(QString name, QList<Map *>maps, QObject *parent)
    : QObject(parent), m_name(name), m_maps(maps)
{
}

MapCategory::~MapCategory()
{
    for (auto m : m_maps)
    {
        if (m) m->deleteLater();
    }
}

void MapCategory::addMap(Map *map)
{
    if (!map) return;

    if (!m_maps.contains(map))
    {
        m_maps.append(map);
    }
}

void MapCategory::loadMaps()
{
    qCDebug(gmMapsMap()) << "Loading maps for category" << m_name;

    if (m_wasLoaded)
    {
        emit loadedMaps(m_name);
        return;
    }

    m_wasLoaded = true;

    m_getFileListRequestId = FileManager::getUniqueRequestId();
    auto path = SettingsManager::getPath("maps") + "/" + m_name;

    auto context = new QObject;
    connect(FileManager::getInstance(), &FileManager::receivedFileList, context, [ = ](int id, QStringList files) {
        if (id != m_getFileListRequestId) return;

        QStringList markerFiles;

        for (auto file : files)
        {
            if (file.endsWith(".json"))
            {
                markerFiles.append(file);
            }
            else
            {
                addMap(new Map(file, path + "/" + file));
            }
        }

        emit loadedMaps(m_name);

        delete context;
    });

    FileManager::getInstance()->getFileList(m_getFileListRequestId, path, false);
}

QVariant MapListModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void MapListModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void MapListModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i) == item) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }
}

QHash<int, QByteArray>MapListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void MapListModel::clear()
{
    while (m_items.size() > 0)
    {
        remove(m_items[0]);
    }
}

void MapListModel::setElements(QList<Map *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
