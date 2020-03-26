#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

#include "managers/filemanager.h"
#include "map.h"

class MapTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(int mapIndex READ mapIndex WRITE setMapIndex NOTIFY mapIndexChanged)
    Q_PROPERTY(int markerIndex READ markerIndex WRITE setMarkerIndex NOTIFY markerIndexChanged)

public:
    explicit MapTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~MapTool();

    QStringList categories();
    Q_INVOKABLE void setCurrentCategory(int index);

    int mapIndex() const { return m_mapIndex; }
    void setMapIndex(int index);

    int markerIndex() const { return m_markerIndex; }
    void setMarkerIndex(int index) { m_markerIndex = index; emit markerIndexChanged(); }

    Q_INVOKABLE void setMarkerPosition(int markerIndex, qreal x, qreal y);
    Q_INVOKABLE void setMarkerProperties(QString name, QString description, QString icon, QString color);
    Q_INVOKABLE void addMarker(qreal x, qreal y);
    Q_INVOKABLE void deleteMarker(int markerIndex);

signals:
    void categoriesChanged();
    void mapIndexChanged();
    void markerIndexChanged();

private:
    FileManager *fileManager;
    QQmlApplicationEngine *qmlEngine;

    QList<MapCategory*> m_categories;
    MapCategory* m_currentCategory = nullptr;
    MapListModel *mapListModel = nullptr;
    MapMarkerModel *mapMarkerModel = nullptr;

    int m_mapIndex = -1;
    int m_markerIndex = -1;
};

#endif // MAPVIEWERTOOL_H
