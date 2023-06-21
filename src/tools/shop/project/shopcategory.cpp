#include "shopcategory.h"
#include "utils/utils.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmShopsShopCategory, "gm.shops.project.category")

ShopCategory::ShopCategory(const QString &name, const QList<ItemShop *> &shops, QObject *parent)
    : BaseProjectItem(name, parent), a_shops(shops)
{
    qCDebug(gmShopsShopCategory) << "Initializing new ShopCategory:" << name << "...";

    connectSignals();
    connectShops();

    if (!a_shops.isEmpty()) currentShop(a_shops.constFirst());
}

ShopCategory::ShopCategory(const ShopCategory &other, QObject *parent) : BaseProjectItem(other.name(), parent)
{
    foreach (const auto *shop, other.shops())
    {
        if (shop)
        {
            auto *copy = new ItemShop(*shop, this);
            connectShop(copy);
            a_shops.append(copy);
        }
    }

    connectSignals();

    if (!a_shops.isEmpty()) currentShop(a_shops.constFirst());
}

auto ShopCategory::toJson() const -> QJsonObject
{
    QJsonObject root({{"name", name()}});
    QJsonArray shopArray;

    foreach (const auto *shop, shops())
    {
        if (shop) shopArray.append(shop->toJson());
    }
    root.insert(QStringLiteral("shops"), shopArray);

    return root;
}

void ShopCategory::addShop(ItemShop *shop)
{
    shop->setParent(this);
    a_shops.append(shop);
    emit shopsChanged(a_shops);

    if (!currentShop()) currentShop(shop);
}

/// Delete a shop, returns the shop's index
auto ShopCategory::deleteShop(ItemShop *shop) -> int
{
    const int index = a_shops.indexOf(shop);
    if (index < 0) return index;

    a_shops.removeAt(index);
    emit shopsChanged(a_shops);

    shop->deleteLater();
    return index;
}

auto ShopCategory::moveShop(ItemShop *shop, int positions) -> bool
{
    const int index = a_shops.indexOf(shop);

    if (!Utils::isInBounds(a_shops, index + positions)) return false;

    a_shops.move(index, index + positions);
    emit shopsChanged(a_shops);
    return true;
}

void ShopCategory::connectShops()
{
    foreach (auto *shop, shops())
    {
        connectShop(shop);
    }
}

void ShopCategory::connectShop(ItemShop *shop)
{
    connect(shop, &ItemShop::wasEdited, this, &ShopCategory::wasEdited);
}

void ShopCategory::connectSignals()
{
    connect(this, &ShopCategory::shopsChanged, this, &ShopCategory::wasEdited);
}
