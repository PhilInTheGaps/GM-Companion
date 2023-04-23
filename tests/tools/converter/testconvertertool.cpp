#include "src/tools/converter/convertertool.h"
#include "tests/testhelper/abstracttest.h"
#include <QtTest>

class TestConverterTool : public AbstractTest
{
    Q_OBJECT

private slots:
    void canConvertUnits();
    void canConvertProjectUnits();
    void canNotConvertInvalidUnits();
    void canLoadProjects();
    void canHandleIncompleteProjects();
};

void TestConverterTool::canConvertUnits()
{
    const ConverterTool converter(nullptr, nullptr);

    auto *fromUnit = new ConverterUnit(QStringLiteral("Inch"), 0.0254, this);
    auto *toUnit = new ConverterUnit(QStringLiteral("Centimeter"), 0.01, this);

    const auto result = converter.convert(fromUnit, QStringLiteral("2.05"), toUnit);
    QCOMPARE(result.toDouble(), 5.207);
}

void TestConverterTool::canConvertProjectUnits()
{
    ConverterTool converter(nullptr, nullptr);

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
    const ConverterTool converter(nullptr, nullptr);
    auto *fromUnit = new ConverterUnit(QStringLiteral("From"), 0, this);
    auto *toUnit = new ConverterUnit(QStringLiteral("To"), 0, this);

    QVERIFY(converter.convert(nullptr, QStringLiteral("1"), toUnit).isEmpty());
    QVERIFY(converter.convert(fromUnit, QStringLiteral("1"), nullptr).isEmpty());
    QVERIFY(converter.convert(nullptr, QStringLiteral("1"), nullptr).isEmpty());
    QVERIFY(converter.convert(fromUnit, QStringLiteral("1"), toUnit).isEmpty());
}

void TestConverterTool::canLoadProjects()
{
    ConverterTool converter(nullptr, nullptr);
    QVERIFY(converter.projects().isEmpty());

    converter.loadData();
    QVERIFY(!converter.projects().isEmpty());
}

void TestConverterTool::canHandleIncompleteProjects()
{
    ConverterTool converter(nullptr, nullptr);
    converter.currentProject(new ConverterProject(QLatin1String(), this));
    converter.currentProject(nullptr);
    converter.currentProject(
        new ConverterProject(QLatin1String(), {new ConverterCategory(QLatin1String(), this)}, this));
}

QTEST_APPLESS_MAIN(TestConverterTool)
#include "testconvertertool.moc"
