#include "itemshop.h"
#include "utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmShopsItemShop, "gm.shops.project.shop")

ItemShop::ItemShop(const QString &name, const QString &owner, const QString &description, const QList<Item *> &items,
                   QObject *parent)
    : BaseProjectItem(name, parent), a_owner(owner), a_description(description), a_items(items)
{
    qCDebug(gmShopsItemShop) << "Initializing new ItemShop" << name << "...";
    connectSignals();
}

ItemShop::ItemShop(const ItemShop &other, QObject *parent)
    : BaseProjectItem(other.name(), parent), a_owner(other.owner()), a_description(other.description())
{
    foreach (const auto *item, other.items())
    {
        if (item)
        {
            auto *copy = new Item(*item, this);
            a_items.append(copy);
        }
    }

    connectSignals();
}

ItemShop::ItemShop(const QJsonObject &json, QObject *parent) : BaseProjectItem(json["name"_L1].toString(), parent)
{
    auto itemArray = json["items"_L1].toArray();
    a_items.reserve(itemArray.size());

    foreach (const auto &item, itemArray)
    {
        a_items.append(new Item(item.toObject(), this));
    }

    owner(json["owner"_L1].toString());
    description(json["description"_L1].toString());
}

auto ItemShop::toJson() const -> QJsonObject
{
    QJsonObject root;

    root.insert(u"name"_s, name());
    root.insert(u"owner"_s, owner());
    root.insert(u"description"_s, description());

    QJsonArray itemArray;

    foreach (const auto *item, items())
    {
        if (item)
        {
            itemArray.append(item->toJson());
        }
    }
    root.insert(u"items"_s, itemArray);

    return root;
}

void ItemShop::addItem(const Item &item)
{
    auto *copy = new Item(item, this);

    if (auto insert = Item::findLastIndexWithCategory(copy->category(), items()) + 1; insert <= 0)
    {
        a_items.append(copy);
    }
    else
    {
        a_items.insert(insert, copy);
    }

    emit itemsChanged(a_items);
}

auto ItemShop::deleteItem(int index) -> bool
{
    if (!Utils::isInBounds(items(), index)) return false;

    auto *item = a_items.takeAt(index);
    item->deleteLater();

    emit itemsChanged(a_items);
    return true;
}

void ItemShop::connectSignals() const
{
    connect(this, &ItemShop::ownerChanged, this, &ItemShop::wasEdited);
    connect(this, &ItemShop::descriptionChanged, this, &ItemShop::wasEdited);
    connect(this, &ItemShop::itemsChanged, this, &ItemShop::wasEdited);
}
