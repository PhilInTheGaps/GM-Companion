#ifndef SHOPPROJECT_H
#define SHOPPROJECT_H

#include <QObject>
#include <QList>
#include <QJsonObject>

#include "itemshop.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class ShopCategory : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)

public:
    ShopCategory(const QString &name, const QList<ItemShop *> &shops);
    ShopCategory(ShopCategory *other);
    ~ShopCategory();

    QJsonObject toJson();

    QList<ItemShop*> shops() const { return m_shops; }
    void setShops(const QList<ItemShop*> &shops);

    ItemShop *currentShop() const { return m_currentShop; }
    void setCurrentShop(int index);

private:
    QList<ItemShop*> m_shops;
    ItemShop *m_currentShop = nullptr;
};

class ShopProject : public QObject
{
    Q_OBJECT
public:
    explicit ShopProject(const QString &name, const QList<ShopCategory*> &categories, QObject *parent = nullptr);
    explicit ShopProject(ShopProject *other);
    explicit ShopProject(const QJsonObject &json);
    ~ShopProject();

    QJsonObject toJson();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    QList<ShopCategory*> categories() const { return m_categories; }
    void setCategories(QList<ShopCategory*> categories) { m_categories = categories; if (m_categories.size() > 0) m_currentCategory = m_categories[0]; }

    ShopCategory *currentCategory() const { return m_currentCategory; }
    void setCurrentCategory(int index) { if (index > -1 && m_categories.size() > index) m_currentCategory = m_categories[index]; else m_currentCategory = nullptr; }

private:
    QString m_name;
    QList<ShopCategory*> m_categories;
    ShopCategory *m_currentCategory = nullptr;
};

#endif // SHOPPROJECT_H
