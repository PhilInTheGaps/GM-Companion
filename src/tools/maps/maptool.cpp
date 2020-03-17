#include "maptool.h"
#include "functions.h"
#include "cloud/googledrive.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MapTool::MapTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Map Tool ...";

    connect(fManager->getMapsFileManger(), &MapsFileManager::mapsChanged, [ = ](QList<MapCategory *>categories) {
        m_categories = categories;

        if (m_categories.size() > 0) setCurrentCategory(m_categories.size() - 1);

        emit categoriesChanged();
    });

    mapListModel = new MapListModel;
    qmlEngine->rootContext()->setContextProperty("mapListModel", mapListModel);

    mapMarkerModel = new MapMarkerModel;
    qmlEngine->rootContext()->setContextProperty("mapMarkerModel", mapMarkerModel);

    fManager->getMapsFileManger()->findMaps(fManager->getModeInt());
}

MapTool::~MapTool()
{
    for (auto c : m_categories)
    {
        if (c) c->deleteLater();
    }
}

QStringList MapTool::categories()
{
    QStringList list;

    for (auto c : m_categories)
    {
        if (c) list.append(c->name());
    }

    return list;
}

void MapTool::setCurrentCategory(int index)
{
    qDebug() << "MapTool: Setting current category" << index;

    if (index < m_categories.size())
    {
        m_currentCategory = m_categories[index];
        mapListModel->setElements(m_currentCategory->maps());
        fileManager->getMapsFileManger()->findMapPaths(index);
    }
    else
    {
        m_currentCategory = nullptr;
        mapListModel->clear();
    }
}

void MapTool::setMarkerPosition(int markerIndex, qreal x, qreal y)
{
    if (m_currentCategory && (m_mapIndex > -1) && (m_mapIndex < m_currentCategory->maps().size()))
    {
        auto map = m_currentCategory->maps()[m_mapIndex];

        if (map && (markerIndex > -1) && (markerIndex < map->markers()->size()))
        {
            auto marker = map->markers()->marker(markerIndex);
            marker->setPosition(x, y);
            fileManager->getMapsFileManger()->saveMarkers(map, m_currentCategory->name());
        }
    }
}

void MapTool::setMarkerProperties(QString name, QString description, QString icon, QString color)
{
    if (m_currentCategory && (m_mapIndex > -1) && (m_mapIndex < m_currentCategory->maps().size()))
    {
        auto map = m_currentCategory->maps()[m_mapIndex];

        if (map && (m_markerIndex > -1) && (m_markerIndex < map->markers()->size()))
        {
            auto marker = map->markers()->marker(m_markerIndex);

            marker->setName(name);
            marker->setDescription(description);
            marker->setIcon(icon);
            marker->setColor(color);

            fileManager->getMapsFileManger()->saveMarkers(map, m_currentCategory->name());
            mapMarkerModel->setElements(map->markers()->elements());
        }
    }
}

void MapTool::addMarker(qreal x, qreal y)
{
    if (m_currentCategory && (m_mapIndex > -1) && (m_mapIndex < m_currentCategory->maps().size()))
    {
        auto map = m_currentCategory->maps()[m_mapIndex];

        if (map)
        {
            map->addMarker(new MapMarker(tr("New Marker"), "", x, y, "\uf3c5"));

            fileManager->getMapsFileManger()->saveMarkers(map, m_currentCategory->name());
            mapMarkerModel->setElements(map->markers()->elements());
            mapListModel->setElements(m_currentCategory->maps());
        }
    }
}

void MapTool::setMapIndex(int index)
{
    m_mapIndex = index;

    if (m_currentCategory && (index > -1) && (index < m_currentCategory->maps().size()))
    {
        auto map = m_currentCategory->maps()[m_mapIndex];

        if (map) mapMarkerModel->setElements(map->markers()->elements());
    }
    else {
        m_mapIndex = -1;
    }


    emit mapIndexChanged();
}

void MapTool::deleteMarker(int markerIndex)
{
    if (m_currentCategory && (m_mapIndex > -1) && (m_mapIndex < m_currentCategory->maps().size()))
    {
        auto map = m_currentCategory->maps()[m_mapIndex];

        if (map)
        {
            map->deleteMarker(markerIndex);

            fileManager->getMapsFileManger()->saveMarkers(map, m_currentCategory->name());
            mapMarkerModel->setElements(map->markers()->elements());
            mapListModel->setElements(m_currentCategory->maps());
        }
    }
}
