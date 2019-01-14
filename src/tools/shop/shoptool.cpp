#include "shoptool.h"

#include <QDebug>
#include <QSettings>
#include "src/functions.h"

ShopTool::ShopTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Tool ...";

    shopEditor = new ShopEditor;
    sManager   = new SettingsManager;
}

QStringList ShopTool::projects()
{
    QStringList project_list;

    QString path = sManager->getSetting(Setting::shopPath);

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".shop"))
        {
            project_list.append(file.replace(".shop", ""));
        }
    }

    return project_list;
}

QStringList ShopTool::categories()
{
    return m_categories;
}

// Load Category List
void ShopTool::loadCategories(QString project)
{
    qDebug() << "Loading categories for shop project:" << project;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    m_categories = settings.value("categories", {}).toStringList();
}

// Load Shop list
void ShopTool::loadShops(QString project)
{
    qDebug() << "Loading shops for category:" << m_category;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    QStringList shops;

    int count = settings.beginReadArray(m_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        shops.append(settings.value("shop").toStringList().at(0));
    }

    settings.endArray();

    m_shops = shops;
}

// Load a shop
void ShopTool::load(QString project, QString shop)
{
    qDebug() << "Loading shop:" << shop;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    m_item_names.clear();
    m_item_prices.clear();
    m_item_descriptions.clear();

    // Get Shop Information
    m_shopName = shop;
    QStringList shopValues;

    int shopCount = settings.beginReadArray(m_category + "_shops");

    for (int i = 0; i < shopCount; i++)
    {
        settings.setArrayIndex(i);

        QStringList tempList = settings.value("shop").toStringList();

        if ((tempList.size() > 0) && (tempList[0] == shop))
        {
            shopValues = tempList;
        }
    }

    settings.endArray();

    if (shopValues.size() > 1) m_shopOwner = shopValues.at(1);

    if (shopValues.size() > 2) m_shopDescription = shopValues.at(2);

    // Get Items
    int count = settings.beginReadArray(m_category + "_" + shop + "_items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QStringList item = settings.value("item").toStringList();

        if (item.size() > 0) m_item_names.append(item[0]);

        if (item.size() > 1) m_item_prices.append(item[1]);

        if (item.size() > 2) m_item_descriptions.append(item[2]);
    }

    settings.endArray();

    emit shopNameChanged();
    emit shopOwnerChanged();
    emit shopDescriptionChanged();
    emit itemsChanged();
}

QString ShopTool::category()
{
    return m_category;
}

void ShopTool::setCategory(QString category)
{
    m_category = category;

    emit categoryChanged();
}

QStringList ShopTool::shops()
{
    return m_shops;
}

QString ShopTool::shopName()
{
    return m_shopName;
}

QString ShopTool::shopOwner()
{
    return m_shopOwner;
}

QString ShopTool::shopDescription()
{
    return m_shopDescription;
}

QStringList ShopTool::item_names()
{
    return m_item_names;
}

QStringList ShopTool::item_prices()
{
    return m_item_prices;
}

QStringList ShopTool::item_descriptions()
{
    return m_item_descriptions;
}
