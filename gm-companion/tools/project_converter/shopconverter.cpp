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
        qDebug() << "   Converting Shop:" << file;

        if (file.endsWith(".shop"))
        {
            QSettings settings(path + "/" + file, QSettings::IniFormat);

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
    }
}
