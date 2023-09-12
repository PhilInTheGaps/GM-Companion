#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class MapMarker : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

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

    [[nodiscard]] auto toJson() const -> QJsonObject;

    [[nodiscard]] auto x() const -> qreal
    {
        return m_x;
    }
    [[nodiscard]] auto y() const -> qreal
    {
        return m_y;
    }
    void setPosition(qreal x, qreal y)
    {
        m_x = x;
        emit xChanged();

        m_y = y;
        emit yChanged();
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
    QML_ELEMENT

public:
    using QAbstractListModel::QAbstractListModel;

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
        return static_cast<MapMarker *>(m_items.at(index));
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
