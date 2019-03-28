#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class Map : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)

public:
    explicit Map(QString name, QString relPath, QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    QString path() const { return m_path; }
    void setPath(QString path) { m_path = path; emit pathChanged(); }

    QString relativePath() const { return m_relativePath; }
    void setRelativePath(QString path) { m_relativePath = path; }

signals:
    void nameChanged();
    void pathChanged();

private:
    QString m_name, m_path, m_relativePath;
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
    QString cloudId() const { return m_cloudId; }
    void setCloudId(QString id) { m_cloudId = id; }

    QList<Map*> maps() const { return m_maps; }
    void setMaps(QList<Map*> maps) { m_maps = maps; }
    void addMap(Map* map);

    int requestId() const { return m_requestId; }
    void setRequestId(int id) { m_requestId = id; }

private:
    QString m_name, m_cloudId;
    QList<Map*> m_maps;
    int m_requestId = -1;
};

#endif // MAP_H
