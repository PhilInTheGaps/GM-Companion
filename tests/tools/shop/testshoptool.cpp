#include "settings/settingsmanager.h"
#include "src/tools/shop/shoptool.h"
#include "tests/testhelper/abstracttest.h"
#include "utils/fileutils.h"

class TestShopTool : public AbstractTest
{
    Q_OBJECT
public:
    using AbstractTest::AbstractTest;

private slots:
    void initTestCase();
    void verifyLoadedProjects();
    void verifyBehaviorForInvalidParameters();
    void cleanupTestCase();

private:
    QString origCloudMode;
    QString backupPath;
    QString userPath;

    ShopTool *tool = nullptr;
};

void TestShopTool::initTestCase()
{
    origCloudMode = SettingsManager::instance()->get(QStringLiteral("cloudMode"), QStringLiteral("local"));
    SettingsManager::instance()->set(QStringLiteral("cloudMode"), QStringLiteral("local"));

    userPath = SettingsManager::getPath(QStringLiteral("shops"));
    backupPath = backupUserFolder(userPath);

    copyResourceToFile(QStringLiteral(":/resources/shopproject/project.shop"),
                       FileUtils::fileInDir(QStringLiteral("project.shop"), userPath));

    tool = new ShopTool(nullptr, this);
    QVERIFY(tool);

    QVERIFY(!tool->isDataLoaded());
    tool->loadData();

    QSignalSpy spy(tool, &ShopTool::isLoadingChanged);
    spy.wait();

    QVERIFY(!tool->isLoading());
    QVERIFY(tool->isDataLoaded());
    QVERIFY(tool->itemModel());
}

void TestShopTool::verifyLoadedProjects()
{
    QVERIFY(tool);

    auto *project = tool->projects().constFirst();
    tool->currentProject(project);

    auto *category = project->categories().constFirst();
    project->currentCategory(category);

    auto *shop = category->shops().constFirst();
    category->currentShop(shop);

    QCOMPARE(tool->projects().count(), 1);
    QCOMPARE(project->categories().count(), 2);
    QCOMPARE(category->shops().count(), 2);

    QCOMPARE(shop->name(), QStringLiteral("Shop 1"));
    QCOMPARE(shop->owner(), QStringLiteral("Owner 1"));
    QCOMPARE(shop->description(), QStringLiteral("Description 1"));

    project->currentCategory(project->categories().at(1));
    shop = project->currentCategory()->currentShop();
    QCOMPARE(shop->name(), QStringLiteral("Shop 3"));
    QCOMPARE(shop->owner(), QStringLiteral("Owner 3"));
    QCOMPARE(shop->description(), QStringLiteral("Description 3"));

    shop = project->currentCategory()->shops().at(1);
    project->currentCategory()->currentShop(shop);
    QCOMPARE(shop->name(), QStringLiteral("Shop 4"));
    QCOMPARE(shop->owner(), QStringLiteral("Owner 4"));
    QCOMPARE(shop->description(), QStringLiteral("Description 4"));
}

void TestShopTool::verifyBehaviorForInvalidParameters()
{
    auto *shop = tool->currentProject()->currentCategory()->currentShop();
    QVERIFY(!shop->name().isEmpty());

    tool->currentProject(nullptr);
    QVERIFY(!tool->currentProject());

    tool->currentProject(tool->projects().constFirst());
    tool->currentProject()->currentCategory(nullptr);
    QVERIFY(!tool->currentProject()->currentCategory());

    tool->currentProject()->currentCategory(tool->currentProject()->categories().constFirst());
    tool->currentProject()->currentCategory()->currentShop(nullptr);
    QVERIFY(!tool->currentProject()->currentCategory()->currentShop());
}

void TestShopTool::cleanupTestCase()
{
    SettingsManager::instance()->set(QStringLiteral("cloudMode"), origCloudMode);

    restoreUserFolder(backupPath, userPath);
}

QTEST_GUILESS_MAIN(TestShopTool)
#include "testshoptool.moc"
