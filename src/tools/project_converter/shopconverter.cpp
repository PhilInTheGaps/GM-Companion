#include "shopconverter.h"
#include "settings/settingsmanager.h"
#include "functions.h"

#include <QDebug>
#include <QSettings>
#include "settings/settingsmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../shop/shopproject.h"

ShopConverter::ShopConverter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Converter ...";
}

void ShopConverter::convert()
{
    // ITEMS
    qDebug() << "Converting Items ...";

    QString path = SettingsManager::getPath("shopPath");

    if (QFile(path + "/CustomItems.items").exists())
    {
        QSettings settings(path + "/CustomItems.items", QSettings::IniFormat);

        int file_version = settings.value("version", -1).toInt();

        if (file_version == 0)
        {
            settings.setValue("version", 1);
            file_version = 1;

            QStringList categories;

            int count = settings.beginReadArray("categories");

            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);

                categories.append(settings.value("name").toString());
            }

            settings.endArray();

            settings.setValue("categories", categories);
        }

        if (file_version == 1)
        {
            QStringList categories = settings.value("categories").toStringList();

            QJsonObject project;
            project.insert("version", 2);

            QJsonArray categoryArray;

            for (auto c : categories)
            {
                QJsonArray itemArray;
                int count = settings.beginReadArray("items");

                for (int i = 0; i < count; i++)
                {
                    settings.setArrayIndex(i);

                    if (settings.value("category").toString() == c)
                    {
                        QJsonObject item;
                        item.insert("name",        settings.value("name").toString());
                        item.insert("price",       settings.value("price").toString());
                        item.insert("description", settings.value("description").toString());

                        itemArray.append(item);
                    }
                }

                settings.endArray();

                QJsonObject category;
                category.insert("name",          c);
                category.insert("items", itemArray);

                categoryArray.append(category);
            }

            project.insert("categories", categoryArray);
            QJsonDocument doc(project);

            // Rename old file and save everything to new one
            QString filePath = path + "/CustomItems.items";
            QFile(filePath).rename(filePath + "_old");
            QFile f(filePath);

            if (f.open(QIODevice::WriteOnly))
            {
                f.write(doc.toJson(QJsonDocument::Indented));
                f.close();

                qDebug() << "Successfully converted" << filePath << "to version 2.";
            }
            else
            {
                qDebug() << "Error: Could not open file" << filePath << "!";
            }
        }
    }

    // SHOPS
    qDebug() << "Converting Shops ...";

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".shop"))
        {
            qDebug() << "   Converting Shop:" << file;

            QSettings settings(path + "/" + file, QSettings::IniFormat);

            int file_version = settings.value("version", -1).toInt();

            if (file_version == 0)
            {
                settings.setValue("version", 1);
                file_version = 1;

                QStringList categories;

                int count = settings.beginReadArray("categories");

                for (int i = 0; i < count; i++)
                {
                    settings.setArrayIndex(i);

                    categories.append(settings.value("name").toString());
                }

                settings.endArray();

                settings.setValue("categories", categories);
            }

            if (file_version == 1)
            {
                settings.setValue("version", 2);
                file_version = 2;

                for (QString category : settings.value("categories").toStringList())
                {
                    QList<QStringList> shops;
                    QList<QList<QStringList> > shops_items;

                    // Read all shop information
                    int count = settings.beginReadArray(category + "_shops");

                    for (int i = 0; i < count; i++)
                    {
                        settings.setArrayIndex(i);

                        QStringList shop;
                        shop.append(settings.value("name", "UNKNOWN SHOP").toString());
                        shop.append(settings.value("owner", "").toString());
                        shop.append(settings.value("description", "").toString());
                        shops.append(shop);

                        QList<QStringList> items;

                        // Read all shop items
                        int itemCount = settings.beginReadArray("items");

                        for (int j = 0; j < itemCount; j++)
                        {
                            settings.setArrayIndex(j);

                            QStringList item;
                            item.append(settings.value("name", "UNKNOWN ITEM").toString());
                            item.append(settings.value("price", "").toString());
                            item.append(settings.value("category", "").toString());
                            item.append(settings.value("description", "").toString());
                            items.append(item);
                        }

                        settings.endArray();

                        shops_items.append(items);
                    }

                    settings.endArray();


                    // Save all information in better format
                    settings.beginWriteArray(category + "_shops");

                    for (int i = 0; i < shops.size(); i++)
                    {
                        settings.setArrayIndex(i);

                        settings.setValue("shop", shops.at(i));
                    }

                    settings.endArray();


                    for (int i  = 0; i < shops.size(); i++)
                    {
                        QList<QStringList> items = shops_items.at(i);

                        settings.beginWriteArray(category + "_" + shops.at(i).at(0) + "_items");

                        for (int j = 0; j < items.size(); j++)
                        {
                            settings.setArrayIndex(j);

                            settings.setValue("item", items.at(j));
                        }

                        settings.endArray();
                    }
                }
            }

            if (file_version == 2)
            {
                QList<ShopCategory *> categories;

                for (auto c : settings.value("categories").toStringList())
                {
                    QList<ItemShop *> shops;
                    int count = settings.beginReadArray(c + "_shops");

                    // Get Shops in Category
                    for (int i = 0; i < count; i++)
                    {
                        settings.setArrayIndex(i);

                        QStringList args = settings.value("shop").toStringList();
                        QString     name, owner, description;

                        if (args.size() > 0) name = args[0];

                        if (args.size() > 1) owner = args[1];

                        if (args.size() > 2) description = args[2];

                        shops.append(new ItemShop(name, owner, description));
                    }

                    settings.endArray();

                    // Get Items in Shop
                    for (auto s : shops)
                    {
                        if (s)
                        {
                            QList<Item *> items;
                            count = settings.beginReadArray(c + "_" + s->name() + "_items");

                            for (int i = 0; i < count; i++)
                            {
                                settings.setArrayIndex(i);
                                QStringList args = settings.value("item").toStringList();
                                QString     name, price, category, description;

                                if (args.size() > 0) name = args[0];

                                if (args.size() > 1) price = args[1];

                                if (args.size() > 2) category = args[2];

                                if (args.size() > 3) description = args[3];

                                items.append(new Item(name, price, description, category));
                            }

                            settings.endArray();

                            s->setItems(items);
                        }
                    }

                    categories.append(new ShopCategory(c, shops));
                }

                // Convert to JSON
                QJsonObject project;
                project.insert("name", settings.value("project_name").toString());
                project.insert("version",     3);

                QJsonArray categoryArray;

                for (auto c : categories)
                {
                    if (c)
                    {
                        QJsonObject category;
                        category.insert("name", c->name());

                        QJsonArray shopArray;

                        for (auto s : c->shops())
                        {
                            if (s)
                            {
                                QJsonObject shop;
                                shop.insert("name",        s->name());
                                shop.insert("owner",       s->owner());
                                shop.insert("description", s->description());

                                QJsonArray itemArray;

                                for (auto i : s->items())
                                {
                                    if (i)
                                    {
                                        QJsonObject item;
                                        item.insert("name",        i->name());
                                        item.insert("price",       i->price());
                                        item.insert("description", i->description());
                                        item.insert("category",    i->category());

                                        itemArray.append(item);
                                    }
                                }

                                shop.insert("items", itemArray);
                                shopArray.append(shop);
                            }
                        }

                        category.insert("shops", shopArray);
                        categoryArray.append(category);
                    }
                }

                project.insert("categories", categoryArray);
                QJsonDocument doc(project);

                // Rename old file and save everything to new one
                QString filePath = path + "/" + file;
                QFile(filePath).rename(filePath + "_old");
                QFile f(filePath);

                if (f.open(QIODevice::WriteOnly))
                {
                    f.write(doc.toJson(QJsonDocument::Indented));
                    f.close();

                    qDebug() << "Successfully converted" << filePath << "to version 3.";
                }
                else
                {
                    qWarning() << "Error: Could not open file" << filePath << "!";
                }
            }
        }
    }
}
