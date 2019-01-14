#ifndef SHOPEDITOR_H
#define SHOPEDITOR_H

#include <QObject>

#include "itemeditor.h"
#include "src/settings/settingsmanager.h"

class ShopEditor : public QObject
{
    Q_OBJECT
public:
    explicit ShopEditor(QObject *parent = nullptr);

    ItemEditor* getItemEditor() const { return itemEditor; }

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
    ItemEditor *itemEditor;

    QStringList m_shopList;

    QString m_projectPath;
    QString m_category;

    QStringList m_categories;

    QString m_shopName;
    QString m_shopOwner;
    QString m_shopDescription;

    QStringList m_itemNames;
    QStringList m_itemPrices;
    QStringList m_itemCategories;
    QStringList m_itemDescriptions;

    // Item List
    QStringList m_itemListTabNames;
    QStringList m_itemListTabPaths;
    int m_itemListTabIndex = 0;

    QStringList m_itemListNames;
    QStringList m_itemListPrices;
    QStringList m_itemListCategories;
    QStringList m_itemListDescriptions;
};

#endif // SHOPEDITOR_H
