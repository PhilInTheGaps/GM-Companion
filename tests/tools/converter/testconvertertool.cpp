#include "src/common/utils/fileutils.h"
#include "src/tools/converter/convertertool.h"
#include "tests/testhelper/abstracttest.h"
#include <QtTest>

class TestConverterTool : public AbstractTest
{
    Q_OBJECT

private slots:
    void initTestCase();
    void canConvertUnits();
    void canConvertProjectUnits();
    void canNotConvertInvalidUnits();
    void canLoadProjects();
    void canLoadAddonProjects();
    void canHandleIncompleteProjects();
    void cleanupTestCase();
};

void TestConverterTool::initTestCase()
{
    enableTestAddons();
}

void TestConverterTool::canConvertUnits()
{
    const ConverterTool converter(nullptr);

    auto *fromUnit = new ConverterUnit(QStringLiteral("Inch"), 0.0254, this);
    auto *toUnit = new ConverterUnit(QStringLiteral("Centimeter"), 0.01, this);

    const auto result = converter.convert(fromUnit, QStringLiteral("2.05"), toUnit);
    QCOMPARE(result.toDouble(), 5.207);
}

void TestConverterTool::canConvertProjectUnits()
{
    ConverterTool converter(nullptr);

    auto *fromUnit = new ConverterUnit(QStringLiteral("Inch"), 0.0254, this);
    auto *toUnit = new ConverterUnit(QStringLiteral("Centimeter"), 0.01, this);
    auto *category = new ConverterCategory(QStringLiteral("Category"), {fromUnit, toUnit}, this);
    auto *project = new ConverterProject(QStringLiteral("Test"), {category}, this);

    converter.currentProject(project);
    converter.toUnit(toUnit);

    const auto result = converter.convert(converter.fromUnit(), QStringLiteral("2.05"), converter.toUnit());
    QCOMPARE(result.toDouble(), 5.207);
}

void TestConverterTool::canNotConvertInvalidUnits()
{
    const ConverterTool converter(nullptr);
    auto *fromUnit = new ConverterUnit(QStringLiteral("From"), 0, this);
    auto *toUnit = new ConverterUnit(QStringLiteral("To"), 0, this);

    QVERIFY(converter.convert(nullptr, QStringLiteral("1"), toUnit).isEmpty());
    QVERIFY(converter.convert(fromUnit, QStringLiteral("1"), nullptr).isEmpty());
    QVERIFY(converter.convert(nullptr, QStringLiteral("1"), nullptr).isEmpty());
    QVERIFY(converter.convert(fromUnit, QStringLiteral("1"), toUnit).isEmpty());
}

void TestConverterTool::canLoadProjects()
{
    ConverterTool converter(nullptr);
    QVERIFY(converter.projects().isEmpty());

    converter.loadData();
    QVERIFY(!converter.projects().isEmpty());
}

void TestConverterTool::canLoadAddonProjects()
{
    const auto unitDir = FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
    const auto backupDir = backupUserFolder(unitDir);

    ConverterTool converter(nullptr);
    QVERIFY(converter.projects().isEmpty());

    converter.loadData();

    const auto projects = converter.projects();

    restoreUserFolder(backupDir, unitDir);

    // should include one default project and two addon projects
    QCOMPARE(converter.projects().length(), 3);
}

void TestConverterTool::canHandleIncompleteProjects()
{
    ConverterTool converter(nullptr);
    converter.currentProject(new ConverterProject(QLatin1String(), this));
    converter.currentProject(nullptr);
    converter.currentProject(
        new ConverterProject(QLatin1String(), {new ConverterCategory(QLatin1String(), this)}, this));
}

void TestConverterTool::cleanupTestCase()
{
    disableTestAddons();
}

QTEST_GUILESS_MAIN(TestConverterTool)
#include "testconvertertool.moc"
