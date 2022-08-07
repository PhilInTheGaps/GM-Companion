#include <QtTest>
#include "src/addons/addonmanager.h"
#include <limits>

#include "tests/testhelper/abstracttest.h"

class TestAddonManager : public AbstractTest
{
    Q_OBJECT

private:
    static constexpr int MAX_INT = std::numeric_limits<int>::max();

private slots:
    void canLoadAddons();
    void canEnableAndDisableAddons();
};


void TestAddonManager::canLoadAddons()
{
    auto *manager = AddonManager::instance();
    QVERIFY(manager);

    QVERIFY(!manager->addons().isEmpty());
}

void TestAddonManager::canEnableAndDisableAddons()
{
    auto *manager = AddonManager::instance();
    QVERIFY(manager);

    auto addons = manager->addons();

    QVERIFY(!manager->setAddonEnabled(-1, true));
    QVERIFY(!manager->setAddonEnabled(MAX_INT, true));

    QVERIFY(manager->setAddonEnabled(0, true));
    QVERIFY(addons[0]->enabled());

    QVERIFY(manager->setAddonEnabled(0, false));
    QVERIFY(!addons[0]->enabled());
}

QTEST_GUILESS_MAIN(TestAddonManager)
#include "testaddonmanager.moc"
