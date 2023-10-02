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

    [[nodiscard]] auto rowCount(const QModelIndex & /*parent*/) const -> int override
    {
        return m_items.size();
    }
    [[nodiscard]] auto size() const -> int
    {
        return m_items.size();
    }
    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;

    void setElements(const QList<MapMarker *> &elements);
    void addElement(MapMarker *marker)
    {
        m_items.append(marker);
    }
    void clear();
    [[nodiscard]] auto isEmpty() const -> bool
    {
        return m_items.isEmpty();
    }
    void removeAt(int index);

    auto marker(int index) -> MapMarker *
    {
        return qobject_cast<MapMarker *>(m_items.at(index));
    }
    [[nodiscard]] auto elements() const -> QList<MapMarker *>;

public slots:
    void insert(QObject *item);
    void remove(QObject *item);

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

private:
    QList<QObject *> m_items = {};
};
