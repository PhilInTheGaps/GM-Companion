#pragma once

#include "mapmarker.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

class Map : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, path)
    AUTO_PROPERTY(QUrl, imageData)

public:
    explicit Map(QObject *parent = nullptr);
    explicit Map(const QString &name, const QString &path, QObject *parent);

    Q_PROPERTY(MapMarkerModel *markers READ markers NOTIFY markersChanged)

    Q_PROPERTY(bool hasMarkers READ hasMarkers NOTIFY markersChanged)
    [[nodiscard]] auto hasMarkers() const -> bool
    {
        return !m_markers.isEmpty();
    }

    auto markers() -> MapMarkerModel *
    {
        return &m_markers;
    }
    void addMarker(MapMarker *marker)
    {
        m_markers.addElement(marker);
    }
    void saveMarkers() const;

    void loadMarkers();
    void deleteMarker(int index);

signals:
    void markersChanged();

private:
    MapMarkerModel m_markers;
    bool m_wasLoaded = false;
};

// Model for QML
class MapListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QAbstractListModel::QAbstractListModel;

    [[nodiscard]] auto rowCount(const QModelIndex & /*parent*/) const -> int override
    {
        return m_items.size();
    }
    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;
    [[nodiscard]] auto size() const -> int
    {
        return m_items.size();
    }

    void setElements(QList<Map *> elements);
    void clear();

    [[nodiscard]] auto elements() const -> QList<Map *>
    {
        QList<Map *> list;
        list.reserve(m_items.size());
        foreach (auto i, m_items)
            list.append(qobject_cast<Map *>(i));
        return list;
    }

public slots:
    void insert(QObject *item);
    void remove(QObject *item);

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

private:
    QList<QObject *> m_items = {};
};

class MapCategory : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QList<Map *>, maps)
public:
    using QObject::QObject;
    explicit MapCategory(const QString &name, const QList<Map *> &maps, QObject *parent);

    void addMap(Map *map);
    void loadMaps();

signals:
    void loadedMaps(QString category);

private:
    int m_getFileListRequestId = -1;
    bool m_wasLoaded = false;
};
