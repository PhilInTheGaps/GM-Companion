#include "src/addons/addonmanager.h"
#include <QtTest>

#include "tests/testhelper/abstracttest.h"

class TestAddonManager : public AbstractTest
{
    Q_OBJECT

private slots:
    void canInstallAddons();
    void canLoadAddons();
    void canEnableAndDisableAddons();
    void canUninstallAddons();
    void canUpdateAddons();
    void cleanupTestCase();
};

void TestAddonManager::canInstallAddons()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    if (manager->repositoryManager().isLoading())
    {
        QSignalSpy spy(&manager->repositoryManager(), &AddonRepositoryManager::isLoadingChanged);
        QVERIFY(spy.wait());
    }

    // can not install null addon
    QVERIFY(manager->installAsync(nullptr).isCanceled());

    auto *addon = manager->addons()[0];
    QVERIFY(!addon->isInstalled());

    testFuture(manager->installAsync(addon), QStringLiteral("AddonManager::installAsync"),
               [addon]() { QVERIFY(addon->isInstalled()); });
}

void TestAddonManager::canLoadAddons()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    QVERIFY(!manager->addons().isEmpty());
}

void TestAddonManager::canEnableAndDisableAddons()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    auto addons = manager->addons();

    expectWarning();
    QVERIFY(!manager->setAddonEnabled(nullptr, true));

    for (int i = 0; i < 2; i++)
    {
        QVERIFY(addons[i]);
        QVERIFY(addons[i]->isInstalled());
        QVERIFY(manager->setAddonEnabled(addons[i], true));
        QVERIFY(addons[i]->enabled());

        QVERIFY(manager->setAddonEnabled(addons[i], false));
        QVERIFY(!addons[i]->enabled());
    }
}

void TestAddonManager::canUninstallAddons()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    // can not uninstall null addon
    QVERIFY(!manager->uninstall(nullptr));

    // can not uninstall a local addon
    const auto addons = manager->addons();
    for (auto *addon : addons)
    {
        if (addon->isLocal())
        {
            QVERIFY(!manager->uninstall(addon));
            break;
        }
    }

    // can uninstall remote addon
    QVERIFY(manager->uninstall(manager->addons()[0]));
}

void TestAddonManager::canUpdateAddons()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    // can not update null addon
    QVERIFY(manager->updateAsync(nullptr).isCanceled());

    // can not update addon that is not installed
    auto *addon = manager->addons()[0];
    QVERIFY(!addon->isInstalled());
    QVERIFY(manager->updateAsync(addon).isCanceled());

    // can update a valid, installed addon
    testFuture(manager->installAsync(addon), QStringLiteral("AddonManager::installAsync"), [this, manager, addon]() {
        QVERIFY(addon->isInstalled());
        addon->version(QStringLiteral("0"));
        testFuture(manager->updateAsync(addon), QStringLiteral("AddonManager::updateAsync"),
                   [addon]() { QCOMPARE(addon->version(), addon->newVersion()); });
    });
}

void TestAddonManager::cleanupTestCase()
{
    auto manager = AddonManager::instance();
    QVERIFY(manager);

    auto *addon = manager->addons()[0];

    if (addon->isInstalled())
    {
        manager->uninstall(addon);
    }

    manager->addons()[1]->enabled(true);
}

QTEST_GUILESS_MAIN(TestAddonManager)
#include "testaddonmanager.moc"
