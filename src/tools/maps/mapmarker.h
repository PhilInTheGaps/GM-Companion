#ifndef MAPMARKER_H
#define MAPMARKER_H

#include <QAbstractListModel>
#include <QJsonObject>
#include <QList>
#include <QObject>

#include "thirdparty/propertyhelper/PropertyHelper.h"

class MapMarker : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QString, color)
    AUTO_PROPERTY(QString, icon)
    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)

public:
    explicit MapMarker(const QString &name, const QString &description, qreal x, qreal y, const QString &icon,
                       const QString &color, QObject *parent);

    explicit MapMarker(const QJsonObject &json, QObject *parent);

    QJsonObject toJson() const;

    qreal x() const
    {
        return m_x;
    }
    qreal y() const
    {
        return m_y;
    }
    void setPosition(qreal x, qreal y)
    {
        m_x = x;
        m_y = y;
    }

signals:
    void xChanged();
    void yChanged();

private:
    qreal m_x = 0, m_y = 0;
};

// Model for QML
class MapMarkerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapMarkerModel(QObject *parent) : QAbstractListModel(parent)
    {
    }

    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    int size() const
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    void setElements(const QList<MapMarker *> &elements);
    void addElement(MapMarker *marker)
    {
        m_items.append(marker);
    }
    void clear();
    bool isEmpty() const
    {
        return m_items.isEmpty();
    }
    void removeAt(int index);

    MapMarker *marker(int index)
    {
        return static_cast<MapMarker *>(m_items[index]);
    }
    QList<MapMarker *> elements() const;

public slots:
    void insert(QObject *item);
    void remove(QObject *item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QObject *> m_items = {};
};

#endif // MAPMARKER_H
