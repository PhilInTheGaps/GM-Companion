#include "shopeditor.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "src/functions.h"

ShopEditor::ShopEditor(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Editor ...";

    sManager = new SettingsManager;
}

// Set the project file path
void ShopEditor::setCurrentProject(QString project)
{
    m_projectPath = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(m_projectPath, QSettings::IniFormat);

    m_categories = settings.value("categories", {}).toStringList();
    m_shopList.clear();
}

// Set the current category
void ShopEditor::setCurrentCategory(QString category)
{
    m_category = category;
}

// Load list of all available shops
void ShopEditor::updateShopList()
{
    QSettings settings(m_projectPath, QSettings::IniFormat);

    QStringList list;

    int count = settings.beginReadArray(m_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        list.append(settings.value("shop").toStringList().at(0));
    }

    settings.endArray();

    m_shopList = list;
}

// Return list of all shops in the category
QStringList ShopEditor::getShopList()
{
    return m_shopList;
}

// Load all data of a shop
void ShopEditor::loadShop(QString shop)
{
    qDebug() << "Shop Editor: Loading Shop:" << shop << "...";

    QSettings settings(m_projectPath, QSettings::IniFormat);

    // Load Basic Shop Information
    m_shopName = shop;
    int count = settings.beginReadArray(m_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("shop").toStringList().at(0) == shop)
        {
            m_shopOwner       = settings.value("shop").toStringList().at(1);
            m_shopDescription = settings.value("shop").toStringList().at(2);
        }
    }

    settings.endArray();

    // Load Items
    m_itemNames.clear();
    m_itemPrices.clear();
    m_itemCategories.clear();
    m_itemDescriptions.clear();

    count = settings.beginReadArray(m_category + "_" + shop + "_items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QStringList item = settings.value("item").toStringList();

        m_itemNames.append(item.at(0));
        m_itemPrices.append(item.at(1));
        m_itemCategories.append(item.at(2));
        m_itemDescriptions.append(item.at(3));
    }

    settings.endArray();
}

// Save Shop to project
void ShopEditor::saveShop()
{
    qDebug() << "Shop Editor: Saving Shop:" << m_shopName << "...";

    if (!m_shopName.isNull() && (m_shopName != ""))
    {
        QSettings settings(m_projectPath, QSettings::IniFormat);

        int shopIndex = m_shopList.indexOf(m_shopName);

        // Save Shop Info
        settings.beginWriteArray(m_category + "_shops");
        settings.setArrayIndex(shopIndex);

        QStringList shop = { m_shopName, m_shopOwner, m_shopDescription };
        settings.setValue("shop", shop);

        settings.endArray();

        // Save Items
        settings.beginWriteArray(m_category + "_" + m_shopName + "_items");

        for (int i = 0; i < m_itemNames.size(); i++)
        {
            settings.setArrayIndex(i);

            QStringList item;
            item.append(m_itemNames[i]);
            item.append(m_itemPrices[i]);
            item.append(m_itemCategories[i]);
            item.append(m_itemDescriptions[i]);

            settings.setValue("item", item);
        }

        settings.endArray();
    }
}

// Delete the current Shop
void ShopEditor::deleteShop()
{
    qDebug() << "Deleting Shop:" << m_shopName << "...";

    QSettings settings(m_projectPath, QSettings::IniFormat);

    int index = m_shopList.indexOf(m_shopName);

    // Get list of all shops
    QList<QStringList> shops;

    int count = settings.beginReadArray(m_category + "_shops");

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

        settings.beginWriteArray(m_category + "_shops");

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
    qDebug() << "Removing Item:" << m_itemNames.at(index);

    m_itemNames.removeAt(index);
    m_itemPrices.removeAt(index);
    m_itemCategories.removeAt(index);
    m_itemDescriptions.removeAt(index);
}

// Remove all items
void ShopEditor::removeAllItems()
{
    qDebug() << "Removing All Items ...";

    m_itemNames.clear();
    m_itemPrices.clear();
    m_itemCategories.clear();
    m_itemDescriptions.clear();
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

    if (!m_categories.contains(category))
    {
        m_categories.append(category);

        QSettings settings(m_projectPath, QSettings::IniFormat);

        settings.setValue("categories", m_categories);
    }
}

QStringList ShopEditor::getCategories()
{
    return m_categories;
}

// Create a new Shop
void ShopEditor::createShop(QString shop)
{
    qDebug() << "Creating New Shop:" << shop << "...";

    if ((m_category != "") && !m_category.isNull())
    {
        QSettings settings(m_projectPath, QSettings::IniFormat);

        int index = m_shopList.size();

        settings.beginWriteArray(m_category + "_shops");
        settings.setArrayIndex(index);

        QStringList shopValues = { shop, "", "" };
        settings.setValue("shop", shopValues);

        settings.endArray();

        m_shopList.append(shop);
    }
    else
    {
        qDebug() << "Error: Cannot create shop, no category available!";
    }
}

QString ShopEditor::getShopName()
{
    return m_shopName;
}

QString ShopEditor::getShopOwner()
{
    return m_shopOwner;
}

void ShopEditor::setShopOwner(QString owner)
{
    m_shopOwner = owner;
}

QString ShopEditor::getShopDescription()
{
    return m_shopDescription;
}

void ShopEditor::setShopDescription(QString description)
{
    m_shopDescription = description;
}

QStringList ShopEditor::getItemNames()
{
    return m_itemNames;
}

QStringList ShopEditor::getItemPrices()
{
    return m_itemPrices;
}

QStringList ShopEditor::getItemCategories()
{
    return m_itemCategories;
}

QStringList ShopEditor::getItemDescriptions()
{
    return m_itemDescriptions;
}

// Load list of all item files
void ShopEditor::loadItemListTabs()
{
    m_itemListTabNames.clear();
    m_itemListTabNames.append("Custom");
    m_itemListTabPaths.clear();
    m_itemListTabPaths.append(sManager->getSetting(Setting::shopPath) + "/CustomItems.items");

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
                    m_itemListTabNames.append(settings.value("name", addon).toString());
                    m_itemListTabPaths.append(addonItemPath);
                }
            }
        }
    }
}

QStringList ShopEditor::getItemListTabNames()
{
    return m_itemListTabNames;
}

void ShopEditor::setItemListTabIndex(int index)
{
    m_itemListTabIndex = index;
}

// Load all items of item list
void ShopEditor::loadItemList()
{
    qDebug() << "Loading Items of Type:" << m_itemListTabNames.at(m_itemListTabIndex);

    m_itemListNames.clear();
    m_itemListPrices.clear();
    m_itemListCategories.clear();
    m_itemListDescriptions.clear();

    QSettings settings(m_itemListTabPaths.at(m_itemListTabIndex), QSettings::IniFormat);

    int count = settings.beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        m_itemListNames.append(settings.value("name").toString());
        m_itemListPrices.append(settings.value("price").toString());
        m_itemListCategories.append(settings.value("category").toString());
        m_itemListDescriptions.append(settings.value("description").toString());
    }

    settings.endArray();
}

// Return Item at index
QStringList ShopEditor::getListItem(int index)
{
    QStringList item;

    item.append(m_itemListNames.at(index));
    item.append(m_itemListPrices.at(index));
    item.append(m_itemListCategories.at(index));
    item.append(m_itemListDescriptions.at(index));

    return item;
}

// Add an item to the shop
void ShopEditor::addItem(QStringList item)
{
    m_itemNames.append(item.at(0));
    m_itemPrices.append(item.at(1));
    m_itemCategories.append(item.at(2));
    m_itemDescriptions.append(item.at(3));
}

QStringList ShopEditor::getItemListNames()
{
    return m_itemListNames;
}

QStringList ShopEditor::getItemListPrices()
{
    return m_itemListPrices;
}

QStringList ShopEditor::getItemListCategories()
{
    return m_itemListCategories;
}

QStringList ShopEditor::getItemListDescriptions()
{
    return m_itemListDescriptions;
}
