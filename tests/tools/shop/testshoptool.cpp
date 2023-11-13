#include "settings/settingsmanager.h"
#include "src/tools/shop/shoptool.h"
#include "tests/testhelper/staticabstracttest.h"
#include "utils/fileutils.h"
#include <gtest/gtest.h>

using namespace Common::Settings;

class ShopToolTest : public StaticAbstractTest
{
public:
    static void SetUpTestSuite()
    {
        origCloudMode = SettingsManager::instance()->get(QStringLiteral("cloudMode"), QStringLiteral("local"));
        SettingsManager::instance()->set(QStringLiteral("cloudMode"), QStringLiteral("local"));

        userPath = SettingsManager::getPath(QStringLiteral("shops"));
        backupPath = backupUserFolder(userPath, tempDir);

        copyResourceToFile(QStringLiteral(":/resources/shopproject/project.shop"),
                           FileUtils::fileInDir(QStringLiteral("project.shop"), userPath));

        tool = std::make_unique<ShopTool>(nullptr);
        EXPECT_TRUE(tool);

        EXPECT_FALSE(tool->isDataLoaded());
        tool->loadData();

        QSignalSpy spy(tool.get(), &ShopTool::isLoadingChanged);
        spy.wait();

        EXPECT_FALSE(tool->isLoading());
        EXPECT_TRUE(tool->isDataLoaded());
        EXPECT_TRUE(tool->itemModel());
    }

    static void TearDownTestSuite()
    {
        SettingsManager::instance()->set(QStringLiteral("cloudMode"), origCloudMode);

        restoreUserFolder(backupPath, userPath);
        tool = nullptr;
    }

protected:
    inline static QString origCloudMode;
    inline static QString backupPath;
    inline static QString userPath;
    inline static QTemporaryDir tempDir;

    inline static std::unique_ptr<ShopTool> tool = nullptr;
};

TEST_F(ShopToolTest, VerifyLoadedProjects)
{
    EXPECT_TRUE(tool);

    auto *project = tool->projects().constFirst();
    tool->currentProject(project);

    auto *category = project->categories().constFirst();
    project->currentCategory(category);

    auto *shop = category->shops().constFirst();
    category->currentShop(shop);

    EXPECT_EQ(tool->projects().count(), 1);
    EXPECT_EQ(project->categories().count(), 2);
    EXPECT_EQ(category->shops().count(), 2);

    EXPECT_EQ(shop->name(), QStringLiteral("Shop 1"));
    EXPECT_EQ(shop->owner(), QStringLiteral("Owner 1"));
    EXPECT_EQ(shop->description(), QStringLiteral("Description 1"));

    project->currentCategory(project->categories().at(1));
    shop = project->currentCategory()->currentShop();
    EXPECT_EQ(shop->name(), QStringLiteral("Shop 3"));
    EXPECT_EQ(shop->owner(), QStringLiteral("Owner 3"));
    EXPECT_EQ(shop->description(), QStringLiteral("Description 3"));

    shop = project->currentCategory()->shops().at(1);
    project->currentCategory()->currentShop(shop);
    EXPECT_EQ(shop->name(), QStringLiteral("Shop 4"));
    EXPECT_EQ(shop->owner(), QStringLiteral("Owner 4"));
    EXPECT_EQ(shop->description(), QStringLiteral("Description 4"));
}

TEST_F(ShopToolTest, VerifyBehaviorForInvalidParameters)
{
    auto *shop = tool->currentProject()->currentCategory()->currentShop();
    EXPECT_FALSE(shop->name().isEmpty());

    tool->currentProject(nullptr);
    EXPECT_FALSE(tool->currentProject());

    tool->currentProject(tool->projects().constFirst());
    tool->currentProject()->currentCategory(nullptr);
    EXPECT_FALSE(tool->currentProject()->currentCategory());

    tool->currentProject()->currentCategory(tool->currentProject()->categories().constFirst());
    tool->currentProject()->currentCategory()->currentShop(nullptr);
    EXPECT_FALSE(tool->currentProject()->currentCategory()->currentShop());
}
