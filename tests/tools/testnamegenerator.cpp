#include <QtTest>
#include <QObject>
#include "src/tools/generators/names/namegenerator.h"

class TestNameGenerator : public NameGenerator
{
    Q_OBJECT

public:
    TestNameGenerator(QObject *parent = nullptr) : NameGenerator(nullptr, parent) {}

private slots:
    void initTestCase();
    void testNormalUse();
};

void TestNameGenerator::initTestCase()
{
    Q_INIT_RESOURCE(libresources);
    QVERIFY(!isDataLoaded());
    loadData();
}

void TestNameGenerator::testNormalUse()
{
    QVERIFY(categories().length() > 0);
    QVERIFY(loadCategory(0));

    QVERIFY(generators().length() > 0);
    QVERIFY(!generators()[0]->name().isEmpty());

    QVERIFY(loadGenerator(0));
    QVERIFY(currentGenerator()->generate(5));
    auto names = currentGenerator()->generatedNames()[0];
    QCOMPARE(names.length(), 5);

    QVERIFY(currentGenerator()->setCategoryEnabled(0, false));
    QVERIFY(currentGenerator()->generate(5));
    QCOMPARE(currentGenerator()->generatedNames()[0].length(), 0);
    QCOMPARE(currentGenerator()->generatedNames()[1].length(), 5);
    QVERIFY(currentGenerator()->setCategoryEnabled(0, true));

    currentGenerator()->activePrefix(1);
    QVERIFY(currentGenerator()->generate(1));
    QCOMPARE(currentGenerator()->generatedNames()[0][0].split(" ").length(), 3); // Should be like "Dr. Name Something"
    currentGenerator()->activePrefix(0);

    currentGenerator()->activeSuffix(1);
    QVERIFY(currentGenerator()->generate(1));
    QCOMPARE(currentGenerator()->generatedNames()[0][0].split(" ").length(), 3); // Should be like "Name Something III"

    currentGenerator()->activePrefix(1);
    currentGenerator()->activeSuffix(1);
    QVERIFY(currentGenerator()->generate(1));
    QCOMPARE(currentGenerator()->generatedNames()[0][0].split(" ").length(), 4); // Should be like "Prof. Name Something III"
}

QTEST_APPLESS_MAIN(TestNameGenerator)
#include "testnamegenerator.moc"
