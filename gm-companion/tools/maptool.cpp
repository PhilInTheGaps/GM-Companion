#include "maptool.h"
#include "gm-companion/functions.h"

#include <QDebug>

MapTool::MapTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Map Tool ...";

    sManager = new SettingsManager;
}

MapTool::~MapTool()
{
    delete sManager;
}

void MapTool::findMaps()
{
    qDebug() << "Finding Maps ...";
    QString basePath = sManager->getSetting(Setting::mapsPath);

    l_maps.clear();
    l_categories.clear();

    // Custom maps in maps folder
    for (QString m : getFiles(basePath))
    {
        if (m.endsWith(".jpg") || m.endsWith(".jpeg") || m.endsWith(".png"))
        {
            Map map;
            map.name     = m;
            map.path     = basePath + "/" + m;
            map.category = "Custom";

            l_maps.append(map);
        }
    }
    l_categories.append("Custom");

    // Addon maps
    QString addonBasePath = QDir::homePath() + "/.gm-companion/addons";

    for (QString addon : getFolders(addonBasePath))
    {
        if (sManager->getIsAddonEnabled(addon) && !addon.contains("."))
        {
            if (QDir(addonBasePath + "/" + addon + "/maps").exists())
            {
                l_categories.append(addon);

                for (QString m : getFiles(addonBasePath + "/" + addon + "/maps"))
                {
                    if (m.endsWith(".jpg") || m.endsWith(".jpeg") || m.endsWith(".png"))
                    {
                        Map map;
                        map.name     = m;
                        map.path     = addonBasePath + "/" + addon + "/maps/" + m;
                        map.category = addon;

                        l_maps.append(map);
                    }
                }
            }
        }
    }

    emit categoriesChanged();
}

QStringList MapTool::maps(QString category)
{
    QStringList maps;

    for (int i = 0; i < l_maps.size(); i++)
    {
        if (l_maps.at(i).category == category)
        {
            maps.append(l_maps.at(i).name);
        }
    }
    return maps;
}

QStringList MapTool::categories()
{
    return l_categories;
}

QStringList MapTool::mapPaths(QString category)
{
    QStringList paths;

    for (int i = 0; i < l_maps.size(); i++)
    {
        if (l_maps.at(i).category == category)
        {
            paths.append(l_maps.at(i).path);
        }
    }

    return paths;
}
