#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

#include "src/managers/filemanager.h"
#include "src/settings/settingsmanager.h"
#include "map.h"

class MapTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit MapTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~MapTool();

    QStringList categories();
    Q_INVOKABLE void setCurrentCategory(int index);

signals:
    void categoriesChanged();


private:
    SettingsManager sManager;
    FileManager *fileManager;
    QQmlApplicationEngine *qmlEngine;

    QList<MapCategory*> m_categories;
    MapCategory* m_currentCategory = nullptr;
    MapListModel *mapListModel = nullptr;
};

#endif // MAPVIEWERTOOL_H
