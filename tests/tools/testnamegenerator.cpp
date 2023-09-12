#include "abstracttest.h"
#include "src/tools/generators/names/namegenerator.h"
#include <QDebug>
#include <QObject>
#include <QtTest>
#include <limits>

class TestNameGenerator : public AbstractTest
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testNormalUse();
    void cannotLoadInvalidGenerators();
    void cleanupTestCase();

private:
    NameGenerator ng = NameGenerator(this);
};

void TestNameGenerator::initTestCase()
{
    enableTestAddons();

    QVERIFY(!ng.isDataLoaded());
    ng.loadData();
}

void TestNameGenerator::testNormalUse()
{
    const auto categories = ng.categories();
    QVERIFY(categories.length() > 2); // default category + 2 addon categories

    for (int i = 0; i < 3; i++)
    {
        qDebug() << "Testing category" << categories[i] << "...";

        // can load category
        QVERIFY(ng.loadCategory(i));

        // can select a generator
        const auto generators = ng.generators();
        QVERIFY(generators.length() > 0);
        QVERIFY(!generators[0]->name().isEmpty());

        for (int j = 0; j < ng.generators().length(); j++)
        {
            qDebug() << "Testing generator" << generators[j]->name() << "...";

            // can generate names
            QVERIFY(ng.loadGenerator(j));
            QVERIFY(ng.currentGenerator()->generate(5));
            auto names = ng.currentGenerator()->generatedNames()[0];
            QCOMPARE(names.length(), 5);

            int namePartsCount = names.first().split(QStringLiteral(" ")).length();

            // can disable a category and only generate names from the second one
            QVERIFY(ng.currentGenerator()->setCategoryEnabled(0, false));
            QVERIFY(ng.currentGenerator()->generate(5));
            QCOMPARE(ng.currentGenerator()->generatedNames()[0].length(), 0);
            QCOMPARE(ng.currentGenerator()->generatedNames()[1].length(), 5);
            QVERIFY(ng.currentGenerator()->setCategoryEnabled(0, true));

            // can generate name with prefix
            if (ng.currentGenerator()->prefixes().length() > 1) // prefix 0 is "none"
            {
                ng.currentGenerator()->activePrefix(1);
                QVERIFY(ng.currentGenerator()->generate(1));
                QCOMPARE(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                         namePartsCount + 1); // Should be like "Dr. Name Something"
                ng.currentGenerator()->activePrefix(0);
            }

            // can generate name with suffix
            if (ng.currentGenerator()->suffixes().length() > 1) // suffix 0 is "none"
            {
                ng.currentGenerator()->activeSuffix(1);
                QVERIFY(ng.currentGenerator()->generate(1));
                QCOMPARE(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                         namePartsCount + 1); // Should be like "Name Something III"
            }

            // can generate name with prefix and suffix
            if (ng.currentGenerator()->prefixes().length() > 1 && ng.currentGenerator()->suffixes().length() > 1)
            {
                ng.currentGenerator()->activePrefix(1);
                ng.currentGenerator()->activeSuffix(1);
                QVERIFY(ng.currentGenerator()->generate(1));
                QCOMPARE(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                         namePartsCount + 2); // Should be like "Prof. Name Something III"
            }
        }
    }
}

void TestNameGenerator::cannotLoadInvalidGenerators()
{
    const auto maxInt = std::numeric_limits<int>::max();

    QVERIFY(!ng.loadCategory(-1));
    QVERIFY(!ng.loadCategory(maxInt));
    QVERIFY(ng.loadCategory(0));

    QVERIFY(!ng.loadGenerator(-1));
    QVERIFY(!ng.loadGenerator(maxInt));
    QVERIFY(ng.loadGenerator(0));

    QVERIFY(!ng.currentGenerator()->generate(-1));
}

void TestNameGenerator::cleanupTestCase()
{
    disableTestAddons();
}

QTEST_GUILESS_MAIN(TestNameGenerator)
#include "testnamegenerator.moc"
