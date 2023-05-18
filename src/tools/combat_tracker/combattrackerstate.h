#pragma once

#include "combatant.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonDocument>
#include <QList>
#include <QObject>

class CombatTrackerState : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_VAL2(int, currentIndex, 0)
    AUTO_PROPERTY_VAL2(int, currentRound, 1)

public:
    using QObject::QObject;

    auto model() -> CombatantListModel &
    {
        return m_model;
    };

    void reset();
    void clear();

    void moveToNextCombatant();
    void moveToPreviousCombatant();
    void startNewRound();

    [[nodiscard]] auto combatants() const -> QList<Combatant *>
    {
        return m_combatants;
    }
    void moveCombatant(int from, int to);
    void moveCombatantToBack(int from);
    void addCombatant(Combatant *combatant);
    void removeCombatant(int index);
    void sortCombatants(bool keepDelay);

    void load(const QJsonDocument &json);
    [[nodiscard]] auto serialize() const -> QJsonDocument;

private:
    QList<Combatant *> m_combatants;
    CombatantListModel m_model;

    void loadModel();
};
