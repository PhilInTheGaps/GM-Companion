#include "src/tools/combat_tracker/combattracker.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

class CombatTrackerTest;

class AccessibleCombatTracker : public CombatTracker
{
    Q_OBJECT
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
    EXPECT_FALSE(combatTracker.add(u""_s, 1, 2, 3, u"Test"_s));
    EXPECT_TRUE(combatTracker.add(u"Frank"_s, 1, 0, 1, u"Test 1"_s, true));
    EXPECT_TRUE(combatTracker.add(u"Paul"_s, 1, 0, 2, u"Test 2"_s, true));
    EXPECT_TRUE(combatTracker.add(u"Jazz"_s, 2, 0, 2, u"Test 3"_s, true));
    EXPECT_TRUE(combatTracker.add(u"Orc 1"_s, 3, 0, 1, u"Test 4"_s, true));
    EXPECT_TRUE(combatTracker.add(u"Orc 2"_s, 2, 0, 4, u"Test 5"_s, true));

    EXPECT_EQ(combatTracker.currentIndex(), 0);
    EXPECT_FALSE(combatTracker.getCombatant(-1));
    EXPECT_TRUE(combatTracker.getCombatant(0));
    EXPECT_EQ(combatTracker.getCombatant(0)->name(), u"Orc 1"_s);
    EXPECT_EQ(combatTracker.getCombatant(1)->name(), u"Orc 2"_s);
    EXPECT_EQ(combatTracker.getCombatant(2)->name(), u"Jazz"_s);
    EXPECT_EQ(combatTracker.getCombatant(3)->name(), u"Paul"_s);
    EXPECT_EQ(combatTracker.getCombatant(4)->name(), u"Frank"_s);
}

TEST_F(CombatTrackerTest, TestNormalUse)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);

    // No combatant added, so we should not be able to modify one
    EXPECT_FALSE(combatTracker.setIni(0, 5));
    EXPECT_FALSE(combatTracker.setHealth(0, -5));
    EXPECT_FALSE(combatTracker.setPriority(0, 3));
    EXPECT_FALSE(combatTracker.setNotes(0, u"This is a test"_s));
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
    EXPECT_EQ(getCombatant(0, combatTracker)->name(), u"Orc 2"_s);

    // Modify combatant
    combatTracker.setIni(2, 20); // should move to position 0
    combatTracker.setHealth(0, 25);
    combatTracker.setPriority(0, 10);
    combatTracker.setNotes(0, u"Modified"_s);
    EXPECT_EQ(getCombatant(0, combatTracker)->ini(), 20);
    EXPECT_EQ(getCombatant(0, combatTracker)->health(), 25);
    EXPECT_EQ(getCombatant(0, combatTracker)->priority(), 10);
    EXPECT_EQ(getCombatant(0, combatTracker)->notes(), u"Modified"_s);

    // Modify using steps
    combatTracker.modifyIni(0, 1);
    EXPECT_EQ(getCombatant(0, combatTracker)->ini(), 21);
    combatTracker.modifyIni(0, -1);
    EXPECT_EQ(getCombatant(0, combatTracker)->ini(), 20);

    combatTracker.modifyHealth(0, 1);
    EXPECT_EQ(getCombatant(0, combatTracker)->health(), 26);
    combatTracker.modifyHealth(0, -1);
    EXPECT_EQ(getCombatant(0, combatTracker)->health(), 25);

    combatTracker.modifyPriority(0, 1);
    EXPECT_EQ(getCombatant(0, combatTracker)->priority(), 11);
    combatTracker.modifyPriority(0, -1);
    EXPECT_EQ(getCombatant(0, combatTracker)->priority(), 10);
}

TEST_F(CombatTrackerTest, CanRemoveCombatants)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);

    EXPECT_FALSE(combatTracker.remove(-1));
    EXPECT_FALSE(combatTracker.remove(0));
    EXPECT_FALSE(combatTracker.remove(1));

    createTestData(combatTracker);

    EXPECT_FALSE(combatTracker.remove(-1));
    EXPECT_FALSE(combatTracker.remove(5));

    EXPECT_TRUE(combatTracker.remove(1)); // second
    EXPECT_TRUE(combatTracker.remove(3)); // last
    EXPECT_FALSE(combatTracker.remove(3));
    EXPECT_TRUE(combatTracker.remove(2)); // last
    EXPECT_TRUE(combatTracker.remove(0)); // first
    EXPECT_TRUE(combatTracker.remove(0)); // first

    EXPECT_FALSE(combatTracker.remove(0));
}

TEST_F(CombatTrackerTest, CanRemoveLastAndActiveCombatant)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
    createTestData(combatTracker);

    combatTracker.reset();
    combatTracker.next();
    combatTracker.next();
    combatTracker.next();
    combatTracker.next();

    EXPECT_EQ(combatTracker.model()->rowCount(QModelIndex()), 5);
    EXPECT_EQ(combatTracker.currentIndex(), 4);

    // removing the last entry should select the new last entry
    EXPECT_TRUE(combatTracker.remove(4));
    EXPECT_EQ(combatTracker.currentIndex(), 3);
    EXPECT_EQ(combatTracker.model()->rowCount(QModelIndex()), 4);
}

TEST_F(CombatTrackerTest, CanRemoveSecondLastAndActiveCombatant)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
    createTestData(combatTracker);

    combatTracker.reset();
    combatTracker.next();
    combatTracker.next();
    combatTracker.next();

    EXPECT_EQ(combatTracker.model()->rowCount(QModelIndex()), 5);
    EXPECT_EQ(combatTracker.currentIndex(), 3);

    // removing the second to last entry should select the last entry
    EXPECT_TRUE(combatTracker.remove(3));
    EXPECT_EQ(combatTracker.currentIndex(), 3);
    EXPECT_EQ(combatTracker.model()->rowCount(QModelIndex()), 4);
}

TEST_F(CombatTrackerTest, TestSaveLoad)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
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

TEST_F(CombatTrackerTest, CanAccessModel)
{
    AccessibleCombatTracker combatTracker(nullptr, nullptr);
    createTestData(combatTracker);

    EXPECT_TRUE(combatTracker.model());
}

#include "testcombattracker.moc"
