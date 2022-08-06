#include <QtTest>
#include <QObject>
#include "../../src/tools/combat_tracker/combattracker.h"

class TestCombatTracker : public CombatTracker
{
    Q_OBJECT

public:
    TestCombatTracker(QObject *parent = nullptr) : CombatTracker(nullptr, parent) {}

private slots:
    void testNormalUse();
    void testSaveLoad();

private:
    void createTestData();
};

void TestCombatTracker::createTestData()
{
    QVERIFY(!add(QString(), 1, 2, 3, QStringLiteral("Test")));
    QVERIFY(add(QStringLiteral("Frank"), 1, 0, 1, "Test 1", true));
    QVERIFY(add(QStringLiteral("Paul"), 1, 0, 2, "Test 2", true));
    QVERIFY(add(QStringLiteral("Jazz"), 2, 0, 2, "Test 3", true));
    QVERIFY(add(QStringLiteral("Orc 1"), 3, 0, 1, "Test 4", true));
    QVERIFY(add(QStringLiteral("Orc 2"), 2, 0, 4, "Test 5", true));

    QCOMPARE(currentIndex(), 0);
    QVERIFY(!getCombatant(-1));
    QVERIFY(getCombatant(0));
    QCOMPARE(getCombatant(0)->name(), QStringLiteral("Orc 1"));
    QCOMPARE(getCombatant(1)->name(), QStringLiteral("Orc 2"));
    QCOMPARE(getCombatant(2)->name(), QStringLiteral("Jazz"));
    QCOMPARE(getCombatant(3)->name(), QStringLiteral("Paul"));
    QCOMPARE(getCombatant(4)->name(), QStringLiteral("Frank"));
}

void TestCombatTracker::testNormalUse()
{
    clear();

    // No combatant added, so we should not be able to modify one
    QVERIFY(!setIni(0, 5));
    QVERIFY(!setHealth(0, -5));
    QVERIFY(!setPriority(0, 3));
    QVERIFY(!setNotes(0, QStringLiteral("This is a test")));
    QVERIFY(!delayTurn(0));

    // Add some combatants
    createTestData();

    // Check rounds and current index
    next();
    QCOMPARE(currentIndex(), 1);
    QCOMPARE(currentRound(), 1);
    next();
    next();
    next();
    next();
    QCOMPARE(currentIndex(), 0);
    QCOMPARE(currentRound(), 2);
    reset();
    QCOMPARE(currentIndex(), 0);
    QCOMPARE(currentRound(), 1);

    // Delay
    auto *combatant = getCombatant(0);
    QVERIFY(combatant);
    delayTurn(0);
    QCOMPARE(getCombatant(0)->name(), QStringLiteral("Orc 2"));

    // Modify combatant
    setIni(2, 20); // should move to position 0
    setHealth(0, 25);
    setPriority(0, 10);
    setNotes(0, QStringLiteral("Modified"));
    QCOMPARE(getCombatant(0)->ini(), 20);
    QCOMPARE(getCombatant(0)->health(), 25);
    QCOMPARE(getCombatant(0)->priority(), 10);
    QCOMPARE(getCombatant(0)->notes(), QStringLiteral("Modified"));
}

void TestCombatTracker::testSaveLoad()
{
    clear();
    createTestData();

    const auto index = currentIndex();
    const auto round = currentRound();

    const auto *combatant = getCombatant(0);
    const auto name = combatant->name();
    const auto notes = combatant->notes();
    const auto ini = combatant->ini();
    const auto health = combatant->health();
    const auto priority = combatant->priority();
    const auto delay = combatant->delay();

    saveToDisk();
    clear(false);

    QCOMPARE(currentIndex(), 0);
    QCOMPARE(currentRound(), 1);
    QCOMPARE(combatants().length(), 0);

    setIsDataLoaded(false);
    loadData();

    QCOMPARE(currentIndex(), index);
    QCOMPARE(currentRound(), round);

    const auto *loadedCombatant = getCombatant(0);
    QCOMPARE(loadedCombatant->name(), name);
    QCOMPARE(loadedCombatant->notes(), notes);
    QCOMPARE(loadedCombatant->ini(), ini);
    QCOMPARE(loadedCombatant->health(), health);
    QCOMPARE(loadedCombatant->priority(), priority);
    QCOMPARE(loadedCombatant->delay(), delay);
}

QTEST_APPLESS_MAIN(TestCombatTracker)
#include "testcombattracker.moc"
