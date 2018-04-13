#include "shoptool.h"

#include <QDebug>
#include <QSettings>
#include "gm-companion/functions.h"

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

    qDebug() << l_categories;
}

// Load Shop list
void ShopTool::loadShops(QString project)
{
    qDebug() << "Loading shops for category:" << l_category;

    QString path = sManager->getSetting(Setting::shopPath) + "/" + project + ".shop";

    QSettings settings(path, QSettings::IniFormat);

    QStringList shop_list;

    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        shop_list.append(settings.value("name", tr("UNKNOWN SHOP")).toString());
    }

    settings.endArray();

    l_shops = shop_list;

    qDebug() << l_shops;
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

    int count = settings.beginReadArray(l_category + "_shops");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("name", tr("UNKNOWN SHOP")).toString() == shop)
        {
            l_shopName        = shop;
            l_shopOwner       = settings.value("owner", tr("UNKNOWN OWNER")).toString();
            l_shopDescription = settings.value("description", tr("No description available")).toString();

            int item_count = settings.beginReadArray("items");

            for (int j = 0; j < item_count; j++)
            {
                settings.setArrayIndex(j);

                l_item_names.append(settings.value("name", tr("UNKNOWN ITEM")).toString());
                l_item_prices.append(settings.value("price", "/").toString());
                l_item_descriptions.append(settings.value("description", "").toString());
            }

            settings.endArray();
        }
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
