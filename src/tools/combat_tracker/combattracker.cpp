#include "combattracker.h"
#include "logging.h"
#include "utils/utils.h"
#include <algorithm>

CombatTracker::CombatTracker(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
{
    effectTool = new EffectTool(this);
    combatantListModel = new CombatantListModel(this);

    if (engine)
    {
        engine->rootContext()->setContextProperty("combat_tracker", this);
        engine->rootContext()->setContextProperty("combat_tracker_effects", effectTool);
        engine->rootContext()->setContextProperty("combatantListModel", combatantListModel);
    }
}

/**
 * @brief Reset the rounds and the current index
 */
void CombatTracker::resetRounds()
{
    sortByIni(false);

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

    for (auto combatant : m_combatants)
    {
        if (Q_LIKELY(combatant))
        {
            combatant->deleteLater();
        }
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

    qCDebug(gmCombatTracker()) << "New index:" << m_currentIndex;

    emit currentIndexChanged();
}

/**
 * @brief Add a combatant to the list
 * @param name Name of the combatant
 * @param ini Initiative of combatant
 * @param health Health of combatant
 * @param sort If combatant list should be sorted after adding
 */
bool CombatTracker::add(const QString& name, int ini, int health, int priority, const QString &notes, bool sort)
{
    if (!name.isEmpty() && (ini > -1))
    {
        auto combatant = new Combatant(name, notes, ini, health, priority, this);
        m_combatants.append(combatant);
        combatantListModel->setElements(m_combatants);

        if ((m_currentRound == 1) && sort)
        {
            sortByIni();
            m_currentIndex = 0;
            emit currentRoundChanged();
        }

        return true;
    }
    else if (sort)
    {
        sortByIni();
    }

    return false;
}

/**
 * @brief Remove combatant from list
 * @param index Index of the combatant
 */
bool CombatTracker::remove(int index)
{
    if (Q_UNLIKELY(!Utils::isInBounds(m_combatants, index)))
    {
        return false;
    }

    auto *combatant = m_combatants.takeAt(index);
    combatant->deleteLater();

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
    return true;
}

/**
 * @brief Sort the list of combatants by initiative
 */
void CombatTracker::sortByIni(bool keepDelay)
{
    qCDebug(gmCombatTracker()) << "Sorting combatants ...";

    // Reset delay
    if (!keepDelay)
    {
        for (auto combatant : m_combatants)
        {
            if (Q_LIKELY(combatant)) combatant->setDelay(false);
        }
    }

    // Get current combatant to find current index later
    Combatant *combatant = getCombatant(m_currentIndex);

    // Sort
    std::sort(m_combatants.begin(), m_combatants.end(), [keepDelay](Combatant *a, Combatant *b) {
        if (keepDelay && a->delay() && !b->delay()) return false;

        if (keepDelay && !a->delay() && b->delay()) return true;

        if (a->ini() == b->ini())
        {
            return a->priority() > b->priority();
        }

        return a->ini() > b->ini();
    });

    combatantListModel->setElements(m_combatants);

    // Find current index
    if (combatant && (m_currentRound > 0))
    {
        m_currentIndex = m_combatants.indexOf(combatant);
    }

    emit combatantsChanged();
    emit currentIndexChanged();
}

/**
 * @brief Set the initiative of combatant
 * @param index Index of combatant
 * @param ini New initiative of combatant
 */
bool CombatTracker::setIni(int index, int ini)
{
    qCDebug(gmCombatTracker()) << "Setting INI  ...";

    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->setIni(ini);
    sortByIni(true);
    return true;
}

/**
 * @brief Increase or decrease INI at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
bool CombatTracker::modifyIni(int index, int steps)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    auto ini = combatant->ini();
    ini += steps;

    combatant->setIni(ini);
    sortByIni(true);
    return true;
}

/**
 * @brief Set the health of combatant
 * @param index Index of combatant
 * @param ini New health of combatant
 */
bool CombatTracker::setHealth(int index, int health)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->setHealth(health);
    return true;
}

/**
 * @brief Increase or decrease health at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
bool CombatTracker::modifyHealth(int index, int steps)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    auto health = combatant->health();
    health += steps;

    combatant->setHealth(health);
    return true;
}

bool CombatTracker::setPriority(int index, int priority)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    qCDebug(gmCombatTracker()) << "Setting priority of" << QString(*combatant) << "to" << priority << "...";

    combatant->setPriority(priority);
    sortByIni(true);
    return true;
}

bool CombatTracker::modifyPriority(int index, int steps)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    qCDebug(gmCombatTracker()) << "Modifying priority of" << QString(*combatant) << "by" << steps << "...";

    auto priority = combatant->priority();
    priority += steps;

    combatant->setPriority(priority);
    sortByIni(true);
    return true;
}

/**
 * @brief Set the notes on combatant
 * @param index Index of combatant
 * @param ini New notes on combatant
 */
bool CombatTracker::setNotes(int index, const QString &notes)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->setNotes(notes);
    return true;
}

bool CombatTracker::delayTurn(int index)
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->setDelay(true);

    m_combatants.move(m_combatants.indexOf(combatant), m_combatants.size() - 1);
    combatantListModel->setElements(m_combatants);

    emit combatantsChanged();
    emit currentIndexChanged();
    return true;
}

auto CombatTracker::getCombatant(int index) -> Combatant *
{
    if (Q_LIKELY(Utils::isInBounds(m_combatants, index)))
    {
        return m_combatants[index];
    }

    return nullptr;
}
