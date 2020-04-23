#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

#include "map.h"

class MapTool : public QObject
{
    Q_OBJECT

public:
    explicit MapTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~MapTool();

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    QStringList categories();

    Q_PROPERTY(int currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    int currentCategory() const { return m_currentCategoryIndex; }

    Q_PROPERTY(Map* currentMap READ currentMap NOTIFY mapIndexChanged)
    Map *currentMap() const { return m_currentMap; }

    Q_PROPERTY(int mapIndex READ mapIndex WRITE setMapIndex NOTIFY mapIndexChanged)
    int mapIndex() const { return m_mapIndex; }
    void setMapIndex(int index);

    Q_PROPERTY(int markerIndex READ markerIndex WRITE setMarkerIndex NOTIFY markerIndexChanged)
    int markerIndex() const { return m_markerIndex; }
    void setMarkerIndex(int index) { m_markerIndex = index; emit markerIndexChanged(); }

    Q_PROPERTY(MapMarker* currentMarker READ currentMarker NOTIFY markerIndexChanged)
    MapMarker *currentMarker() const;

public slots:
    void setCurrentCategory(int index);
    void setMarkerPosition(int markerIndex, qreal x, qreal y);
    void setMarkerProperties(QString name, QString description, QString icon, QString color);
    void addMarker();
    void deleteMarker(int markerIndex);

signals:
    void currentCategoryChanged();
    void categoriesChanged();
    void mapIndexChanged();
    void markerIndexChanged();

private:
    QQmlApplicationEngine *qmlEngine;

    QList<MapCategory*> m_categories;
    int m_currentCategoryIndex = -1;
    Map* m_currentMap = nullptr;
    MapCategory* m_currentCategory = nullptr;
    MapListModel *mapListModel = nullptr;
    MapMarkerModel *mapMarkerModel = nullptr;

    int m_mapIndex = -1;
    int m_markerIndex = -1;
    int m_getCategoriesRequestId = -1;

    void findCategories();
    void receivedCategories(QStringList folders);

private slots:
    void onMapsLoaded(QString category);
    void onFileListReceived(int id, QStringList files);
};

#endif // MAPVIEWERTOOL_H
