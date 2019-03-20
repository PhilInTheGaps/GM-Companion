#include "dsa5listitem.h"

DSA5ListItem::DSA5ListItem(QString name, QString group, QList<int>values, QObject *parent)
    : QObject(parent), m_name(name), m_group(group), m_values(values)
{
}

QVariant DSA5ListModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void DSA5ListModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void DSA5ListModel::remove(QObject *item)
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

QHash<int, QByteArray>DSA5ListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void DSA5ListModel::clear()
{
    while (m_items.size() > 0)
    {
        auto i = m_items.takeAt(0);
        i->deleteLater();
    }
}

void DSA5ListModel::setElements(QList<DSA5ListItem *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
