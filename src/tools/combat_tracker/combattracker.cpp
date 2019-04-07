#include "combattracker.h"

#include <QDebug>
#include <algorithm>

CombatTracker::CombatTracker(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), qmlEngine(engine)
{
    qDebug() << "Loading Combat Tracker ...";

    effectTool = new EffectTool;

    combatantListModel = new CombatantListModel;
    qmlEngine->rootContext()->setContextProperty("combatantListModel", combatantListModel);
}

/**
 * @brief Reset the rounds and the current index
 */
void CombatTracker::resetRounds()
{
    sortByIni();

    m_currentRound = 1;
    m_currentIndex = 0;

    emit currentRoundChanged();
    emit currentIndexChanged();
}

/**
 * @brief Delete all combatants
 */
void CombatTracker::clear()
{
    combatantListModel->clear();

    for (auto c : m_combatants)
    {
        if (c) c->deleteLater();
    }

    m_combatants.clear();
    m_currentIndex = 0;
    m_currentRound = 0;

    emit combatantsChanged();
    emit currentRoundChanged();
    emit currentIndexChanged();
}

/**
 * @brief Switch to next combatant
 */
void CombatTracker::next()
{
    if (m_currentIndex + 1 < m_combatants.size())
    {
        m_currentIndex++;
    }
    else
    {
        sortByIni();
        m_currentIndex = 0;
        m_currentRound++;
        emit currentRoundChanged();
    }

    qDebug() << m_currentIndex;

    emit currentIndexChanged();
}

/**
 * @brief Add a combatant to the list
 * @param name Name of the combatant
 * @param ini Initiative of combatant
 * @param health Health of combatant
 * @param sort If combatant list should be sorted after adding
 */
void CombatTracker::add(QString name, int ini, int health, QString notes, bool sort)
{
    if (!name.isEmpty() && (ini > -1))
    {
        auto c = new Combatant(name, notes, ini, health);
        m_combatants.append(c);
        combatantListModel->setElements(m_combatants);

        if ((m_currentRound == 0) && sort)
        {
            sortByIni();
            m_currentIndex = 0;
            emit currentRoundChanged();
        }
    }
    else if (sort) sortByIni();
}

/**
 * @brief Remove combatant from list
 * @param index Index of the combatant
 */
void CombatTracker::remove(int index)
{
    if ((index < m_combatants.size()) && (index > -1))
    {
        auto c = m_combatants.takeAt(index);
        c->deleteLater();

        if ((m_currentIndex == index) && (m_currentIndex > 0))
        {
            if (m_currentIndex == m_combatants.size())
            {
                m_currentIndex--;
            }
            else if (m_currentIndex < m_combatants.size() - 1)
            {
                m_currentIndex++;
            }
        }

        combatantListModel->setElements(m_combatants);
        emit combatantsChanged();
        emit currentIndexChanged();
    }
}

/**
 * @brief Sort the list of combatants by initiative
 */
void CombatTracker::sortByIni(bool keepDelay)
{
    qDebug() << "CombatTracker: Sorting combatants ...";

    // Reset delay
    if (!keepDelay)
        for (auto c : m_combatants) if (c) c->setDelay(false);

    // Get current combatant to find current index later
    Combatant *c = nullptr;

    if (m_currentIndex < m_combatants.size()) c = m_combatants[m_currentIndex];

    // Sort
    std::sort(m_combatants.begin(), m_combatants.end(), [ = ](Combatant *a, Combatant *b) {
        if (keepDelay && a->delay() && !b->delay()) return false;

        if (keepDelay && !a->delay() && b->delay()) return true;

        return a->ini() > b->ini();
    });

    combatantListModel->setElements(m_combatants);

    // Find current index
    if (c && (m_currentRound > 0)) m_currentIndex = m_combatants.indexOf(c);

    emit combatantsChanged();
    emit currentIndexChanged();
}

/**
 * @brief Set the initiative of combatant
 * @param index Index of combatant
 * @param ini New initiative of combatant
 */
void CombatTracker::setIni(int index, int ini)
{
    qDebug() << "Setting INI  ...";

    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        if (ini > -1) c->setIni(ini);
        else c->setIni(0);

        sortByIni(true);
    }
}

/**
 * @brief Increase or decrease INI at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
void CombatTracker::modifyIni(int index, int steps)
{
    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        auto ini = c->ini();
        ini += steps;

        if (ini > -1) c->setIni(ini);
        else c->setIni(0);

        sortByIni(true);
    }
}

/**
 * @brief Set the health of combatant
 * @param index Index of combatant
 * @param ini New health of combatant
 */
void CombatTracker::setHealth(int index, int health)
{
    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        if (health > -100) c->setHealth(health);
        else c->setHealth(0);
    }
}

/**
 * @brief Increase or decrease health at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
void CombatTracker::modifyHealth(int index, int steps)
{
    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        auto health = c->health();
        health += steps;

        if (health > -100) c->setHealth(health);
        else c->setHealth(0);
    }
}

/**
 * @brief Set the notes on combatant
 * @param index Index of combatant
 * @param ini New notes on combatant
 */
void CombatTracker::setNotes(int index, QString notes)
{
    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        c->setNotes(notes);
    }
}

void CombatTracker::delayTurn(int index)
{
    if (index < m_combatants.size())
    {
        auto c = m_combatants[index];

        if (!c) return;

        c->setDelay(true);

        m_combatants.move(m_combatants.indexOf(c), m_combatants.size() - 1);
        combatantListModel->setElements(m_combatants);

        emit combatantsChanged();
        emit currentIndexChanged();
    }
}
