#include "map.h"

Map::Map(QString name, QString relPath, QObject *parent)
    : QObject(parent), m_name(name), m_relativePath(relPath)
{
    m_markers = new MapMarkerModel;
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
