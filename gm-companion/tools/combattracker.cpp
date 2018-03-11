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

int CombatTracker::combatantHealth()
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    return c.health;
}

int CombatTracker::combatantMaxHealth()
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    return c.max_health;
}

QString CombatTracker::combatantStatus()
{
    return l_combatants.at(l_currentCombatantIndex).status;
}

int CombatTracker::currentRound()
{
    return l_currentRound;
}

// Return the latest combatant added to the list
QStringList CombatTracker::latestAddedCombatant()
{
    return l_latestAddedCombatant;
}

void CombatTracker::resetRounds()
{
    l_currentRound          = 1;
    l_currentCombatantIndex = getStartIndex();

    // Notify UI to change combatant details
    emit currentRoundChanged();

    if (l_combatants.size() > 0)
    {
        emit combatantNameChanged();
        emit combatantIniChanged();
        emit combatantHealthChanged();
        emit combatantStatusChanged();
    }
}

void CombatTracker::clearCombatants()
{
    l_combatants.clear();
    l_currentCombatantIndex = -1;
    l_currentRound          = 1;
}

// Switch to next combatant
void CombatTracker::nextCombatant()
{
    int next = getNextIndex();

    if ((l_currentCombatantIndex < l_combatants.size()) && (next > -1))
    {
        l_currentCombatantIndex = next;
    }
    else
    {
        l_currentCombatantIndex = getStartIndex();
        l_currentRound++;
        emit currentRoundChanged();
    }

    // Notify UI to change combatant details
    if (l_combatants.size() > 0)
    {
        emit combatantNameChanged();
        emit combatantIniChanged();
        emit combatantHealthChanged();
        emit combatantStatusChanged();
    }
}

// Returns the index of the next combatant
int CombatTracker::getNextIndex()
{
    int  oldIni         = (l_combatants.size() > 0) ? l_combatants.at(l_currentCombatantIndex).ini : -1;
    int  newIni         = -1;
    int  index          = -1;
    bool foundDoubleIni = false;

    for (int i = 0; i < l_combatants.size(); i++)
    {
        int tempIni = l_combatants.at(i).ini;

        if ((tempIni < oldIni) && (tempIni > newIni))
        {
            newIni = tempIni;
            index  = i;
        }
        else if ((tempIni == oldIni) && (l_currentCombatantIndex < i) && !foundDoubleIni)
        {
            foundDoubleIni = true;
            index          = i;
            newIni         = tempIni;
        }
    }

    return index;
}

// Returns index of highest initiative
int CombatTracker::getStartIndex()
{
    int index = 0;

    for (int i = 0; i < l_combatants.size(); i++)
    {
        if (l_combatants.at(i).ini > l_combatants.at(index).ini)
        {
            index = i;
        }
    }

    return index;
}

// Add a combatant to the list
void CombatTracker::addCombatant(QString name, int ini, int health)
{
    if ((name != NULL) && (ini > 0) && (health > 0))
    {
        Combatant c;

        c.name       = name;
        c.ini        = ini;
        c.health     = health;
        c.max_health = health;
        c.status     = tr("Alive");

        l_combatants.append(c);

        l_latestAddedCombatant.clear();
        l_latestAddedCombatant.append({ name, QString::number(ini), QString::number(health) + " / " + QString::number(health) });

        emit latestAddedCombatantChanged();

        l_currentCombatantIndex = getStartIndex();

        emit combatantNameChanged();
        emit combatantIniChanged();
        emit combatantHealthChanged();
        emit combatantStatusChanged();
    }
}

int CombatTracker::currentIndex()
{
    return l_currentCombatantIndex;
}

// Remove Combatant from list
void CombatTracker::removeCombatant(int index)
{
    if ((index > -1) && (l_combatants.size() > index))
    {
        l_combatants.removeAt(index);
    }
}

void CombatTracker::setCombatantIni(int ini)
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    c.ini = ini;

    l_combatants.replace(l_currentCombatantIndex, c);
}

void CombatTracker::setCombatantHealth(int health)
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    c.health = health;

    l_combatants.replace(l_currentCombatantIndex, c);
}

void CombatTracker::setCombatantMaxHealth(int health)
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    c.max_health = health;

    l_combatants.replace(l_currentCombatantIndex, c);
}

void CombatTracker::setCombatantStatus(QString status)
{
    Combatant c = l_combatants.at(l_currentCombatantIndex);

    c.status = status;

    l_combatants.replace(l_currentCombatantIndex, c);
}
