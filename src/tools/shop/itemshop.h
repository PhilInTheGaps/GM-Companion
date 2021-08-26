#ifndef ITEMSHOP_H
#define ITEMSHOP_H

#include <QObject>
#include <QAbstractListModel>
#include <QJsonObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Item : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, price)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QString, category)

public:
    Item(const QString &name, const QString &price, const QString &description, const QString &category)
        : a_name(name), a_price(price), a_description(description), a_category(category) {}
    Item(Item *other);
    Item(const QJsonObject &json);
    Item(const QString &category, const QJsonObject &json);

    QJsonObject toJson();
};

class ItemGroup : public QObject
{
    Q_OBJECT
    READONLY_PROPERTY(QString, name)
    AUTO_PROPERTY(QList<Item*>, items)

public:
    ItemGroup(const QString &name, const QList<Item*> &items) : a_name(name), a_items(items) {}
    ItemGroup(ItemGroup *other);
    ItemGroup(const QString &groupName, const QJsonObject &json);
    ~ItemGroup() { for (auto *i : items()) if (i) i->deleteLater(); }

    QJsonObject toJson();
};

class ItemModel : public QAbstractListModel {
    Q_OBJECT
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(QList<Item*> elements);
    void clear();

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

class ItemShop : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, owner)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QList<Item*>, items)

public:
    explicit ItemShop(const QString& name, const QString &owner,
                      const QString &description, const QList<Item *> &items = {},
                      QObject *parent = nullptr);
    explicit ItemShop(ItemShop *other);
    explicit ItemShop(const QJsonObject &json);
    ~ItemShop();

    QJsonObject toJson();;
};

#endif // ITEMSHOP_H
