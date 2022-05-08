#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

#include "mapmarker.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Map : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, path)
    AUTO_PROPERTY(QString, imageData)

public:
    explicit Map(const QString &name, const QString &path, QObject *parent);

    Q_PROPERTY(MapMarkerModel* markers READ markers NOTIFY markersChanged)

    Q_PROPERTY(bool hasMarkers READ hasMarkers NOTIFY markersChanged)
    bool hasMarkers() const { return !m_markers->isEmpty(); }

    MapMarkerModel* markers() const { return m_markers; }
    void setMarkers(MapMarkerModel* markers) { m_markers = markers;  }
    void addMarker(MapMarker* marker) { m_markers->addElement(marker); }
    void saveMarkers();

    void loadMarkers();
    void deleteMarker(int index);

signals:
    void markersChanged();

private:
    MapMarkerModel* m_markers;
    bool m_wasLoaded = false;
};

// Model for QML
class MapListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapListModel(QObject *parent) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;
    int size() const { return m_items.size(); }

    void setElements(QList<Map*> elements);
    void clear();

    QList<Map*> elements() const { QList<Map*> list; for (auto i : m_items) list.append(static_cast<Map*>(i)); return list; }

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

class MapCategory : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QList<Map*>, maps)
public:
    MapCategory(const QString &name, const QList<Map *> &maps, QObject *parent);

    void addMap(Map* map);
    void loadMaps();

signals:
    void loadedMaps(QString category);

private:
    int m_getFileListRequestId = -1;
    bool m_wasLoaded = false;
};

#endif // MAP_H
