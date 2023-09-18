#include "src/addons/addonmanager.h"
#include "tests/testhelper/abstracttest.h"

class AddonManagerTest : public AbstractTest
{
protected:
    void TearDown() override
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        auto *addon = manager->addons()[0];

        if (addon->isInstalled())
        {
            manager->uninstall(addon);
        }
    };

    void installAddons()
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        if (manager->repositoryManager().isLoading())
        {
            QSignalSpy spy(&manager->repositoryManager(), &AddonRepositoryManager::isLoadingChanged);
            EXPECT_TRUE(spy.wait());
        }

        // can not install null addon
        EXPECT_TRUE(manager->installAsync(nullptr).isCanceled());

        auto *addon = manager->addons()[0];
        EXPECT_FALSE(addon->isInstalled());

        testFuture(manager->installAsync(addon), "AddonManager::installAsync",
                   [addon]() { EXPECT_TRUE(addon->isInstalled()); });
    }

    void checkAddonsAreLoaded()
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        EXPECT_FALSE(manager->addons().isEmpty());
    }

    void enableAndDisableAddons()
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        auto addons = manager->addons();

        expectWarning();
        EXPECT_FALSE(manager->setAddonEnabled(nullptr, true));

        for (int i = 0; i < 2; i++)
        {
            EXPECT_TRUE(addons[i]);
            EXPECT_TRUE(addons[i]->isInstalled());
            EXPECT_TRUE(manager->setAddonEnabled(addons[i], true));
            EXPECT_TRUE(addons[i]->enabled());

            EXPECT_TRUE(manager->setAddonEnabled(addons[i], false));
            EXPECT_FALSE(addons[i]->enabled());
        }
    }

    void uninstallAddons()
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        // can not uninstall null addon
        EXPECT_FALSE(manager->uninstall(nullptr));

        // can not uninstall a local addon
        const auto addons = manager->addons();
        for (auto *addon : addons)
        {
            if (addon->isLocal())
            {
                EXPECT_FALSE(manager->uninstall(addon));
                break;
            }
        }

        // can uninstall remote addon
        EXPECT_TRUE(manager->uninstall(manager->addons()[0]));
    }

    void updateAddons()
    {
        auto manager = AddonManager::instance();
        ASSERT_TRUE(manager);

        // can not update null addon
        EXPECT_TRUE(manager->updateAsync(nullptr).isCanceled());

        // can not update addon that is not installed
        auto *addon = manager->addons()[0];
        EXPECT_FALSE(addon->isInstalled());
        EXPECT_TRUE(manager->updateAsync(addon).isCanceled());

        // can update a valid, installed addon
        testFuture(manager->installAsync(addon), "AddonManager::installAsync", [this, manager, addon]() {
            EXPECT_TRUE(addon->isInstalled());
            addon->version(QStringLiteral("0"));
            testFuture(manager->updateAsync(addon), "AddonManager::updateAsync",
                       [addon]() { EXPECT_EQ(addon->version(), addon->newVersion()); });
        });
    }
};

TEST_F(AddonManagerTest, CanManageAddons)
{
    installAddons();
    checkAddonsAreLoaded();
    enableAndDisableAddons();
    uninstallAddons();
    updateAddons();
}
