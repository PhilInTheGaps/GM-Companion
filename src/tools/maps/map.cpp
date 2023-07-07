#include "map.h"
#include "filesystem/file.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/stringutils.h"
#include "utils/utils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QPixmap>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmMapsMap, "gm.maps.map")

Map::Map(const QString &name, const QString &path, QObject *parent)
    : QObject(parent), a_name(name), a_path(path), m_markers(this)
{
    Files::File::getDataAsync(path).then(this, [this](Files::FileDataResult *result) {
        if (!result) return;

        QPixmap pixmap;
        pixmap.loadFromData(result->data());
        imageData(StringUtils::stringFromImage(pixmap));

        loadMarkers();
        result->deleteLater();
    });
}

void Map::saveMarkers() const
{
    QJsonArray markerArray;

    foreach (auto *marker, m_markers.elements())
    {
        markerArray.append(marker->toJson());
    }

    QJsonObject root({{"markers", markerArray}});

    Files::File::saveAsync(path() + ".json", QJsonDocument(root).toJson());
}

void Map::loadMarkers()
{
    const auto filePath = path() + ".json";
    Files::File::checkAsync(filePath).then(this, [this, filePath](Files::FileCheckResult *result) {
        if (!result) return;

        if (result->exists())
        {
            Files::File::getDataAsync(filePath).then(this, [this](Files::FileDataResult *result) {
                if (!result) return;

                auto markers = QJsonDocument::fromJson(result->data()).object()["markers"_L1].toArray();

                for (const auto &marker : markers)
                {
                    addMarker(new MapMarker(marker.toObject(), this));
                }
                emit markersChanged();
                result->deleteLater();
            });
        }

        result->deleteLater();
    });
}

void Map::deleteMarker(int index)
{
    if (Utils::isInBounds(m_markers.elements(), index))
    {
        m_markers.removeAt(index);
    }
}

MapCategory::MapCategory(const QString &name, const QList<Map *> &maps, QObject *parent)
    : QObject(parent), a_name(name), a_maps(maps)
{
}

void MapCategory::addMap(Map *map)
{
    if (!map) return;

    if (!a_maps.contains(map))
    {
        a_maps.append(map);
        emit mapsChanged(maps());
    }
}

void MapCategory::loadMaps()
{
    qCDebug(gmMapsMap()) << "Loading maps for category" << name();

    if (m_wasLoaded)
    {
        emit loadedMaps(name());
        return;
    }

    m_wasLoaded = true;

    const auto path = FileUtils::fileInDir(name(), SettingsManager::getPath(u"maps"_s));

    Files::File::listAsync(path, true, false).then(this, [this, path](Files::FileListResult *result) {
        if (!result) return;

        foreach (const auto &file, result->files())
        {
            if (!file.endsWith(".json"_L1))
            {
                addMap(new Map(file, FileUtils::fileInDir(file, path), this));
            }
        }

        emit loadedMaps(name());

        result->deleteLater();
    });
}

auto MapListModel::data(const QModelIndex &index, int /*role*/) const -> QVariant
{
    auto *item = m_items.at(index.row());
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
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i) == item)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }
}

auto MapListModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void MapListModel::clear()
{
    while (!m_items.isEmpty())
    {
        remove(m_items.constFirst());
    }
}

void MapListModel::setElements(QList<Map *> elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
