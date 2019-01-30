#ifndef ITEMSHOP_H
#define ITEMSHOP_H

#include <QObject>
#include <QAbstractListModel>

class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString price READ price WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)

public:
    Item(QString name, QString price, QString description, QString category) : m_name(name), m_price(price), m_description(description), m_category(category) {}
    Item(Item *other) { if (other) { m_name = other->m_name; m_price = other->m_price; m_description = other->m_description; m_category = other->m_category; } }

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    QString price() const { return m_price; }
    void setPrice(QString price) { m_price = price; emit priceChanged(); }

    QString description() const { return m_description; }
    void setDescription(QString description) { m_description = description; emit descriptionChanged(); }

    QString category() const { return m_category; }
    void setCategory(QString category) { m_category = category; emit categoryChanged(); }

signals:
    void nameChanged();
    void priceChanged();
    void descriptionChanged();
    void categoryChanged();

private:
    QString m_name, m_price, m_description, m_category;
};

class ItemGroup : public QObject
{
    Q_OBJECT
public:
    ItemGroup(QString name, QList<Item*> items) : m_name(name), m_items(items) {}
    ItemGroup(ItemGroup *other);
    ~ItemGroup() { for (auto i : m_items) if (i) i->deleteLater(); }

    QString name() const { return m_name; }
    QList<Item*> items() const { return m_items; }
    void setItems(QList<Item*> items) { m_items = items; }

private:
    QString m_name;
    QList<Item*> m_items;
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
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QList<Item*> items READ items WRITE setItems NOTIFY itemsChanged)

public:
    explicit ItemShop(QString name, QString owner, QString description, QList<Item*> items = {}, QObject *parent = nullptr);
    explicit ItemShop(ItemShop *other);
    ~ItemShop();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    QString owner() const { return m_owner; }
    void setOwner(QString owner) { m_owner = owner; emit ownerChanged(); }

    QString description() const { return m_description; }
    void setDescription(QString description) { m_description = description; emit descriptionChanged(); }

    QList<Item*> items() const { return m_items; }
    void setItems(QList<Item*> items) { m_items = items; emit itemsChanged(); }

signals:
    void nameChanged();
    void ownerChanged();
    void descriptionChanged();
    void itemsChanged();

private:
    QString m_name, m_owner, m_description;
    QList<Item*> m_items;

};

#endif // ITEMSHOP_H
