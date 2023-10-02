#pragma once

#include "models/baseprojectitem.h"
#include "models/customobjectlistmodel.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class Item : public BaseProjectItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ShopItem)
    QML_UNCREATABLE("")

    AUTO_PROPERTY(QString, price)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QString, category)

public:
    Item(const QString &name, const QString &price, const QString &description, const QString &category,
         QObject *parent);
    Item(const Item &other, QObject *parent);
    Item(const QJsonObject &json, QObject *parent);
    Item(const QString &category, const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    static auto findLastIndexWithCategory(const QString &category, const QList<Item *> &items) -> int;
};

class ItemModel : public CustomObjectListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using CustomObjectListModel::CustomObjectListModel;

    enum class Roles
    {
        Name = Qt::UserRole,
        Price = Qt::UserRole + 1,
        Description = Qt::UserRole + 2,
        Category = Qt::UserRole + 3,
    };

    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;
};
