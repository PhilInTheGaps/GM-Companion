#ifndef MAPMARKER_H
#define MAPMARKER_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class MapMarker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY propertiesChanged)
    Q_PROPERTY(QString description READ description NOTIFY propertiesChanged)
    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)
    Q_PROPERTY(QString color READ color NOTIFY propertiesChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY propertiesChanged)

public:
    explicit MapMarker(QString name, QString description, qreal x, qreal y, QString icon, QString color = "red", QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit propertiesChanged(); }

    QString description() { return m_description; }
    void setDescription(QString description) { m_description = description; emit propertiesChanged(); }

    QString color() const { return m_color; }
    void setColor(QString color) { m_color = color; emit propertiesChanged(); }

    QString icon() const { return m_icon; }
    void setIcon(QString icon) { m_icon = icon; emit propertiesChanged(); }

    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    void setPosition(qreal x, qreal y) { m_x = x; m_y = y; }

signals:
    void propertiesChanged();
    void xChanged();
    void yChanged();

private:
    QString m_name, m_description, m_color, m_icon;
    qreal m_x, m_y;

};

// Model for QML
class MapMarkerModel : public QAbstractListModel {
    Q_OBJECT
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    int size() const { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(QList<MapMarker*> elements);
    void addElement(MapMarker* marker) { m_items.append(marker); }
    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }
    void removeAt(int index);

    MapMarker* marker(int index) { return static_cast<MapMarker*>(m_items[index]); }
    QList<MapMarker*> elements() const { QList<MapMarker*> list; for (auto i : m_items) list.append(static_cast<MapMarker*>(i)); return list; }

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

#endif // MAPMARKER_H
