#pragma once

#include "item.h"
#include "models/baseprojectitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class ItemShop : public BaseProjectItem
{
    Q_OBJECT
    AUTO_PROPERTY(QString, owner)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QList<Item *>, items)

public:
    explicit ItemShop(const QString &name, const QString &owner, const QString &description, const QList<Item *> &items,
                      QObject *parent);
    explicit ItemShop(const ItemShop &other, QObject *parent);
    explicit ItemShop(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    void addItem(const Item &item);
    auto deleteItem(int index) -> bool;

private:
    void connectSignals();
};
