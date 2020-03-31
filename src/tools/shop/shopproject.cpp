#include "shopproject.h"
#include "logging.h"

#include <QJsonArray>

ShopProject::ShopProject(QString name, QList<ShopCategory *>categories, QObject *parent)
    : QObject(parent), m_name(name), m_categories(categories)
{
    qCDebug(gmShopsShopProject()) << "Initializing new ShopProject:" << name << "...";

    if (categories.size() > 0) m_currentCategory = categories[0];
}

ShopProject::ShopProject(ShopProject *other)
{
    if (other)
    {
        m_name = other->m_name;

        for (auto c : other->m_categories)
        {
            auto copy = new ShopCategory(c);
            m_categories.append(copy);
        }

        if (m_categories.size() > 0) m_currentCategory = m_categories[0];
    }
}

ShopProject::ShopProject(QJsonObject json)
{
    auto categoryArray = json["categories"].toArray();

    for (auto c : categoryArray)
    {
        auto co = c.toObject();

        // Get Shops in Category
        QList<ItemShop *> shops;
        auto shopArray = co.value("shops").toArray();

        for (auto shop : shopArray)
        {
            shops.append(new ItemShop(shop.toObject()));
        }


        ShopCategory *category = new ShopCategory(co.value("name").toString(), shops);
        category->setCurrentShop(0);

        m_categories.append(category);
    }

    m_name = json["name"].toString();
    setCurrentCategory(0);
}

ShopProject::~ShopProject()
{
    for (auto c : m_categories)
    {
        c->deleteLater();
    }
}

QJsonObject ShopProject::toJson()
{
    QJsonObject root;

    root.insert("name",    name());
    root.insert("version", 3);

    QJsonArray categoryArray;

    for (auto category : categories())
    {
        if (category)
        {
            categoryArray.append(category->toJson());
        }
    }

    root.insert("categories", categoryArray);

    return root;
}

ShopCategory::ShopCategory(QString name, QList<ItemShop *>shops)
    : m_name(name), m_shops(shops)
{
    qCDebug(gmShopsShopProject()) << "Initializing new ShopCategory:" << name << "...";
}

ShopCategory::ShopCategory(ShopCategory *other)
{
    m_name = other->m_name;

    for (auto s : other->shops())
    {
        if (s)
        {
            auto copy = new ItemShop(s);
            m_shops.append(copy);
        }
    }

    if (m_shops.size() > 0) m_currentShop = m_shops[0];
}

ShopCategory::~ShopCategory()
{
    for (auto s : m_shops)
    {
        s->deleteLater();
    }
}

QJsonObject ShopCategory::toJson()
{
    QJsonObject root;

    root.insert("name", name());

    QJsonArray shopArray;

    for (auto shop : shops())
    {
        if (shop)
        {
            shopArray.append(shop->toJson());
        }
    }
    root.insert("shops", shopArray);

    return root;
}
