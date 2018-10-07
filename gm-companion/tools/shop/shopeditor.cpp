#include "shopeditor.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "gm-companion/functions.h"

ShopEditor::ShopEditor(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Editor ...";

    sManager = new SettingsManager;
}

// Set the project file path
void ShopEditor::setCurrentProject(QString project)
{
    l_projectPath = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(l_projectPath, QSettings::IniFormat);

    l_categories = settings.value("categories", {}).toStringList();
    l_shopList.clear();
}

// Set the current category
void ShopEditor::setCurrentCategory(QString category)
{
    l_category = category;
}

// Load list of all available shops
void ShopEditor::updateShopList()
{
    QSettings settings(l_projectPath, QSettings::IniFormat);

    QStringList list;

    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        list.append(settings.value("shop").toStringList().at(0));
    }

    settings.endArray();

    l_shopList = list;
}

// Return list of all shops in the category
QStringList ShopEditor::getShopList()
{
    return l_shopList;
}

// Load all data of a shop
void ShopEditor::loadShop(QString shop)
{
    qDebug() << "Shop Editor: Loading Shop:" << shop << "...";

    QSettings settings(l_projectPath, QSettings::IniFormat);

    // Load Basic Shop Information
    l_shopName = shop;
    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("shop").toStringList().at(0) == shop)
        {
            l_shopOwner       = settings.value("shop").toStringList().at(1);
            l_shopDescription = settings.value("shop").toStringList().at(2);
        }
    }

    settings.endArray();

    // Load Items
    l_itemNames.clear();
    l_itemPrices.clear();
    l_itemCategories.clear();
    l_itemDescriptions.clear();

    count = settings.beginReadArray(l_category + "_" + shop + "_items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QStringList item = settings.value("item").toStringList();

        l_itemNames.append(item.at(0));
        l_itemPrices.append(item.at(1));
        l_itemCategories.append(item.at(2));
        l_itemDescriptions.append(item.at(3));
    }

    settings.endArray();
}

// Save Shop to project
void ShopEditor::saveShop()
{
    qDebug() << "Shop Editor: Saving Shop:" << l_shopName << "...";

    if (!l_shopName.isNull() && (l_shopName != ""))
    {
        QSettings settings(l_projectPath, QSettings::IniFormat);

        int shopIndex = l_shopList.indexOf(l_shopName);

        // Save Shop Info
        settings.beginWriteArray(l_category + "_shops");
        settings.setArrayIndex(shopIndex);

        QStringList shop = { l_shopName, l_shopOwner, l_shopDescription };
        settings.setValue("shop", shop);

        settings.endArray();

        // Save Items
        settings.beginWriteArray(l_category + "_" + l_shopName + "_items");

        for (int i = 0; i < l_itemNames.size(); i++)
        {
            settings.setArrayIndex(i);

            QStringList item;
            item.append(l_itemNames[i]);
            item.append(l_itemPrices[i]);
            item.append(l_itemCategories[i]);
            item.append(l_itemDescriptions[i]);

            settings.setValue("item", item);
        }

        settings.endArray();
    }
}

// Delete the current Shop
void ShopEditor::deleteShop()
{
    qDebug() << "Deleting Shop:" << l_shopName << "...";

    QSettings settings(l_projectPath, QSettings::IniFormat);

    int index = l_shopList.indexOf(l_shopName);

    // Get list of all shops
    QList<QStringList> shops;

    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        shops.append(settings.value("shop").toStringList());
    }

    settings.endArray();

    // Remove current shop from list and write all others back to file
    if (shops.size() > index)
    {
        shops.removeAt(index);

        settings.beginWriteArray(l_category + "_shops");

        for (int i = 0; i < shops.size(); i++)
        {
            settings.setArrayIndex(i);

            settings.setValue("shop", shops.at(i));
        }

        settings.endArray();
    }
}

// Remove an Item from the Shop
void ShopEditor::removeItem(int index)
{
    qDebug() << "Removing Item:" << l_itemNames.at(index);

    l_itemNames.removeAt(index);
    l_itemPrices.removeAt(index);
    l_itemCategories.removeAt(index);
    l_itemDescriptions.removeAt(index);
}

// Remove all items
void ShopEditor::removeAllItems()
{
    qDebug() << "Removing All Items ...";

    l_itemNames.clear();
    l_itemPrices.clear();
    l_itemCategories.clear();
    l_itemDescriptions.clear();
}

// Create a new Project file
void ShopEditor::createProject(QString project)
{
    qDebug() << "Creating New Shop Project:" << project << "...";

    QSettings settings(sManager->getSetting(Setting::shopPath) + "/" + project + ".shop", QSettings::IniFormat);

    settings.setValue("project_name", project);
    settings.setValue("version",      2);
}

// Create a new Category
void ShopEditor::createCategory(QString category)
{
    qDebug() << "Creating New Category:" << category << "...";

    if (!l_categories.contains(category))
    {
        l_categories.append(category);

        QSettings settings(l_projectPath, QSettings::IniFormat);

        settings.setValue("categories", l_categories);
    }
}

QStringList ShopEditor::getCategories()
{
    return l_categories;
}

// Create a new Shop
void ShopEditor::createShop(QString shop)
{
    qDebug() << "Creating New Shop:" << shop << "...";

    if ((l_category != "") && !l_category.isNull())
    {
        QSettings settings(l_projectPath, QSettings::IniFormat);

        int index = l_shopList.size();

        settings.beginWriteArray(l_category + "_shops");
        settings.setArrayIndex(index);

        QStringList shopValues = { shop, "", "" };
        settings.setValue("shop", shopValues);

        settings.endArray();

        l_shopList.append(shop);
    }
    else
    {
        qDebug() << "Error: Cannot create shop, no category available!";
    }
}

QString ShopEditor::getShopName()
{
    return l_shopName;
}

QString ShopEditor::getShopOwner()
{
    return l_shopOwner;
}

void ShopEditor::setShopOwner(QString owner)
{
    l_shopOwner = owner;
}

QString ShopEditor::getShopDescription()
{
    return l_shopDescription;
}

void ShopEditor::setShopDescription(QString description)
{
    l_shopDescription = description;
}

QStringList ShopEditor::getItemNames()
{
    return l_itemNames;
}

QStringList ShopEditor::getItemPrices()
{
    return l_itemPrices;
}

QStringList ShopEditor::getItemCategories()
{
    return l_itemCategories;
}

QStringList ShopEditor::getItemDescriptions()
{
    return l_itemDescriptions;
}

// Load list of all item files
void ShopEditor::loadItemListTabs()
{
    l_itemListTabNames.clear();
    l_itemListTabNames.append("Custom");
    l_itemListTabPaths.clear();
    l_itemListTabPaths.append(sManager->getSetting(Setting::shopPath) + "/CustomItems.items");

    for (QString path : QStringList({ QDir::homePath() + "/.gm-companion/addons", ":/addons" }))
    {
        for (QString addon : getFolders(path))
        {
            if (sManager->getIsAddonEnabled(addon))
            {
                QString addonItemPath = path + "/" + addon + "/shop/AddonItems.items";

                if (QFile(addonItemPath).exists())
                {
                    QSettings settings(path + "/" + addon + "/addon.ini", QSettings::IniFormat);
                    l_itemListTabNames.append(settings.value("name", addon).toString());
                    l_itemListTabPaths.append(addonItemPath);
                }
            }
        }
    }
}

QStringList ShopEditor::getItemListTabNames()
{
    return l_itemListTabNames;
}

void ShopEditor::setItemListTabIndex(int index)
{
    l_itemListTabIndex = index;
}

// Load all items of item list
void ShopEditor::loadItemList()
{
    qDebug() << "Loading Items of Type:" << l_itemListTabNames.at(l_itemListTabIndex);

    l_itemListNames.clear();
    l_itemListPrices.clear();
    l_itemListCategories.clear();
    l_itemListDescriptions.clear();

    QSettings settings(l_itemListTabPaths.at(l_itemListTabIndex), QSettings::IniFormat);

    int count = settings.beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        l_itemListNames.append(settings.value("name").toString());
        l_itemListPrices.append(settings.value("price").toString());
        l_itemListCategories.append(settings.value("category").toString());
        l_itemListDescriptions.append(settings.value("description").toString());
    }

    settings.endArray();
}

// Return Item at index
QStringList ShopEditor::getListItem(int index)
{
    QStringList item;

    item.append(l_itemListNames.at(index));
    item.append(l_itemListPrices.at(index));
    item.append(l_itemListCategories.at(index));
    item.append(l_itemListDescriptions.at(index));

    return item;
}

// Add an item to the shop
void ShopEditor::addItem(QStringList item)
{
    l_itemNames.append(item.at(0));
    l_itemPrices.append(item.at(1));
    l_itemCategories.append(item.at(2));
    l_itemDescriptions.append(item.at(3));
}

QStringList ShopEditor::getItemListNames()
{
    return l_itemListNames;
}

QStringList ShopEditor::getItemListPrices()
{
    return l_itemListPrices;
}

QStringList ShopEditor::getItemListCategories()
{
    return l_itemListCategories;
}

QStringList ShopEditor::getItemListDescriptions()
{
    return l_itemListDescriptions;
}
