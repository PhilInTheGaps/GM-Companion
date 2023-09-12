#pragma once

#include "common/abstracttool.h"
#include "map.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QQmlEngine>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class MapTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    AUTO_PROPERTY_VAL2(int, markerIndex, -1)

public:
    MapTool() = delete;
    explicit MapTool(QObject *parent = nullptr);

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> MapTool *;

    Q_PROPERTY(MapListModel *listModel READ listModel CONSTANT)
    [[nodiscard]] auto listModel() -> MapListModel *
    {
        return &m_mapListModel;
    }

    Q_PROPERTY(MapMarkerModel *markerModel READ markerModel CONSTANT)
    [[nodiscard]] auto markerModel() -> MapMarkerModel *
    {
        return &m_mapMarkerModel;
    }

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    [[nodiscard]] QStringList categories() const;

    Q_PROPERTY(int currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    [[nodiscard]] int currentCategory() const
    {
        return m_currentCategoryIndex;
    }

    Q_PROPERTY(Map *currentMap READ currentMap NOTIFY mapIndexChanged)
    [[nodiscard]] Map *currentMap() const
    {
        return m_currentMap;
    }

    Q_PROPERTY(int mapIndex READ mapIndex WRITE setMapIndex NOTIFY mapIndexChanged)
    [[nodiscard]] int mapIndex() const
    {
        return m_mapIndex;
    }
    void setMapIndex(int index);

    Q_PROPERTY(MapMarker *currentMarker READ currentMarker NOTIFY markerIndexChanged)
    MapMarker *currentMarker() const;

public slots:
    void setCurrentCategory(int index);
    void setMarkerPosition(int markerIndex, qreal x, qreal y);
    void setMarkerProperties(const QString &name, const QString &description, const QString &icon,
                             const QString &color);
    void addMarker();
    void deleteMarker(int markerIndex);
    void loadData() override;

signals:
    void currentCategoryChanged();
    void categoriesChanged();
    void mapIndexChanged();

private:
    QList<MapCategory *> m_categories;
    int m_currentCategoryIndex = -1;
    Map *m_currentMap = nullptr;
    MapCategory *m_currentCategory = nullptr;
    MapListModel m_mapListModel;
    MapMarkerModel m_mapMarkerModel;

    int m_mapIndex = -1;

    void findCategories();
    void receivedCategories(const QStringList &folders);

private slots:
    void onMapsLoaded(const QString &category);
};
