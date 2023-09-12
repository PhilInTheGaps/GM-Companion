#pragma once

#include "models/baseprojectitem.h"
#include "shopcategory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ShopProject : public BaseProjectItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READONLY_PROPERTY2(int, version, 3)
    AUTO_PROPERTY_VAL2(bool, isSaved, true)
    AUTO_LIST_PROPERTY(ShopCategory, categories)
    AUTO_PROPERTY_VAL2(ShopCategory *, currentCategory, nullptr)

public:
    explicit ShopProject(const QString &name, const QList<ShopCategory *> &categories, QObject *parent);
    explicit ShopProject(const ShopProject &other, QObject *parent);
    explicit ShopProject(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    void addCategory(ShopCategory *category);
    auto deleteCategory(ShopCategory *category) -> bool;

signals:
    void currentShopChanged(ItemShop *currentShop);

private slots:
    void onCurrentCategoryChanged(ShopCategory *category);
    void onWasEdited();

private:
    void connectSignals() const;
    void connectCategories();
    void connectCategory(ShopCategory *category) const;

    QMetaObject::Connection m_categoryConnection;
};
