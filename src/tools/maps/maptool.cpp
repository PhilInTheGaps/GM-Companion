#include "maptool.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem_new/file.h"
#include "utils/utils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>

using namespace AsyncFuture;

MapTool::MapTool(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), a_markerIndex(-1), qmlEngine(engine)
{
    qDebug() << "Loading Map Tool ...";

    qmlEngine->rootContext()->setContextProperty("map_tool", this);

    mapListModel = new MapListModel(this);
    qmlEngine->rootContext()->setContextProperty("mapListModel", mapListModel);

    mapMarkerModel = new MapMarkerModel(this);
    qmlEngine->rootContext()->setContextProperty("mapMarkerModel", mapMarkerModel);
}

MapTool::~MapTool()
{
    for (auto *category : m_categories)
    {
        if (category) category->deleteLater();
    }
}

auto MapTool::categories() const -> QStringList
{
    QStringList list;

    for (const auto *category : m_categories)
    {
        if (category) list.append(category->name());
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

void MapTool::setMarkerProperties(const QString &name, const QString &description, const QString &icon, const QString &color)
{
    if (m_currentMap &&  (markerIndex() > -1) && (markerIndex() < m_currentMap->markers()->size()))
    {
        auto *marker = m_currentMap->markers()->marker(markerIndex());

        marker->name(name);
        marker->description(description);
        marker->icon(icon);
        marker->color(color);
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
        auto data  = QByteArray::fromBase64(m_currentMap->imageData().replace("data:image/jpg;base64,", "").toLatin1());
        auto image = QImage::fromData(data);
        auto x     = image.width() / 2.0;
        auto y     = image.height() / 2.0;

        m_currentMap->addMarker(new MapMarker(tr("New Marker"), "", x, y, DEFAULT_ICON, DEFAULT_COLOR, this));
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

    if (m_currentCategory && Utils::isInBounds(m_currentCategory->maps(), index))
    {
        m_currentMap = m_currentCategory->maps()[m_mapIndex];

        if (m_currentMap)
        {
            mapMarkerModel->setElements(m_currentMap->markers()->elements());
        }
    }
    else
    {
        m_mapIndex   = -1;
        m_currentMap = nullptr;
    }

    markerIndex(-1);
    emit mapIndexChanged();
}

auto MapTool::currentMarker() const -> MapMarker*
{
    const auto &elements = mapMarkerModel->elements();
    if (Utils::isInBounds(elements, markerIndex()))
    {
        return mapMarkerModel->elements()[markerIndex()];
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

void MapTool::loadData()
{
    if (m_isDataLoaded) return;

    m_isDataLoaded = true;
    findCategories();
}

void MapTool::findCategories()
{
    qCDebug(gmMapsTool()) << "Finding map categories ...";

    const auto dir = SettingsManager::getPath("maps");
    observe(Files::File::listAsync(dir, false, true)).subscribe([this](Files::FileListResult *result) {
        receivedCategories(result->folders());
        result->deleteLater();
    });
}

void MapTool::receivedCategories(const QStringList &folders)
{
    qCDebug(gmMapsTool()) << "Received map categories.";

    for (const auto &folder : folders)
    {
        m_categories.append(new MapCategory(folder, {}, this));
    }

    emit categoriesChanged();
}

void MapTool::onMapsLoaded(const QString &category)
{
    if (m_currentCategory && (category == m_currentCategory->name()))
    {
        mapListModel->setElements(m_currentCategory->maps());
    }
}
