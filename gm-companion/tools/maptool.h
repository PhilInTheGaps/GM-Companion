#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QObject>
#include <QStringList>
#include "gm-companion/settings/settingsmanager.h"

struct Map
{
    QString category;
    QString name;
    QString path;
    QString icon;
};

class MapTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit MapTool(QObject *parent = 0);
    ~MapTool();

    Q_INVOKABLE void findMaps();
    Q_INVOKABLE QStringList maps(QString category);
    Q_INVOKABLE QStringList mapPaths(QString category);

    QStringList categories();

signals:
    void categoriesChanged();


private:
    SettingsManager *sManager;

    QList<Map> l_maps;
    QStringList l_categories;

};

#endif // MAPVIEWERTOOL_H
