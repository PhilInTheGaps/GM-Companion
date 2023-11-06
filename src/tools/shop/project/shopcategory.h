#pragma once

#include "itemshop.h"
#include "models/baseprojectitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ShopCategory : public BaseProjectItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_LIST_PROPERTY(ItemShop, shops)
    AUTO_PROPERTY_VAL2(ItemShop *, currentShop, nullptr)

public:
    ShopCategory(const QString &name, const QList<ItemShop *> &shops, QObject *parent);
    ShopCategory(const ShopCategory &other, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    void addShop(ItemShop *shop);
    auto deleteShop(ItemShop *shop) -> qsizetype;
    auto moveShop(ItemShop *shop, int positions) -> bool;

private:
    void connectShops() const;
    void connectShop(const ItemShop *shop) const;
    void connectSignals() const;
};
