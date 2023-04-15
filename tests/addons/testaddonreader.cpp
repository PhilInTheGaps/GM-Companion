#include "src/addons/addon_reader/addonreader.h"
#include <QtTest>

#include "tests/testhelper/abstracttest.h"

class TestAddonReader : public AbstractTest
{
    Q_OBJECT

private slots:
    void initTestCase();
    void canFindAllFiles();
    void canReadFiles();
    void canCheckFiles();
    void canLoadFeatures();
    void cleanupTestCase();
};

void TestAddonReader::initTestCase()
{
    enableTestAddons();
}

void TestAddonReader::canFindAllFiles()
{
    const auto addons = testAddons();
    for (auto *addon : addons)
    {
        AddonReader reader(*addon);

        QVERIFY(!reader.findAllFiles(QStringLiteral("/"), {}).isEmpty());
        QCOMPARE(reader.findAllFiles(QStringLiteral("/"), {QStringLiteral("*.json")}).length(), 1);
        QCOMPARE(reader.findAllFiles(QStringLiteral("/names"), {QStringLiteral("*.json")}).length(), 1);

        QVERIFY(reader.findAllFiles(QStringLiteral("/this/path/does/not/exist"), {}).isEmpty());
    }
}

void TestAddonReader::canReadFiles()
{
    const auto addons = testAddons();
    for (auto *addon : addons)
    {
        AddonReader reader(*addon);

        QVERIFY(!reader.readFile(QStringLiteral("addon.json")).isEmpty());
        QVERIFY(!reader.readFile(QStringLiteral("/names/test.json")).isEmpty());

        expectWarning();
        QVERIFY(reader.readFile(QStringLiteral("/this/file/is/missing.json")).isEmpty());
    }
}

void TestAddonReader::canCheckFiles()
{
    const auto addons = testAddons();
    for (auto *addon : addons)
    {
        AddonReader reader(*addon);

        QVERIFY(reader.checkFile(QStringLiteral("addon.json")));
        QVERIFY(reader.checkFile(QStringLiteral("/names")));
        QVERIFY(reader.checkFile(QStringLiteral("/names/test.json")));
        QVERIFY(!reader.checkFile(QStringLiteral("this-file-does-not-exist")));
        QVERIFY(!reader.checkFile(QStringLiteral("/this/path/does/not/exist")));
    }
}

void TestAddonReader::canLoadFeatures()
{
    const auto addons = testAddons();
    for (auto *addon : addons)
    {
        AddonReader reader(*addon);
        const auto features = reader.getFeatures();
        QVERIFY(features.testFlag(AddonReader::Feature::Names));
        QVERIFY(features.testFlag(AddonReader::Feature::Audio));
    }
}

void TestAddonReader::cleanupTestCase()
{
    disableTestAddons();
}

QTEST_GUILESS_MAIN(TestAddonReader)
#include "testaddonreader.moc"
