#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QStringList>
#include <QQmlApplicationEngine>
#include "common/abstracttool.h"
#include "map.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class MapTool : public AbstractTool
{
    Q_OBJECT
    AUTO_PROPERTY(int, markerIndex)

public:
    explicit MapTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~MapTool();

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    QStringList categories() const;

    Q_PROPERTY(int currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    int currentCategory() const { return m_currentCategoryIndex; }

    Q_PROPERTY(Map* currentMap READ currentMap NOTIFY mapIndexChanged)
    Map *currentMap() const { return m_currentMap; }

    Q_PROPERTY(int mapIndex READ mapIndex WRITE setMapIndex NOTIFY mapIndexChanged)
    int mapIndex() const { return m_mapIndex; }
    void setMapIndex(int index);

    Q_PROPERTY(MapMarker* currentMarker READ currentMarker NOTIFY markerIndexChanged)
    MapMarker *currentMarker() const;

public slots:
    void setCurrentCategory(int index);
    void setMarkerPosition(int markerIndex, qreal x, qreal y);
    void setMarkerProperties(const QString &name, const QString &description,
                             const QString &icon, const QString &color);
    void addMarker();
    void deleteMarker(int markerIndex);
    void loadData() override;

signals:
    void currentCategoryChanged();
    void categoriesChanged();
    void mapIndexChanged();

private:
    QQmlApplicationEngine *qmlEngine;

    QList<MapCategory*> m_categories;
    int m_currentCategoryIndex = -1;
    Map* m_currentMap = nullptr;
    MapCategory* m_currentCategory = nullptr;
    MapListModel *mapListModel = nullptr;
    MapMarkerModel *mapMarkerModel = nullptr;

    int m_mapIndex = -1;

    void findCategories();
    void receivedCategories(const QStringList &folders);

    static constexpr const char* DEFAULT_COLOR = "red";
    static constexpr const char* DEFAULT_ICON = "\uf3c5";

private slots:
    void onMapsLoaded(const QString &category);
};

#endif // MAPVIEWERTOOL_H
