#include "shopproject.h"
#include "logging.h"
#include "utils/utils.h"

#include <QJsonArray>

ShopProject::ShopProject(const QString &name, const QList<ShopCategory*> &categories, QObject *parent)
    : QObject(parent), m_name(name), m_categories(categories)
{
    qCDebug(gmShopsShopProject()) << "Initializing new ShopProject:" << name << "...";

    if (!categories.isEmpty()) m_currentCategory = categories[0];
}

ShopProject::ShopProject(ShopProject *other)
{
    if (!other) return;

    m_name = other->m_name;

    for (auto *category : other->m_categories)
    {
        auto *copy = new ShopCategory(category);
        m_categories.append(copy);
    }

    if (!m_categories.isEmpty()) m_currentCategory = m_categories[0];
}

ShopProject::ShopProject(const QJsonObject &json)
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


        auto *category = new ShopCategory(co.value("name").toString(), shops);
        category->setCurrentShop(0);

        m_categories.append(category);
    }

    m_name = json["name"].toString();
    setCurrentCategory(0);
}

ShopProject::~ShopProject()
{
    for (auto *category : m_categories)
    {
        category->deleteLater();
    }
}

auto ShopProject::toJson() -> QJsonObject
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

ShopCategory::ShopCategory(const QString &name, const QList<ItemShop*> &shops)
    : a_name(name), m_shops(shops)
{
    qCDebug(gmShopsShopProject()) << "Initializing new ShopCategory:" << name << "...";
}

ShopCategory::ShopCategory(ShopCategory *other)
{
    name(other->name());

    for (auto *shop : other->shops())
    {
        if (shop)
        {
            auto *copy = new ItemShop(shop);
            m_shops.append(copy);
        }
    }

    if (!m_shops.isEmpty()) m_currentShop = m_shops[0];
}

ShopCategory::~ShopCategory()
{
    for (auto *shop : m_shops)
    {
        shop->deleteLater();
    }
}

auto ShopCategory::toJson() -> QJsonObject
{
    QJsonObject root;

    root.insert("name", name());

    QJsonArray shopArray;

    for (auto *shop : shops())
    {
        if (shop)
        {
            shopArray.append(shop->toJson());
        }
    }
    root.insert("shops", shopArray);

    return root;
}

void ShopCategory::setShops(const QList<ItemShop *> &shops)
{
    m_shops = shops;
    if (!m_shops.isEmpty()) m_currentShop = shops[0];
}

void ShopCategory::setCurrentShop(int index)
{
    if (Utils::isInBounds(m_shops, index))
    {
        m_currentShop = m_shops[index];
        return;
    }

    m_currentShop = nullptr;
}
