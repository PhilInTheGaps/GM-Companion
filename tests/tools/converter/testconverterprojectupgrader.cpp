#include "src/tools/converter/project/converterproject.h"
#include "src/tools/converter/project/converterprojectupgrader.h"
#include "tests/testhelper/abstracttest.h"
#include <gtest/gtest.h>

class ConverterProjectUpgraderTest : public AbstractTest
{
};

TEST_F(ConverterProjectUpgraderTest, CanUpgradeProject)
{
    const auto projectData = readResource(QStringLiteral(":/resources/units/units_v1.ini"));
    EXPECT_FALSE(projectData.isEmpty()) << "Project file does not contain data";

    ConverterProjectUpgrader upgrader;
    upgrader.parse(projectData);
    const auto result = upgrader.run();
    EXPECT_FALSE(result.isEmpty());

    const auto json = QJsonDocument::fromJson(result).object();
    const ConverterProject project(json, nullptr);

    EXPECT_EQ(project.version(), 2);
    EXPECT_EQ(project.name(), QStringLiteral("General"));

    const auto categories = project.categories();
    EXPECT_EQ(categories.length(), 5);
    EXPECT_EQ(categories[0]->name(), QStringLiteral("Area"));
    EXPECT_EQ(categories[4]->name(), QStringLiteral("Weight"));

    const auto units = categories[0]->units();
    EXPECT_EQ(units.length(), 12);
    EXPECT_EQ(units[0]->name(), QStringLiteral("Square Meter"));
    EXPECT_EQ(units[0]->value(), 1);
    EXPECT_EQ(units[1]->name(), QStringLiteral("Square Kilometer"));
    EXPECT_EQ(units[1]->value(), 1000000);
}

TEST_F(ConverterProjectUpgraderTest, CanNotUpgradeNewerProject)
{
    const auto projectData = readResource(QStringLiteral(":/units/units.json"));
    EXPECT_FALSE(projectData.isEmpty()) << "Project file does not contain data";

    ConverterProjectUpgrader upgrader;
    upgrader.parse(projectData);
    const auto result = upgrader.run();
    EXPECT_TRUE(result.isEmpty());
}
