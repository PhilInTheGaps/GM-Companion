#include "shopproject.h"
#include <QJsonArray>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmShopsShopProject, "gm.shops.project")

ShopProject::ShopProject(const QString &name, const QList<ShopCategory *> &categories, QObject *parent)
    : BaseProjectItem(name, parent), a_categories(categories)
{
    qCDebug(gmShopsShopProject()) << "Initializing new ShopProject:" << name << "...";

    connectSignals();
    connectCategories();

    if (!a_categories.isEmpty()) currentCategory(a_categories.constFirst());
}

ShopProject::ShopProject(const ShopProject &other, QObject *parent)
    : BaseProjectItem(other.name(), parent), a_version(other.version())
{
    connectSignals();

    a_categories.reserve(other.categories().size());
    foreach (const auto *category, other.categories())
    {
        auto *copy = new ShopCategory(*category, this);
        connectCategory(copy);
        a_categories.append(copy);
    }

    if (!a_categories.isEmpty()) currentCategory(a_categories.constFirst());
}

ShopProject::ShopProject(const QJsonObject &json, QObject *parent)
    : BaseProjectItem(json["name"_L1].toString(), parent), a_version(json["version"_L1].toInt())
{
    connectSignals();

    auto categoryArray = json["categories"_L1].toArray();

    foreach (const auto &categoryJson, categoryArray)
    {
        const auto categoryObject = categoryJson.toObject();

        QList<ItemShop *> shops;
        auto shopArray = categoryObject["shops"_L1].toArray();
        shops.reserve(shopArray.size());

        foreach (const auto &shop, shopArray)
        {
            shops.append(new ItemShop(shop.toObject(), this));
        }

        auto *category = new ShopCategory(categoryObject["name"_L1].toString(), shops, this);
        connectCategory(category);
        a_categories.append(category);
    }

    if (!a_categories.isEmpty()) currentCategory(a_categories.constFirst());
}

auto ShopProject::toJson() const -> QJsonObject
{
    QJsonObject root({{"name", name()}, {"version", 3}});
    QJsonArray categoryArray;

    foreach (const auto *category, categories())
    {
        if (category)
        {
            categoryArray.append(category->toJson());
        }
    }

    root.insert(u"categories"_s, categoryArray);
    return root;
}

void ShopProject::addCategory(ShopCategory *category)
{
    category->setParent(this);
    a_categories.append(category);

    if (!currentCategory()) currentCategory(category);

    emit categoriesChanged(a_categories);
}

auto ShopProject::deleteCategory(ShopCategory *category) -> bool
{
    if (!a_categories.removeOne(category)) return false;

    emit categoriesChanged(a_categories);

    category->deleteLater();
    return true;
}

void ShopProject::onCurrentCategoryChanged(ShopCategory *category)
{
    if (m_categoryConnection)
    {
        disconnect(m_categoryConnection);
    }

    if (!category) return;

    m_categoryConnection = connect(category, &ShopCategory::currentShopChanged, this, &ShopProject::currentShopChanged);

    emit currentShopChanged(category->currentShop());
}

void ShopProject::onWasEdited()
{
    isSaved(false);
}

void ShopProject::connectSignals() const
{
    connect(this, &ShopProject::currentCategoryChanged, this, &ShopProject::onCurrentCategoryChanged);
    connect(this, &ShopProject::wasEdited, this, &ShopProject::onWasEdited);
    connect(this, &ShopProject::categoriesChanged, this, &ShopProject::wasEdited);
}

void ShopProject::connectCategories()
{
    foreach (auto *category, categories())
    {
        connectCategory(category);
    }
}

void ShopProject::connectCategory(ShopCategory *category) const
{
    connect(category, &ShopCategory::wasEdited, this, &ShopProject::wasEdited);
}
