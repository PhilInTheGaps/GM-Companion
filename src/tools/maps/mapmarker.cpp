#include "mapmarker.h"
#include "utils/utils.h"

MapMarker::MapMarker(const QString &name, const QString &description, qreal x, qreal y, const QString &icon,
                     const QString &color, QObject *parent)
    : QObject(parent), a_name(name), a_description(description), a_color(color), a_icon(icon), m_x(x), m_y(y)
{
}

MapMarker::MapMarker(const QJsonObject &json, QObject *parent) : QObject(parent)
{
    name(json[QStringLiteral("title")].toString());
    description(json[QStringLiteral("description")].toString());
    color(json[QStringLiteral("color")].toString());
    icon(json[QStringLiteral("icon")].toString());

    setPosition(json[QStringLiteral("x")].toDouble(), json[QStringLiteral("y")].toDouble());
}

auto MapMarker::toJson() const -> QJsonObject
{
    return {{"title", name()}, {"description", description()}, {"icon", icon()}, {"color", color()}, {"x", x()},
            {"y", y()}};
}

auto MapMarkerModel::data(const QModelIndex &index, int /*role*/) const -> QVariant
{
    auto *item = m_items.at(index.row());
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

auto MapMarkerModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void MapMarkerModel::clear()
{
    while (!m_items.isEmpty())
    {
        remove(m_items.constFirst());
    }
}

void MapMarkerModel::setElements(const QList<MapMarker *> &elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}

void MapMarkerModel::removeAt(int index)
{
    if (Utils::isInBounds(m_items, index))
    {
        m_items.removeAt(index);
    }
}

auto MapMarkerModel::elements() const -> QList<MapMarker *>
{
    QList<MapMarker *> list;
    list.reserve(m_items.size());

    for (auto *item : m_items)
    {
        list.append(qobject_cast<MapMarker *>(item));
    }

    return list;
}
