#include "combattracker.h"

#include <QDebug>

CombatTracker::CombatTracker(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Combat Tracker ...";
}

void CombatTracker::resetRounds()
{
    l_currentRound = 1;
    l_currentIndex = getStartIndex();

    emit currentRoundChanged();
}

void CombatTracker::clear()
{
    l_combatants.clear();
    l_currentIndex = 0;
    l_currentRound = 1;

    emit combatantsChanged();
    emit currentRoundChanged();
}

// Switch to next combatant
void CombatTracker::next()
{
    int next = getNextIndex();

    if ((l_currentIndex < l_combatants.size()) && (next > -1))
    {
        l_currentIndex = next;
        emit currentIndexChanged();
    }
    else
    {
        l_currentIndex = getStartIndex();
        l_currentRound++;
        emit currentRoundChanged();
        emit currentIndexChanged();
    }
}

// Returns the index of the next combatant
int CombatTracker::getNextIndex()
{
    int  oldIni         = (l_combatants.size() > 0) ? l_combatants.at(l_currentIndex).ini : -1;
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
        else if ((tempIni == oldIni) && (l_currentIndex < i) && !foundDoubleIni)
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
void CombatTracker::add(QString name, int ini, int health)
{
    if ((name != NULL) && (ini > 0))
    {
        Combatant c;

        c.name   = name;
        c.ini    = ini;
        c.health = health;
        c.status = tr("Alive");
        c.notes  = "";

        l_combatants.append(c);
        emit combatantsChanged();
    }
}

// Remove Combatant from list
void CombatTracker::remove(int index)
{
    if ((index > -1) && (l_combatants.size() > index))
    {
        l_combatants.removeAt(index);
    }

    emit combatantsChanged();
}

void CombatTracker::sortByIni()
{
    QList<Combatant> sorted;

    while (!l_combatants.isEmpty())
    {
        int ini   = -1;
        int index = 0;

        for (int i = 0; i < l_combatants.size(); i++)
        {
            Combatant c = l_combatants.at(i);

            if (c.ini > ini)
            {
                index = i;
                ini   = c.ini;
            }
        }

        sorted.append(l_combatants.at(index));
        l_combatants.removeAt(index);
    }

    l_combatants = sorted;
    emit combatantsChanged();
}

void CombatTracker::setIni(int index, int ini)
{
    Combatant c = l_combatants.at(index);

    c.ini = ini;

    l_combatants.replace(index, c);
}

void CombatTracker::setHealth(int index, int health)
{
    Combatant c = l_combatants.at(index);

    c.health = health;

    l_combatants.replace(index, c);
}

void CombatTracker::setStatus(int index, QString status)
{
    Combatant c = l_combatants.at(index);

    c.status = status;

    l_combatants.replace(index, c);
}

void CombatTracker::setNotes(int index, QString notes)
{
    Combatant c = l_combatants.at(index);

    c.notes = notes;

    l_combatants.replace(index, c);
}
