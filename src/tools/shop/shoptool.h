#pragma once

#include <QQmlApplicationEngine>
#include <QStringList>

#include "baseshoptool.h"
#include "project/shopproject.h"
#include "shopeditor.h"

class ShopTool : public BaseShopTool
{
    Q_OBJECT

public:
    explicit ShopTool(QQmlApplicationEngine *engine = nullptr, QObject *parent = nullptr);

    Q_PROPERTY(ItemModel *itemModel READ itemModel CONSTANT)
    [[nodiscard]] auto itemModel() -> ItemModel *;

    Q_PROPERTY(ShopEditor *editor READ editor CONSTANT)
    [[nodiscard]] auto editor() -> ShopEditor *;

private:
    ShopEditor m_shopEditor;
    ItemModel m_itemModel = ItemModel(false, nullptr);
    QMetaObject::Connection m_projectConnection;

private slots:
    void onCurrentProjectChanged(ShopProject *project);
    void onCurrentShopChanged(ItemShop *shop);
    void onShopEditorSaved(const QList<ShopProject *> &projects);
};
