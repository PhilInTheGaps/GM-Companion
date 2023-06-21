#pragma once

#include "item.h"
#include "models/baseprojectitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class ItemGroup : public BaseProjectItem
{
    Q_OBJECT
    AUTO_PROPERTY(QList<Item *>, items)
    READONLY_PROPERTY(QStringList, categories)

public:
    ItemGroup(const QString &name, const QList<Item *> &items, QObject *parent);
    ItemGroup(const ItemGroup &other, QObject *parent);
    ItemGroup(const QString &name, const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

private:
    [[nodiscard]] auto findCategories() const -> QStringList;
};
