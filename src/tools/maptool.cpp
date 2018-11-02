#include "maptool.h"
#include "src/functions.h"

#include <QDebug>
#include <QSettings>

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

    m_maps.clear();
    m_categories.clear();

    // Get folders
    for (QString f : getFolders(basePath))
    {
        if (!f.contains("."))
        {
            bool containsMaps = false;

            // Get maps
            for (QString m : getFiles(basePath + "/" + f))
            {
                if (m.endsWith(".jpg") || m.endsWith(".jpeg") || m.endsWith(".png"))
                {
                    Map map;
                    map.name     = m;
                    map.path     = basePath + "/" + f + "/" + m;
                    map.category = f;

                    m_maps.append(map);

                    containsMaps = true;
                }
            }

            if (containsMaps) m_categories.append(f);
        }
    }

    // Addon maps
    for (QString path : QStringList({ QDir::homePath() + "/.gm-companion/addons", ":/addons" }))
    {
        for (QString addon : getFolders(path))
        {
            if (sManager->getIsAddonEnabled(addon) && !addon.contains("."))
            {
                if (QDir(path + "/" + addon + "/maps").exists())
                {
                    QSettings settings(path + "/" + addon + "/addon.ini", QSettings::IniFormat);
                    m_categories.append(settings.value("name", addon).toString());

                    for (QString m : getFiles(path + "/" + addon + "/maps"))
                    {
                        if (m.endsWith(".jpg") || m.endsWith(".jpeg") || m.endsWith(".png"))
                        {
                            Map map;
                            map.name     = m;
                            map.path     = path + "/" + addon + "/maps/" + m;
                            map.category = addon;

                            m_maps.append(map);
                        }
                    }
                }
            }
        }
    }

    qDebug() << m_categories;

    emit categoriesChanged();
}

QStringList MapTool::maps(QString category)
{
    QStringList maps;

    for (int i = 0; i < m_maps.size(); i++)
    {
        if (m_maps.at(i).category == category)
        {
            maps.append(m_maps.at(i).name);
        }
    }
    return maps;
}

QStringList MapTool::categories()
{
    return m_categories;
}

QStringList MapTool::mapPaths(QString category)
{
    QStringList paths;

    for (int i = 0; i < m_maps.size(); i++)
    {
        if (m_maps.at(i).category == category)
        {
            paths.append(m_maps.at(i).path);
        }
    }

    return paths;
}
