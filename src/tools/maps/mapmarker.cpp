#include "mapmarker.h"

MapMarker::MapMarker(QString name, QString description, qreal x, qreal y, QString icon, QString color, QObject *parent)
    : QObject(parent), m_name(name), m_description(description), m_color(color), m_icon(icon), m_x(x), m_y(y)
{
}

QVariant MapMarkerModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void MapMarkerModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void MapMarkerModel::remove(QObject *item)
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

QHash<int, QByteArray>MapMarkerModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void MapMarkerModel::clear()
{
    while (m_items.size() > 0)
    {
        remove(m_items[0]);
    }
}

void MapMarkerModel::setElements(QList<MapMarker *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}

void MapMarkerModel::removeAt(int index)
{
    if ((index > -1) && (index < m_items.size()))
    {
        m_items.removeAt(index);
    }
}
