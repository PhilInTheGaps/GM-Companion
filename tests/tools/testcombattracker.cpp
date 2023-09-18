#include "src/tools/combat_tracker/combattracker.h"
#include <gtest/gtest.h>

class CombatTrackerTest;

class AccessibleCombatTracker : public CombatTracker
{
    friend CombatTrackerTest;

public:
    using CombatTracker::CombatTracker;
};

class CombatTrackerTest : public ::testing::Test
{
protected:
    static void createTestData(AccessibleCombatTracker &combatTracker);
    [[nodiscard]] static auto getCombatant(int index, AccessibleCombatTracker &combatTracker) -> Combatant *
    {
        return combatTracker.getCombatant(index);
    }

    [[nodiscard]] static auto combatants(AccessibleCombatTracker &combatTracker) -> QList<Combatant *>
    {
        return combatTracker.combatants();
    }

    static void saveToDisk(AccessibleCombatTracker &combatTracker)
    {
        combatTracker.saveToDisk();
    }

    static void resetDelayForAll(AccessibleCombatTracker &combatTracker)
    {
        combatTracker.resetDelayForAll();
    }

    static void setIsDataLoaded(bool isLoaded, AccessibleCombatTracker &combatTracker)
    {
        combatTracker.setIsDataLoaded(isLoaded);
    }
};

void CombatTrackerTest::createTestData(AccessibleCombatTracker &combatTracker)
{
    EXPECT_FALSE(combatTracker.add(QString(), 1, 2, 3, QStringLiteral("Test")));
    EXPECT_TRUE(combatTracker.add(QStringLiteral("Frank"), 1, 0, 1, "Test 1", true));
    EXPECT_TRUE(combatTracker.add(QStringLiteral("Paul"), 1, 0, 2, "Test 2", true));
    EXPECT_TRUE(combatTracker.add(QStringLiteral("Jazz"), 2, 0, 2, "Test 3", true));
    EXPECT_TRUE(combatTracker.add(QStringLiteral("Orc 1"), 3, 0, 1, "Test 4", true));
    EXPECT_TRUE(combatTracker.add(QStringLiteral("Orc 2"), 2, 0, 4, "Test 5", true));

    EXPECT_EQ(combatTracker.currentIndex(), 0);
    EXPECT_FALSE(combatTracker.getCombatant(-1));
    EXPECT_TRUE(combatTracker.getCombatant(0));
    EXPECT_EQ(combatTracker.getCombatant(0)->name(), QStringLiteral("Orc 1"));
    EXPECT_EQ(combatTracker.getCombatant(1)->name(), QStringLiteral("Orc 2"));
    EXPECT_EQ(combatTracker.getCombatant(2)->name(), QStringLiteral("Jazz"));
    EXPECT_EQ(combatTracker.getCombatant(3)->name(), QStringLiteral("Paul"));
    EXPECT_EQ(combatTracker.getCombatant(4)->name(), QStringLiteral("Frank"));
}

TEST_F(CombatTrackerTest, TestNormalUse)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
    combatTracker.clear();

    // No combatant added, so we should not be able to modify one
    EXPECT_FALSE(combatTracker.setIni(0, 5));
    EXPECT_FALSE(combatTracker.setHealth(0, -5));
    EXPECT_FALSE(combatTracker.setPriority(0, 3));
    EXPECT_FALSE(combatTracker.setNotes(0, QStringLiteral("This is a test")));
    EXPECT_FALSE(combatTracker.delayTurn(0));

    // Add some combatants
    createTestData(combatTracker);

    // Check rounds and current index
    combatTracker.next();
    EXPECT_EQ(combatTracker.currentIndex(), 1);
    EXPECT_EQ(combatTracker.currentRound(), 1);
    combatTracker.next();
    combatTracker.next();
    combatTracker.next();
    combatTracker.next();
    EXPECT_EQ(combatTracker.currentIndex(), 0);
    EXPECT_EQ(combatTracker.currentRound(), 2);
    combatTracker.reset();
    EXPECT_EQ(combatTracker.currentIndex(), 0);
    EXPECT_EQ(combatTracker.currentRound(), 1);

    // Delay
    auto *combatant = getCombatant(0, combatTracker);
    EXPECT_TRUE(combatant);
    combatTracker.delayTurn(0);
    EXPECT_EQ(getCombatant(0, combatTracker)->name(), QStringLiteral("Orc 2"));

    // Modify combatant
    combatTracker.setIni(2, 20); // should move to position 0
    combatTracker.setHealth(0, 25);
    combatTracker.setPriority(0, 10);
    combatTracker.setNotes(0, QStringLiteral("Modified"));
    EXPECT_EQ(getCombatant(0, combatTracker)->ini(), 20);
    EXPECT_EQ(getCombatant(0, combatTracker)->health(), 25);
    EXPECT_EQ(getCombatant(0, combatTracker)->priority(), 10);
    EXPECT_EQ(getCombatant(0, combatTracker)->notes(), QStringLiteral("Modified"));
}

TEST_F(CombatTrackerTest, TestSaveLoad)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
    combatTracker.clear();
    createTestData(combatTracker);

    const auto index = combatTracker.currentIndex();
    const auto round = combatTracker.currentRound();

    const auto *combatant = getCombatant(0, combatTracker);
    const auto name = combatant->name();
    const auto notes = combatant->notes();
    const auto ini = combatant->ini();
    const auto health = combatant->health();
    const auto priority = combatant->priority();
    const auto delay = combatant->delay();

    saveToDisk(combatTracker);
    combatTracker.clear(false);

    EXPECT_EQ(combatTracker.currentIndex(), 0);
    EXPECT_EQ(combatTracker.currentRound(), 1);
    EXPECT_EQ(combatants(combatTracker).length(), 0);

    setIsDataLoaded(false, combatTracker);
    combatTracker.loadData();

    EXPECT_EQ(combatTracker.currentIndex(), index);
    EXPECT_EQ(combatTracker.currentRound(), round);

    const auto *loadedCombatant = getCombatant(0, combatTracker);
    EXPECT_EQ(loadedCombatant->name(), name);
    EXPECT_EQ(loadedCombatant->notes(), notes);
    EXPECT_EQ(loadedCombatant->ini(), ini);
    EXPECT_EQ(loadedCombatant->health(), health);
    EXPECT_EQ(loadedCombatant->priority(), priority);
    EXPECT_EQ(loadedCombatant->delay(), delay);
}
