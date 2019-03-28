#include "itemshop.h"
#include <QDebug>

ItemShop::ItemShop(QString name, QString owner, QString description, QList<Item *>items, QObject *parent)
    : QObject(parent), m_name(name), m_owner(owner), m_description(description), m_items(items)
{
    qDebug() << "Initializing new ItemShop" << name << "...";
}

ItemShop::ItemShop(ItemShop *other)
{
    if (other)
    {
        m_name        = other->m_name;
        m_owner       = other->m_owner;
        m_description = other->m_description;

        for (auto i : other->m_items)
        {
            if (i)
            {
                auto copy = new Item(i);
                m_items.append(copy);
            }
        }
    }
}

ItemShop::~ItemShop()
{
    for (auto i : m_items)
    {
        if (i) i->deleteLater();
    }
}

ItemGroup::ItemGroup(ItemGroup *other)
{
    m_name = other->m_name;

    for (auto i : other->m_items)
    {
        if (i)
        {
            Item *item = new Item(i);
            m_items.append(item);
        }
    }
}

QVariant ItemModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void ItemModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void ItemModel::remove(QObject *item)
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

QHash<int, QByteArray>ItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void ItemModel::clear()
{
    while (m_items.size() > 0)
    {
        remove(m_items[0]);
    }
}

void ItemModel::setElements(QList<Item *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
