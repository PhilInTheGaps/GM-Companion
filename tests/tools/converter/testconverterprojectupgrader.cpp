#include "src/tools/converter/project/converterproject.h"
#include "src/tools/converter/project/converterprojectupgrader.h"
#include "tests/testhelper/abstracttest.h"
#include <QtTest>

class TestConverterProjectUpgrader : public AbstractTest
{
    Q_OBJECT

private slots:
    void canUpgradeProject();
    void canNotUpgradeNewerProject();
};

void TestConverterProjectUpgrader::canUpgradeProject()
{
    const auto projectData = readResource(QStringLiteral(":/resources/units/units_v1.ini"));
    QVERIFY2(!projectData.isEmpty(), "Project file does not contain data");

    ConverterProjectUpgrader upgrader;
    upgrader.parse(projectData);
    const auto result = upgrader.run();
    QVERIFY(!result.isEmpty());

    const auto json = QJsonDocument::fromJson(result).object();
    const ConverterProject project(json, this);

    QCOMPARE(project.version(), 2);
    QCOMPARE(project.name(), QStringLiteral("General"));

    const auto categories = project.categories();
    QCOMPARE(categories.length(), 5);
    QCOMPARE(categories[0]->name(), QStringLiteral("Area"));
    QCOMPARE(categories[4]->name(), QStringLiteral("Weight"));

    const auto units = categories[0]->units();
    QCOMPARE(units.length(), 12);
    QCOMPARE(units[0]->name(), QStringLiteral("Square Meter"));
    QCOMPARE(units[0]->value(), 1);
    QCOMPARE(units[1]->name(), QStringLiteral("Square Kilometer"));
    QCOMPARE(units[1]->value(), 1000000);
}

void TestConverterProjectUpgrader::canNotUpgradeNewerProject()
{
    const auto projectData = readResource(QStringLiteral(":/units/units.json"));
    QVERIFY2(!projectData.isEmpty(), "Project file does not contain data");

    ConverterProjectUpgrader upgrader;
    upgrader.parse(projectData);
    const auto result = upgrader.run();
    QVERIFY(result.isEmpty());
}

QTEST_APPLESS_MAIN(TestConverterProjectUpgrader)
#include "testconverterprojectupgrader.moc"
