#ifndef SHOPEDITOR_H
#define SHOPEDITOR_H

#include <QObject>

#include "src/settings/settingsmanager.h"

class ShopEditor : public QObject
{
    Q_OBJECT
public:
    explicit ShopEditor(QObject *parent = nullptr);

    Q_INVOKABLE void setCurrentProject(QString project);
    Q_INVOKABLE void setCurrentCategory(QString category);

    Q_INVOKABLE void createProject(QString project);
    Q_INVOKABLE void createCategory(QString category);
    Q_INVOKABLE void createShop(QString shop);

    Q_INVOKABLE void updateShopList();
    Q_INVOKABLE QStringList getShopList();

    Q_INVOKABLE void loadShop(QString shop);
    Q_INVOKABLE void saveShop();
    Q_INVOKABLE void deleteShop();

    Q_INVOKABLE QStringList getCategories();

    Q_INVOKABLE QString getShopName();
    Q_INVOKABLE QString getShopOwner();
    Q_INVOKABLE void setShopOwner(QString owner);
    Q_INVOKABLE QString getShopDescription();
    Q_INVOKABLE void setShopDescription(QString description);

    Q_INVOKABLE void addItem(QStringList item);
    Q_INVOKABLE QStringList getItemNames();
    Q_INVOKABLE QStringList getItemPrices();
    Q_INVOKABLE QStringList getItemCategories();
    Q_INVOKABLE QStringList getItemDescriptions();

    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void removeAllItems();

    // Item List on the right
    Q_INVOKABLE void loadItemListTabs();
    Q_INVOKABLE QStringList getItemListTabNames();
    Q_INVOKABLE void setItemListTabIndex(int index);

    Q_INVOKABLE void loadItemList();
    Q_INVOKABLE QStringList getItemListNames();
    Q_INVOKABLE QStringList getItemListPrices();
    Q_INVOKABLE QStringList getItemListCategories();
    Q_INVOKABLE QStringList getItemListDescriptions();

    Q_INVOKABLE QStringList getListItem(int index);

signals:

private:
    SettingsManager *sManager;

    QStringList l_shopList;

    QString l_projectPath;
    QString l_category;

    QStringList l_categories;

    QString l_shopName;
    QString l_shopOwner;
    QString l_shopDescription;

    QStringList l_itemNames;
    QStringList l_itemPrices;
    QStringList l_itemCategories;
    QStringList l_itemDescriptions;

    // Item List
    QStringList l_itemListTabNames;
    QStringList l_itemListTabPaths;
    int l_itemListTabIndex = 0;

    QStringList l_itemListNames;
    QStringList l_itemListPrices;
    QStringList l_itemListCategories;
    QStringList l_itemListDescriptions;
};

#endif // SHOPEDITOR_H
