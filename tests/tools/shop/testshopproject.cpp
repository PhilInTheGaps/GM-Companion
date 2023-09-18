#include "src/tools/shop/project/itemgroup.h"
#include "src/tools/shop/project/shopproject.h"
#include "tests/testhelper/abstracttest.h"
#include <gtest/gtest.h>
#include <memory>

class ShopProjectTest : public AbstractTest
{
public:
    ShopProjectTest()
    {
        QFile projectFile(QStringLiteral(":/resources/shopproject/project.shop"));
        EXPECT_TRUE(projectFile.open(QIODevice::ReadOnly)) << "Could not open test project file";
        projectJson = projectFile.readAll();
        projectFile.close();
        EXPECT_FALSE(projectJson.isEmpty()) << "Project file does not contain data";

        project = std::make_unique<ShopProject>(QJsonDocument::fromJson(projectJson).object(), nullptr);
        EXPECT_TRUE(project);

        QFile itemGroupFile(QStringLiteral(":/resources/shopproject/group.items"));
        EXPECT_TRUE(itemGroupFile.open(QIODevice::ReadOnly)) << "Could not open test item group file";
        itemGroupJson = itemGroupFile.readAll();
        itemGroupFile.close();
        EXPECT_FALSE(itemGroupJson.isEmpty()) << "Item group file does not contain data";

        itemGroup = std::make_unique<ItemGroup>(QStringLiteral("TestGroup"),
                                                QJsonDocument::fromJson(itemGroupJson).object(), nullptr);
        EXPECT_TRUE(itemGroup);
    }

protected:
    std::unique_ptr<ShopProject> project;
    QByteArray projectJson;

    std::unique_ptr<ItemGroup> itemGroup;
    QByteArray itemGroupJson;
};

TEST_F(ShopProjectTest, CanSerializeProject)
{
    auto newJson = project->toJson();
    EXPECT_EQ(newJson, QJsonDocument::fromJson(projectJson).object());
}

TEST_F(ShopProjectTest, VerifyProjectContent)
{
    // Check meta data
    ASSERT_TRUE(project) << "Project is null after constructing from json";
    EXPECT_EQ(project->name(), QStringLiteral("TestProject"));
    EXPECT_EQ(project->version(), 3);
    EXPECT_EQ(project->categories().length(), 2);
    EXPECT_TRUE(project->currentCategory()) << "Current category is null";
    EXPECT_TRUE(project->currentCategory()->currentShop()) << "Current shop is null";
    EXPECT_FALSE(project->wasRenamed());
    EXPECT_TRUE(project->isSaved());

    // check categories
    int shopCount = 1;
    for (int i = 0; i < project->categories().length(); i++)
    {
        const auto *category = project->categories().at(i);
        EXPECT_TRUE(category);
        EXPECT_EQ(category->name(), QStringLiteral("Category %1").arg(i + 1));

        // check shops
        EXPECT_FALSE(category->shops().isEmpty()) << "category contains no shops";
        foreach (const auto *shop, category->shops())
        {
            EXPECT_TRUE(shop);
            EXPECT_EQ(shop->name(), QStringLiteral("Shop %1").arg(shopCount));
            EXPECT_EQ(shop->owner(), QStringLiteral("Owner %1").arg(shopCount));
            EXPECT_EQ(shop->description(), QStringLiteral("Description %1").arg(shopCount));

            // check items
            int itemCount = 1;
            foreach (const auto *item, shop->items())
            {
                EXPECT_TRUE(item);
                EXPECT_EQ(item->name(), QStringLiteral("Item %1").arg(itemCount));
                EXPECT_EQ(item->category(), QStringLiteral("Item Category %1").arg(itemCount));
                EXPECT_EQ(item->description(), QStringLiteral("Item Description %1").arg(itemCount));
                EXPECT_EQ(item->price(), QStringLiteral("%1 Units").arg(itemCount));
                itemCount++;
            }

            shopCount++;
        }
    }
}

TEST_F(ShopProjectTest, CanCloneProject)
{
    const ShopProject clone(*project, nullptr);
    EXPECT_EQ(clone.name(), project->name());
    EXPECT_EQ(clone.version(), project->version());
    EXPECT_EQ(clone.categories().count(), project->categories().count());
    EXPECT_EQ(clone.currentCategory()->name(), project->currentCategory()->name());
    EXPECT_NE(clone.currentCategory(), project->currentCategory());
    EXPECT_EQ(clone.currentCategory()->shops().count(), project->currentCategory()->shops().count());
}

TEST_F(ShopProjectTest, CanSerializeItemGroup)
{
    auto newJson = itemGroup->toJson();
    EXPECT_EQ(newJson, QJsonDocument::fromJson(itemGroupJson).object());
}

TEST_F(ShopProjectTest, VerifyItemGroupContent)
{
    ASSERT_TRUE(itemGroup);
    EXPECT_EQ(itemGroup->name(), QStringLiteral("TestGroup"));
    EXPECT_EQ(itemGroup->items().count(), 6);

    QStringList categories;

    int count = 1;
    foreach (const auto *item, itemGroup->items())
    {
        EXPECT_EQ(item->name(), QStringLiteral("Item %1").arg(count));
        EXPECT_EQ(item->description(), QStringLiteral("Description %1").arg(count));
        EXPECT_EQ(item->price(), QStringLiteral("%1 Units").arg(count));

        if (!categories.contains(item->category()))
        {
            categories << item->category();
        }

        count++;
    }

    EXPECT_EQ(categories.count(), 2);
}
