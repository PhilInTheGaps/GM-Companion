#include "maptool.h"
#include "logging.h"

#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>

#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"

MapTool::MapTool(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), qmlEngine(engine)
{
    qDebug() << "Loading Map Tool ...";

    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &MapTool::onFileListReceived);

    qmlEngine->rootContext()->setContextProperty("map_tool", this);

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
    qCDebug(gmMapsTool()) << "Setting current category" << index;

    if (index == m_currentCategoryIndex) return;

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

    m_currentCategoryIndex = index;
    emit currentCategoryChanged();
}

void MapTool::setMarkerPosition(int markerIndex, qreal x, qreal y)
{
    if (m_currentMap && (markerIndex > -1) && (markerIndex < m_currentMap->markers()->size()))
    {
        auto marker = m_currentMap->markers()->marker(markerIndex);
        marker->setPosition(x, y);
        m_currentMap->saveMarkers();
        return;
    }

    qCCritical(gmMapsTool()) << "Error: Could not save marker position" << markerIndex << x << y;
}

void MapTool::setMarkerProperties(QString name, QString description, QString icon, QString color)
{
    if (m_currentMap && (m_markerIndex > -1) && (m_markerIndex < m_currentMap->markers()->size()))
    {
        auto marker = m_currentMap->markers()->marker(m_markerIndex);

        marker->setName(name);
        marker->setDescription(description);
        marker->setIcon(icon);
        marker->setColor(color);
        m_currentMap->saveMarkers();
        mapMarkerModel->setElements(m_currentMap->markers()->elements());
        return;
    }

    qCCritical(gmMapsTool()) << "Error: Could not save marker properties:" << name << description << icon << color;
}

void MapTool::addMarker()
{
    if (m_currentMap)
    {
        auto data  = QByteArray::fromBase64(m_currentMap->data().replace("data:image/jpg;base64,", "").toLatin1());
        auto image = QImage::fromData(data);
        auto x     = image.width() / 2.0;
        auto y     = image.height() / 2.0;

        m_currentMap->addMarker(new MapMarker(tr("New Marker"), "", x, y, "\uf3c5"));
        m_currentMap->saveMarkers();
        mapMarkerModel->setElements(m_currentMap->markers()->elements());
        mapListModel->setElements(m_currentCategory->maps());
        return;
    }

    qCCritical(gmMapsTool()) << "Error: Could not add marker";
}

void MapTool::setMapIndex(int index)
{
    m_mapIndex = index;

    if (m_currentCategory && (index > -1) && (index < m_currentCategory->maps().size()))
    {
        m_currentMap = m_currentCategory->maps()[m_mapIndex];

        if (m_currentMap) mapMarkerModel->setElements(m_currentMap->markers()->elements());
    }
    else {
        m_mapIndex   = -1;
        m_currentMap = nullptr;
    }

    setMarkerIndex(-1);
    emit mapIndexChanged();
}

MapMarker * MapTool::currentMarker() const
{
    if ((m_markerIndex > -1) && (mapMarkerModel->size() > m_markerIndex))
    {
        return mapMarkerModel->elements()[m_markerIndex];
    }

    return nullptr;
}

void MapTool::deleteMarker(int markerIndex)
{
    if (m_currentMap)
    {
        m_currentMap->deleteMarker(markerIndex);
        m_currentMap->saveMarkers();
        mapMarkerModel->setElements(m_currentMap->markers()->elements());
        mapListModel->setElements(m_currentCategory->maps());
        return;
    }

    qCCritical(gmMapsTool()) << "Error: Could not delete marker" << markerIndex;
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
