#pragma once

#include "baseshoptool.h"
#include "project/shopproject.h"
#include "shopeditor.h"
#include <QJSEngine>
#include <QQmlEngine>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class ShopTool : public BaseShopTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    ShopTool() = delete;
    explicit ShopTool(QObject *parent = nullptr);
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> ShopTool *;

    Q_PROPERTY(ItemModel *itemModel READ itemModel CONSTANT) [[nodiscard]] auto itemModel() -> ItemModel *;

    Q_PROPERTY(ShopEditor *editor READ editor CONSTANT)
    [[nodiscard]] auto editor() -> ShopEditor *;

private:
    ShopEditor m_shopEditor;
    ItemModel m_itemModel = ItemModel(false, nullptr);
    QMetaObject::Connection m_projectConnection;

private slots:
    void onCurrentProjectChanged(const ShopProject *project);
    void onCurrentShopChanged(const ItemShop *shop);
    void onShopEditorSaved(const QList<ShopProject *> &projects);
};
