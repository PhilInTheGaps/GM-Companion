#include "shopproject.h"
#include <QDebug>

ShopProject::ShopProject(QString name, QList<ShopCategory *>categories, QObject *parent)
    : QObject(parent), m_name(name), m_categories(categories)
{
    qDebug() << "Initializing new ShopProject:" << name << "...";

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

ShopProject::~ShopProject()
{
    for (auto c : m_categories)
    {
        c->deleteLater();
    }
}

ShopCategory::ShopCategory(QString name, QList<ItemShop *>shops)
    : m_name(name), m_shops(shops)
{
    qDebug() << "Initializing new ShopCategory:" << name << "...";
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
