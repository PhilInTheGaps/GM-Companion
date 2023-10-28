#include "combattrackerstate.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

void CombatTrackerState::reset()
{
    currentIndex(0);
    currentRound(1);
}

void CombatTrackerState::clear()
{
    reset();
    m_model.clear();

    foreach (auto *combatant, m_combatants)
    {
        if (Q_LIKELY(combatant)) combatant->deleteLater();
    }

    m_combatants.clear();
}

void CombatTrackerState::startNewRound()
{
    currentIndex(0);
    currentRound(currentRound() + 1);
}

void CombatTrackerState::moveToNextCombatant()
{
    currentIndex(currentIndex() + 1);
}

void CombatTrackerState::moveToPreviousCombatant()
{
    currentIndex(currentIndex() - 1);
}

void CombatTrackerState::moveCombatant(int from, int to)
{
    m_combatants.move(from, to);
    m_model.move(from, to);
    emit currentIndexChanged(currentIndex());
}

void CombatTrackerState::moveCombatantToBack(int from)
{
    moveCombatant(from, m_combatants.size() - 1);
}

void CombatTrackerState::addCombatant(Combatant *combatant)
{
    m_combatants.append(combatant);
    m_model.append(combatant);
}

void CombatTrackerState::removeCombatant(int index)
{
    m_model.remove(index);

    auto *combatant = m_combatants.takeAt(index);
    combatant->deleteLater();

    if ((currentIndex() == index) && (currentIndex() > 0))
    {
        if (currentIndex() == m_combatants.size())
        {
            moveToPreviousCombatant();
        }
        else if (currentIndex() < m_combatants.size() - 1)
        {
            moveToNextCombatant();
        }
    }

    emit currentIndexChanged(currentIndex());
}

void CombatTrackerState::sortCombatants(bool keepDelay)
{
    std::sort(m_combatants.begin(), m_combatants.end(), [keepDelay](const Combatant *a, const Combatant *b) {
        if (keepDelay && a->delay() && !b->delay()) return false;

        if (keepDelay && !a->delay() && b->delay()) return true;

        if (a->ini() == b->ini())
        {
            return a->priority() > b->priority();
        }

        return a->ini() > b->ini();
    });

    loadModel();
}

void CombatTrackerState::load(const QJsonDocument &json)
{
    clear();

    auto state = json.object();

    foreach (auto combatant, state["combatants"_L1].toArray())
    {
        m_combatants << Combatant::fromJson(combatant.toObject(), this);
    }

    loadModel();

    currentIndex(state["currentIndex"_L1].toInt());
    currentRound(state["currentRound"_L1].toInt());
}

auto CombatTrackerState::serialize() const -> QJsonDocument
{
    QJsonArray combatants;
    foreach (auto *combatant, m_combatants)
    {
        if (Q_LIKELY(combatant))
        {
            combatants << combatant->toJson();
        }
    }

    const auto state =
        QJsonObject({{"currentIndex", currentIndex()}, {"currentRound", currentRound()}, {"combatants", combatants}});
    return QJsonDocument(state);
}

void CombatTrackerState::loadModel()
{
    m_model.setElements(m_combatants);
    emit currentIndexChanged(currentIndex());
}
