#include "src/common/settings/settingsmanager.h"
#include "src/common/utils/fileutils.h"
#include "src/tools/converter/convertertool.h"
#include "tests/testhelper/staticabstracttest.h"
#include <gtest/gtest.h>

using namespace Common::Settings;

class ConverterToolTest : public StaticAbstractTest
{
public:
    static void SetUpTestSuite()
    {
        tempDir.setAutoRemove(true);
        testAddons = enableTestAddons();
    }

    static void TearDownTestSuite()
    {
        disableTestAddons(testAddons);
    }

protected:
    inline static QTemporaryDir tempDir;
    inline static QList<Addon *> testAddons;
};

TEST_F(ConverterToolTest, CanConvertUnits)
{
    const ConverterTool converter(nullptr);

    ConverterUnit fromUnit(QStringLiteral("Inch"), 0.0254, nullptr);
    ConverterUnit toUnit(QStringLiteral("Centimeter"), 0.01, nullptr);

    const auto result = converter.convert(&fromUnit, SettingsManager::getLanguage().toString(2.05), &toUnit);
    EXPECT_EQ(result.toDouble(), 5.207);
}

TEST_F(ConverterToolTest, CanConvertProjectUnits)
{
    ConverterTool converter(nullptr);

    ConverterUnit fromUnit(QStringLiteral("Inch"), 0.0254, nullptr);
    ConverterUnit toUnit(QStringLiteral("Centimeter"), 0.01, nullptr);
    ConverterCategory category(QStringLiteral("Category"), {&fromUnit, &toUnit}, nullptr);
    ConverterProject project(QStringLiteral("Test"), {&category}, nullptr);

    converter.currentProject(&project);
    converter.toUnit(&toUnit);

    const auto result =
        ConverterTool::convert(converter.fromUnit(), SettingsManager::getLanguage().toString(2.05), converter.toUnit());
    EXPECT_EQ(result.toDouble(), 5.207);
}

TEST_F(ConverterToolTest, CanNotConvertInvalidUnits)
{
    const ConverterTool converter(nullptr);
    ConverterUnit fromUnit(QStringLiteral("From"), 0, nullptr);
    ConverterUnit toUnit(QStringLiteral("To"), 0, nullptr);

    EXPECT_TRUE(converter.convert(nullptr, QStringLiteral("1"), &toUnit).isEmpty());
    EXPECT_TRUE(converter.convert(&fromUnit, QStringLiteral("1"), nullptr).isEmpty());
    EXPECT_TRUE(converter.convert(nullptr, QStringLiteral("1"), nullptr).isEmpty());
    EXPECT_TRUE(converter.convert(&fromUnit, QStringLiteral("1"), &toUnit).isEmpty());
}

TEST_F(ConverterToolTest, CanLoadProjects)
{
    ConverterTool converter(nullptr);
    EXPECT_TRUE(converter.projects().isEmpty());

    converter.loadData();
    EXPECT_FALSE(converter.projects().isEmpty());
}

TEST_F(ConverterToolTest, CanLoadAddonProjects)
{
    const auto unitDir = FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
    const auto backupDir = backupUserFolder(unitDir, tempDir);

    ConverterTool converter(nullptr);
    EXPECT_TRUE(converter.projects().isEmpty());

    converter.loadData();

    restoreUserFolder(backupDir, unitDir);

    // should include one default project and two addon projects
    EXPECT_EQ(converter.projects().length(), 3);
}

TEST_F(ConverterToolTest, CanHandleIncompleteProjects)
{
    ConverterTool converter(nullptr);
    converter.currentProject(new ConverterProject(QLatin1String(), &converter));
    converter.currentProject(nullptr);
    converter.currentProject(
        new ConverterProject(QLatin1String(), {new ConverterCategory(QLatin1String(), &converter)}, &converter));
}
