#include "combattracker.h"

#include <QDebug>

CombatTracker::CombatTracker(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Combat Tracker ...";
}

QString CombatTracker::combatantName()
{
    return l_combatants.at(l_currentCombatantIndex).name;
}

int CombatTracker::combatantIni()
{
    return l_combatants.at(l_currentCombatantIndex).ini;
}

QString CombatTracker::combatantHealth()
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);
    return QString::number(c.health) + " / " + QString::number(c.max_health);
}

QString CombatTracker::combatantStatus()
{
    return l_combatants.at(l_currentCombatantIndex).status;
}

void CombatTracker::nextCombatant()
{
    if (l_currentCombatantIndex < l_combatants.size() - 1)
    {
        l_currentCombatantIndex++;
        emit combatantNameChanged();
        emit combatantIniChanged();
        emit combatantHealthChanged();
        emit combatantStatusChanged();
    }
    else
    {
        l_currentCombatantIndex = 0;
        l_currentRound++;
    }
}

void CombatTracker::addCombatant(QString name, int ini, int health)
{
    Combatant c;
    c.name = name;
    c.ini = ini;
    c.health = health;

    l_combatants.append(c);
}
