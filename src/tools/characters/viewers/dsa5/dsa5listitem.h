#ifndef DSA5LISTITEM_H
#define DSA5LISTITEM_H

#include <QObject>
#include <QAbstractListModel>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class DSA5ListItem : public QObject
{
    Q_OBJECT
    READONLY_PROPERTY(QString, name)
    READONLY_PROPERTY(QString, group)
    READONLY_PROPERTY(QList<int>, values)

public:
    explicit DSA5ListItem(const QString &name, const QString &group,
                          const QList<int> &values, QObject *parent);
};

// Model for QML
class DSA5ListModel : public QAbstractListModel {
    Q_OBJECT
public:
    DSA5ListModel(QObject *parent) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(const QList<DSA5ListItem *> &elements);
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
