#include "src/tools/shop/project/itemgroup.h"
#include "src/tools/shop/project/shopproject.h"
#include "tests/testhelper/abstracttest.h"
#include <QObject>
#include <QtTest>
#include <memory>

class TestShopProject : public AbstractTest
{
    Q_OBJECT
public:
    using AbstractTest::AbstractTest;

private:
    std::unique_ptr<ShopProject> project;
    QByteArray projectJson;

    std::unique_ptr<ItemGroup> itemGroup;
    QByteArray itemGroupJson;

private slots:
    void initTestCase();
    void canSerializeProject();
    void verifyProjectContent();
    void canCloneProject();
    void canSerializeItemGroup();
    void verifyItemGroupContent();
};

void TestShopProject::initTestCase()
{
    QFile projectFile(QStringLiteral(":/resources/shopproject/project.shop"));
    QVERIFY2(projectFile.open(QIODevice::ReadOnly), "Could not open test project file");
    projectJson = projectFile.readAll();
    projectFile.close();
    QVERIFY2(!projectJson.isEmpty(), "Project file does not contain data");

    project = std::make_unique<ShopProject>(QJsonDocument::fromJson(projectJson).object(), nullptr);
    QVERIFY(project);

    QFile itemGroupFile(QStringLiteral(":/resources/shopproject/group.items"));
    QVERIFY2(itemGroupFile.open(QIODevice::ReadOnly), "Could not open test item group file");
    itemGroupJson = itemGroupFile.readAll();
    itemGroupFile.close();
    QVERIFY2(!itemGroupJson.isEmpty(), "Item group file does not contain data");

    itemGroup = std::make_unique<ItemGroup>(QStringLiteral("TestGroup"),
                                            QJsonDocument::fromJson(itemGroupJson).object(), nullptr);
    QVERIFY(itemGroup);
}

void TestShopProject::canSerializeProject()
{
    auto newJson = project->toJson();
    QCOMPARE(newJson, QJsonDocument::fromJson(projectJson).object());
}

void TestShopProject::verifyProjectContent()
{
    // Check meta data
    QVERIFY2(project, "Project is null after constructing from json");
    QCOMPARE(project->name(), QStringLiteral("TestProject"));
    QCOMPARE(project->version(), 3);
    QCOMPARE(project->categories().length(), 2);
    QVERIFY2(project->currentCategory(), "Current category is null");
    QVERIFY2(project->currentCategory()->currentShop(), "Current shop is null");
    QVERIFY(!project->wasRenamed());
    QVERIFY(project->isSaved());

    // check categories
    int shopCount = 1;
    for (int i = 0; i < project->categories().length(); i++)
    {
        const auto *category = project->categories().at(i);
        QVERIFY(category);
        QCOMPARE(category->name(), QStringLiteral("Category %1").arg(i + 1));

        // check shops
        QVERIFY2(!category->shops().isEmpty(), "category contains no shops");
        foreach (const auto *shop, category->shops())
        {
            QVERIFY(shop);
            QCOMPARE(shop->name(), QStringLiteral("Shop %1").arg(shopCount));
            QCOMPARE(shop->owner(), QStringLiteral("Owner %1").arg(shopCount));
            QCOMPARE(shop->description(), QStringLiteral("Description %1").arg(shopCount));

            // check items
            int itemCount = 1;
            foreach (const auto *item, shop->items())
            {
                QVERIFY(item);
                QCOMPARE(item->name(), QStringLiteral("Item %1").arg(itemCount));
                QCOMPARE(item->category(), QStringLiteral("Item Category %1").arg(itemCount));
                QCOMPARE(item->description(), QStringLiteral("Item Description %1").arg(itemCount));
                QCOMPARE(item->price(), QStringLiteral("%1 Units").arg(itemCount));
                itemCount++;
            }

            shopCount++;
        }
    }
}

void TestShopProject::canCloneProject()
{
    const ShopProject clone(*project, this);
    QCOMPARE(clone.name(), project->name());
    QCOMPARE(clone.version(), project->version());
    QCOMPARE(clone.categories().count(), project->categories().count());
    QCOMPARE(clone.currentCategory()->name(), project->currentCategory()->name());
    QVERIFY(clone.currentCategory() != project->currentCategory());
    QCOMPARE(clone.currentCategory()->shops().count(), project->currentCategory()->shops().count());
}

void TestShopProject::canSerializeItemGroup()
{
    auto newJson = itemGroup->toJson();
    QCOMPARE(newJson, QJsonDocument::fromJson(itemGroupJson).object());
}

void TestShopProject::verifyItemGroupContent()
{
    QVERIFY(itemGroup);
    QCOMPARE(itemGroup->name(), QStringLiteral("TestGroup"));
    QCOMPARE(itemGroup->items().count(), 6);

    QStringList categories;

    int count = 1;
    foreach (const auto *item, itemGroup->items())
    {
        QCOMPARE(item->name(), QStringLiteral("Item %1").arg(count));
        QCOMPARE(item->description(), QStringLiteral("Description %1").arg(count));
        QCOMPARE(item->price(), QStringLiteral("%1 Units").arg(count));

        if (!categories.contains(item->category()))
        {
            categories << item->category();
        }

        count++;
    }

    QCOMPARE(categories.count(), 2);
}

QTEST_APPLESS_MAIN(TestShopProject)
#include "testshopproject.moc"
