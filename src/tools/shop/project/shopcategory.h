#pragma once

#include "itemshop.h"
#include "models/baseprojectitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class ShopCategory : public BaseProjectItem
{
    Q_OBJECT
    AUTO_PROPERTY(QList<ItemShop *>, shops)
    AUTO_PROPERTY_VAL2(ItemShop *, currentShop, nullptr)

public:
    ShopCategory(const QString &name, const QList<ItemShop *> &shops, QObject *parent);
    ShopCategory(const ShopCategory &other, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    void addShop(ItemShop *shop);
    auto deleteShop(ItemShop *shop) -> int;
    auto moveShop(ItemShop *shop, int positions) -> bool;

private:
    void connectShops();
    void connectShop(ItemShop *shop);
    void connectSignals();
};
