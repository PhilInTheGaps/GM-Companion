#ifndef DSA5LISTITEM_H
#define DSA5LISTITEM_H

#include <QObject>
#include <QAbstractListModel>

class DSA5ListItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString group READ group NOTIFY groupChanged)
    Q_PROPERTY(QList<int> values READ values NOTIFY valuesChanged)

public:
    explicit DSA5ListItem(QString name, QString group, QList<int> values, QObject *parent = nullptr);

    QString name() const { return m_name; }
    QString group() const { return m_group; }
    QList<int> values() const { return m_values; }

signals:
    void nameChanged();
    void groupChanged();
    void valuesChanged();

private:
    QString m_name, m_group;
    QList<int> m_values;

};

// Model for QML
class DSA5ListModel : public QAbstractListModel {
    Q_OBJECT
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(QList<DSA5ListItem*> elements);
    void clear();

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

#endif // DSA5LISTITEM_H
