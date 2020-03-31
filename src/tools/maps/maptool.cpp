#include "maptool.h"
#include "logging.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"

MapTool::MapTool(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), qmlEngine(engine)
{
    qDebug() << "Loading Map Tool ...";

    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &MapTool::onFileListReceived);

    mapListModel = new MapListModel;
    qmlEngine->rootContext()->setContextProperty("mapListModel", mapListModel);

    mapMarkerModel = new MapMarkerModel;
    qmlEngine->rootContext()->setContextProperty("mapMarkerModel", mapMarkerModel);

    findCategories();
}

MapTool::~MapTool()
{
    for (auto category : m_categories)
    {
        if (category) category->deleteLater();
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

    if (m_currentCategory)
    {
        disconnect(m_currentCategory, &MapCategory::loadedMaps, this, &MapTool::onMapsLoaded);
    }

    if (index < m_categories.size())
    {
        m_currentCategory = m_categories[index];

        connect(m_currentCategory, &MapCategory::loadedMaps, this, &MapTool::onMapsLoaded);

        m_currentCategory->loadMaps();
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
            map->saveMarkers();
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
            map->saveMarkers();
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
            map->saveMarkers();
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
            map->saveMarkers();
            mapMarkerModel->setElements(map->markers()->elements());
            mapListModel->setElements(m_currentCategory->maps());
        }
    }
}

void MapTool::findCategories()
{
    qCDebug(gmMapsTool()) << "Finding map categories ...";

    m_getCategoriesRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFileList(m_getCategoriesRequestId, SettingsManager::getPath("maps"), true);
}

void MapTool::receivedCategories(QStringList folders)
{
    qCDebug(gmMapsTool()) << "Received map categories.";

    for (auto folder : folders)
    {
        m_categories.append(new MapCategory(folder, {}));
    }

    if (m_categories.size() > 0) setCurrentCategory(m_categories.size() - 1);

    emit categoriesChanged();
}

void MapTool::onMapsLoaded(QString category)
{
    if (m_currentCategory && (category == m_currentCategory->name()))
    {
        mapListModel->setElements(m_currentCategory->maps());
    }
}

void MapTool::onFileListReceived(int id, QStringList files)
{
    if (id == m_getCategoriesRequestId)
    {
        receivedCategories(files);
    }
}
