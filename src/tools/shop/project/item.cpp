#include "item.h"

Item::Item(const QString &name, const QString &price, const QString &description, const QString &category,
           QObject *parent)
    : BaseProjectItem(name, parent), a_price(price), a_description(description), a_category(category)
{
    connect(this, &Item::priceChanged, this, &Item::wasEdited);
    connect(this, &Item::descriptionChanged, this, &Item::wasEdited);
    connect(this, &Item::categoryChanged, this, &Item::wasEdited);
}

Item::Item(const Item &other, QObject *parent)
    : Item(other.name(), other.price(), other.description(), other.category(), parent)
{
}

Item::Item(const QJsonObject &json, QObject *parent)
    : Item(json[QStringLiteral("name")].toString(), json[QStringLiteral("price")].toString(),
           json[QStringLiteral("description")].toString(), json[QStringLiteral("category")].toString(), parent)
{
}

Item::Item(const QString &category, const QJsonObject &json, QObject *parent)
    : Item(json[QStringLiteral("name")].toString(), json[QStringLiteral("price")].toString(),
           json[QStringLiteral("description")].toString(), category, parent)
{
}

auto Item::toJson() const -> QJsonObject
{
    return QJsonObject{{"name", name()}, {"price", price()}, {"description", description()}, {"category", category()}};
}

auto Item::findLastIndexWithCategory(const QString &category, const QList<Item *> &items) -> int
{
    for (int i = items.size() - 1; i >= 0; i--)
    {
        if (items.at(i) && items.at(i)->category() == category) return i;
    }

    return -1;
}

auto ItemModel::data(const QModelIndex &index, int role) const -> QVariant
{
    const auto *item = qobject_cast<Item *>(get(index.row()));

    if (!item) return {};

    if (role == Qt::DisplayRole) return item->name();

    if (role < Qt::UserRole) return {};

    switch (static_cast<Roles>(role))
    {
    case Roles::Name:
        return item->name();
    case Roles::Price:
        return item->price();
    case Roles::Description:
        return item->description();
    case Roles::Category:
        return item->category();
    default:
        return {};
    }
}

auto ItemModel::roleNames() const -> QHash<int, QByteArray>
{
    auto roles = CustomObjectListModel::roleNames();
    roles[static_cast<int>(Roles::Name)] = "name";
    roles[static_cast<int>(Roles::Price)] = "price";
    roles[static_cast<int>(Roles::Description)] = "description";
    roles[static_cast<int>(Roles::Category)] = "category";
    return roles;
}
