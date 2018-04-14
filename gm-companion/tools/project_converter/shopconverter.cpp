#include "shopconverter.h"
#include "gm-companion/settings/settingsmanager.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QSettings>

ShopConverter::ShopConverter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Converter ...";
}

void ShopConverter::convert()
{
    SettingsManager *sManager = new SettingsManager;

    qDebug() << "Converting Items ...";

    QString path = sManager->getSetting(Setting::shopPath);

    if (QFile(path + "/CustomItems.items").exists())
    {
        QSettings settings(path + "/CustomItems.items", QSettings::IniFormat);

        int file_version = settings.value("version", 0).toInt();

        if (file_version == 0)
        {
            settings.setValue("version", 1);

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
    }

    qDebug() << "Converting Shops ...";

    for (QString file : getFiles(path))
    {
        if (file.endsWith(".shop"))
        {
            qDebug() << "   Converting Shop:" << file;

            QSettings settings(path + "/" + file, QSettings::IniFormat);

            int file_version = settings.value("version", 0).toInt();

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
        }
    }
}
