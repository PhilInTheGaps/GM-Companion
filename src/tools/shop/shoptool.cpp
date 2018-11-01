#include "shoptool.h"

#include <QDebug>
#include <QSettings>
#include "src/functions.h"

ShopTool::ShopTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Tool ...";

    sManager = new SettingsManager;
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
    return l_categories;
}

// Load Category List
void ShopTool::loadCategories(QString project)
{
    qDebug() << "Loading categories for shop project:" << project;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    l_categories = settings.value("categories", {}).toStringList();
}

// Load Shop list
void ShopTool::loadShops(QString project)
{
    qDebug() << "Loading shops for category:" << l_category;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    QStringList shops;

    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        shops.append(settings.value("shop").toStringList().at(0));
    }

    settings.endArray();

    l_shops = shops;
}

// Load a shop
void ShopTool::load(QString project, QString shop)
{
    qDebug() << "Loading shop:" << shop;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    l_item_names.clear();
    l_item_prices.clear();
    l_item_descriptions.clear();

    // Get Shop Information
    l_shopName = shop;
    QStringList shopValues;

    int shopCount = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < shopCount; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("shop").toStringList().at(0) == shop) shopValues = settings.value("shop").toStringList();
    }

    settings.endArray();

    l_shopOwner       = shopValues.at(1);
    l_shopDescription = shopValues.at(2);

    // Get Items
    int count = settings.beginReadArray(l_category + "_" + shop + "_items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QStringList item = settings.value("item").toStringList();

        l_item_names.append(item.at(0));
        l_item_prices.append(item.at(1));
        l_item_descriptions.append(item.at(3));
    }

    settings.endArray();

    emit shopNameChanged();
    emit shopOwnerChanged();
    emit shopDescriptionChanged();
    emit itemsChanged();
}

QString ShopTool::category()
{
    return l_category;
}

void ShopTool::setCategory(QString category)
{
    l_category = category;

    emit categoryChanged();
}

QStringList ShopTool::shops()
{
    return l_shops;
}

QString ShopTool::shopName()
{
    return l_shopName;
}

QString ShopTool::shopOwner()
{
    return l_shopOwner;
}

QString ShopTool::shopDescription()
{
    return l_shopDescription;
}

QStringList ShopTool::item_names()
{
    return l_item_names;
}

QStringList ShopTool::item_prices()
{
    return l_item_prices;
}

QStringList ShopTool::item_descriptions()
{
    return l_item_descriptions;
}
