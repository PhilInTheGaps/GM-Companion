#include "src/tools/audio/editor/addonelementmanager.h"
#include "tests/testhelper/abstracttest.h"
#include <QObject>
#include <QtTest>

class TestAudioAddons : public AbstractTest
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void canFindAddonsWithAudioContent();
};

void TestAudioAddons::initTestCase()
{
    enableTestAddons();
}

void TestAudioAddons::cleanupTestCase()
{
    disableTestAddons();
}

void TestAudioAddons::canFindAddonsWithAudioContent()
{
    AddonElementManager manager;
    QCOMPARE(manager.addons().length(), 0);

    manager.loadData();

    if (manager.isLoading())
    {
        QSignalSpy spy(&manager, &AddonElementManager::isLoadingChanged);
        QVERIFY(spy.wait());
    }

    QCOMPARE(manager.addons().length(), 2);

    manager.currentIndex(0);
    const auto projects = manager.projects();
    QCOMPARE(projects.length(), 1);
}

QTEST_GUILESS_MAIN(TestAudioAddons)
#include "testaudioaddons.moc"
