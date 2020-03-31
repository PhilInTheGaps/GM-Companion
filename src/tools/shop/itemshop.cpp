#include "itemshop.h"
#include <QDebug>
#include <QJsonArray>

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

ItemShop::ItemShop(QJsonObject json)
{
    // Get Items in Shop
    auto itemArray = json["items"].toArray();

    for (auto item : itemArray)
    {
        m_items.append(new Item(item.toObject()));
    }

    m_name        = json["name"].toString();
    m_owner       = json["owner"].toString();
    m_description = json["description"].toString();
}

ItemShop::~ItemShop()
{
    for (auto i : m_items)
    {
        if (i) i->deleteLater();
    }
}

QJsonObject ItemShop::toJson()
{
    QJsonObject root;

    root.insert("name",        name());
    root.insert("owner",       owner());
    root.insert("description", description());

    QJsonArray itemArray;

    for (auto item : items())
    {
        if (item)
        {
            itemArray.append(item->toJson());
        }
    }
    root.insert("items", itemArray);

    return root;
}

ItemGroup::ItemGroup(ItemGroup *other)
{
    m_name = other->m_name;

    for (auto item : other->m_items)
    {
        if (item)
        {
            m_items.append(new Item(item));
        }
    }
}

ItemGroup::ItemGroup(QString groupName, QJsonObject json)
{
    auto categories = json["categories"].toArray();

    for (auto category : categories)
    {
        auto categoryName = category.toObject()["name"].toString();

        for (auto item : category.toObject()["items"].toArray())
        {
            m_items.append(new Item(categoryName, item.toObject()));
        }
    }

    m_name = groupName;
}

QJsonObject ItemGroup::toJson()
{
    QJsonObject root;

    root.insert("version", 2);

    QJsonArray  categoryArray;
    QStringList categories;

    // Find all categories
    for (auto item : items())
    {
        if (item && !categories.contains(item->category()))
        {
            categories.append(item->category());
        }
    }

    for (auto category : categories)
    {
        QJsonObject categoryObject;
        categoryObject.insert("name", category);

        QJsonArray itemArray;

        for (auto item : items())
        {
            if (item && (item->category() == category))
            {
                itemArray.append(item->toJson());
            }
        }

        categoryObject.insert("items", itemArray);
        categoryArray.append(categoryObject);
    }

    root.insert("categories", categoryArray);
    return root;
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

Item::Item(QJsonObject json)
{
    m_name        = json["name"].toString();
    m_price       = json["price"].toString();
    m_description = json["description"].toString();
    m_category    = json["category"].toString();
}

Item::Item(QString category, QJsonObject json)
{
    m_name        = json["name"].toString();
    m_price       = json["price"].toString();
    m_description = json["description"].toString();
    m_category    = category;
}

QJsonObject Item::toJson()
{
    QJsonObject root;

    root.insert("name",        name());
    root.insert("price",       price());
    root.insert("description", description());
    root.insert("category",    category());
    return root;
}
