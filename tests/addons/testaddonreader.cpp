#include "src/addons/addon_reader/addonreader.h"
#include "tests/testhelper/abstracttest.h"

class AddonReaderTest : public AbstractTest
{
protected:
    QList<Addon *> testAddons;

    void SetUp() override
    {
        testAddons = enableTestAddons();
    }

    void TearDown() override
    {
        disableTestAddons(testAddons);
    }
};

TEST_F(AddonReaderTest, CanFindAllFiles)
{
    foreach (auto *addon, testAddons)
    {
        AddonReader reader(*addon);

        EXPECT_FALSE(reader.findAllFiles(QStringLiteral("/"), {}).isEmpty());
        EXPECT_EQ(reader.findAllFiles(QStringLiteral("/"), {QStringLiteral("*.json")}).length(), 1);
        EXPECT_EQ(reader.findAllFiles(QStringLiteral("/names"), {QStringLiteral("*.json")}).length(), 1);

        EXPECT_TRUE(reader.findAllFiles(QStringLiteral("/this/path/does/not/exist"), {}).isEmpty());
    }
}

TEST_F(AddonReaderTest, CanReadFiles)
{
    foreach (auto *addon, testAddons)
    {
        AddonReader reader(*addon);

        EXPECT_FALSE(reader.readFile(QStringLiteral("addon.json")).isEmpty());
        EXPECT_FALSE(reader.readFile(QStringLiteral("/names/test.json")).isEmpty());

        expectWarning();
        EXPECT_TRUE(reader.readFile(QStringLiteral("/this/file/is/missing.json")).isEmpty());
    }
}

TEST_F(AddonReaderTest, CanCheckFiles)
{
    foreach (auto *addon, testAddons)
    {
        AddonReader reader(*addon);

        EXPECT_TRUE(reader.checkFile(QStringLiteral("addon.json")));
        EXPECT_TRUE(reader.checkFile(QStringLiteral("/names")));
        EXPECT_TRUE(reader.checkFile(QStringLiteral("/names/test.json")));
        EXPECT_FALSE(reader.checkFile(QStringLiteral("this-file-does-not-exist")));
        EXPECT_FALSE(reader.checkFile(QStringLiteral("/this/path/does/not/exist")));
    }
}

TEST_F(AddonReaderTest, CanLoadFeatures)
{
    foreach (auto *addon, testAddons)
    {
        AddonReader reader(*addon);
        const auto features = reader.getFeatures();
        EXPECT_TRUE(features.testFlag(AddonReader::Feature::Names));
        EXPECT_TRUE(features.testFlag(AddonReader::Feature::Audio));
        EXPECT_TRUE(features.testFlag(AddonReader::Feature::Units));
    }
}
