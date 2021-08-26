#include "itemshop.h"
#include <QDebug>
#include <QJsonArray>

ItemShop::ItemShop(const QString& name, const QString &owner, const QString &description, const QList<Item*> &items, QObject *parent)
    : QObject(parent), a_name(name), a_owner(owner), a_description(description), a_items(items)
{
    qDebug() << "Initializing new ItemShop" << name << "...";
}

ItemShop::ItemShop(ItemShop *other)
{
    if (!other) return;

    name(other->name());
    owner(other->owner());
    description(other->description());

    for (auto *item : other->items())
    {
        if (item)
        {
            auto *copy = new Item(item);
            a_items.append(copy);
        }
    }
}

ItemShop::ItemShop(const QJsonObject &json)
{
    // Get Items in Shop
    auto itemArray = json["items"].toArray();

    for (const auto &item : itemArray)
    {
        a_items.append(new Item(item.toObject()));
    }

    name(json["name"].toString());
    owner(json["owner"].toString());
    description(json["description"].toString());
}

ItemShop::~ItemShop()
{
    for (auto *item : items())
    {
        if (item) item->deleteLater();
    }
}

auto ItemShop::toJson() -> QJsonObject
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
    a_name = other->name();

    for (auto *item : other->items())
    {
        if (item)
        {
            a_items.append(new Item(item));
        }
    }
}

ItemGroup::ItemGroup(const QString &groupName, const QJsonObject &json)
{
    auto categories = json["categories"].toArray();

    for (auto category : categories)
    {
        auto categoryName = category.toObject()["name"].toString();

        for (auto item : category.toObject()["items"].toArray())
        {
            a_items.append(new Item(categoryName, item.toObject()));
        }
    }

    a_name = groupName;
}

auto ItemGroup::toJson() -> QJsonObject
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

    for (const auto& category : categories)
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

auto ItemModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
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

auto ItemModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void ItemModel::clear()
{
    while (!m_items.isEmpty())
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

Item::Item(Item *other)
{
    if (!other) return;

    name(other->name());
    price(other->price());
    description(other->description());
    category(other->category());
}

Item::Item(const QJsonObject &json)
{
    name(json["name"].toString());
    price(json["price"].toString());
    description(json["description"].toString());
    category(json["category"].toString());
}

Item::Item(const QString &category, const QJsonObject &json)
{
    name(json["name"].toString());
    price(json["price"].toString());
    description(json["description"].toString());
    this->category(category);
}

auto Item::toJson() -> QJsonObject
{
    QJsonObject root;

    root.insert("name",        name());
    root.insert("price",       price());
    root.insert("description", description());
    root.insert("category",    category());
    return root;
}
