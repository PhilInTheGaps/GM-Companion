#include "maptool.h"
#include "src/functions.h"
#include "src/cloud/googledrive.h"

#include <QDebug>
#include <QSettings>
#include <QQmlContext>

MapTool::MapTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Map Tool ...";

    connect(fManager->getMapsFileManger(), &MapsFileManager::mapsChanged, [ = ](QList<MapCategory *>categories) {
        m_categories = categories;

        if (m_categories.size() > 0) setCurrentCategory(m_categories.size() - 1);
        emit categoriesChanged();
    });

    mapListModel = new MapListModel;
    qmlEngine->rootContext()->setContextProperty("mapListModel", mapListModel);

    fManager->getMapsFileManger()->findMaps(fManager->getModeInt());
}

MapTool::~MapTool()
{
    for (auto c : m_categories)
    {
        if (c) c->deleteLater();
    }
}

QStringList MapTool::categories()
{
    QStringList list;

    for (auto c : m_categories)
    {
        if (c) list.append(c->name());
    }

    return list;
}

void MapTool::setCurrentCategory(int index)
{
    qDebug() << "MapTool: Setting current category" << index;

    if (index < m_categories.size())
    {
        m_currentCategory = m_categories[index];
        mapListModel->setElements(m_currentCategory->maps());
        fileManager->getMapsFileManger()->findMapPaths(index);
    }
    else
    {
        m_currentCategory = nullptr;
        mapListModel->clear();
    }
}
