#include "itemgroup.h"
#include <QJsonArray>

ItemGroup::ItemGroup(const QString &name, const QList<Item *> &items, QObject *parent)
    : BaseProjectItem(name, parent), a_items(items)
{
    a_categories = findCategories();
}

ItemGroup::ItemGroup(const ItemGroup &other, QObject *parent) : BaseProjectItem(other.name(), parent)
{
    foreach (const auto *item, other.items())
    {
        if (item)
        {
            a_items.append(new Item(*item, this));
        }
    }

    a_categories = findCategories();
}

ItemGroup::ItemGroup(const QString &name, const QJsonObject &json, QObject *parent) : BaseProjectItem(name, parent)
{
    const auto categories = json[QStringLiteral("categories")].toArray();
    a_categories.reserve(categories.count());

    foreach (const auto &category, categories)
    {
        auto categoryName = category.toObject()[QStringLiteral("name")].toString();
        a_categories.append(categoryName);

        foreach (const auto &item, category.toObject()[QStringLiteral("items")].toArray())
        {
            a_items.append(new Item(categoryName, item.toObject(), parent));
        }
    }
}

auto ItemGroup::toJson() const -> QJsonObject
{
    QJsonObject root({{"version", 2}, {"name", name()}});

    QJsonArray categoryArray;

    foreach (const auto &category, categories())
    {
        QJsonObject categoryObject;
        categoryObject.insert(QStringLiteral("name"), category);

        QJsonArray itemArray;

        foreach (const auto *item, items())
        {
            if (item && (item->category() == category))
            {
                itemArray.append(item->toJson());
            }
        }

        categoryObject.insert(QStringLiteral("items"), itemArray);
        categoryArray.append(categoryObject);
    }

    root.insert(QStringLiteral("categories"), categoryArray);
    return root;
}

auto ItemGroup::findCategories() const -> QStringList
{
    QStringList categories;

    foreach (const auto *item, items())
    {
        if (item && !categories.contains(item->category()))
        {
            categories.append(item->category());
        }
    }

    return categories;
}
