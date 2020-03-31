#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

#include "mapmarker.h"

class Map : public QObject
{
    Q_OBJECT

public:
    explicit Map(QString name, QString path, QObject *parent = nullptr);
    ~Map();

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    QString path() const { return m_path; }
    void setPath(QString path) { m_path = path; emit pathChanged(); }

    Q_PROPERTY(QString imageData READ data NOTIFY dataChanged)
    QString data() const { return m_data; }
    void setData(QString data) { m_data = data; emit dataChanged(); }

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
    void nameChanged();
    void pathChanged();
    void markersChanged();
    void dataChanged();

private:
    QString m_name, m_path, m_data;
    MapMarkerModel* m_markers;
    bool m_wasLoaded = false;
};

// Model for QML
class MapListModel : public QAbstractListModel {
    Q_OBJECT
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(QList<Map*> elements);
    void clear();

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
public:
    MapCategory(QString name, QList<Map*> maps, QObject *parent = nullptr);
    virtual ~MapCategory();

    QString name() const { return m_name; }

    QList<Map *> maps() const { return m_maps; }
    void setMaps(QList<Map*> maps) { m_maps = maps; }
    void addMap(Map* map);
    void loadMaps();

signals:
    void loadedMaps(QString category);

private:
    QString m_name;
    QList<Map*> m_maps;
    int m_getFileListRequestId = -1;
    bool m_wasLoaded = false;
};

#endif // MAP_H
