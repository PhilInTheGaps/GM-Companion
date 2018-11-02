#include "combattracker.h"

#include <QDebug>

CombatTracker::CombatTracker(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Combat Tracker ...";
}

void CombatTracker::resetRounds()
{
    m_currentRound = 1;
    m_currentIndex = getStartIndex();

    emit currentRoundChanged();
}

void CombatTracker::clear()
{
    m_combatants.clear();
    m_currentIndex = 0;
    m_currentRound = 1;

    emit combatantsChanged();
    emit currentRoundChanged();
}

// Switch to next combatant
void CombatTracker::next()
{
    int next = getNextIndex();

    if ((m_currentIndex < m_combatants.size()) && (next > -1))
    {
        m_currentIndex = next;
        emit currentIndexChanged();
    }
    else
    {
        m_currentIndex = getStartIndex();
        m_currentRound++;
        emit currentRoundChanged();
        emit currentIndexChanged();
    }
}

// Returns the index of the next combatant
int CombatTracker::getNextIndex()
{
    int  oldIni         = (m_combatants.size() > 0) ? m_combatants.at(m_currentIndex).ini : -1;
    int  newIni         = -1;
    int  index          = -1;
    bool foundDoubleIni = false;

    for (int i = 0; i < m_combatants.size(); i++)
    {
        int tempIni = m_combatants.at(i).ini;

        if ((tempIni < oldIni) && (tempIni > newIni))
        {
            newIni = tempIni;
            index  = i;
        }
        else if ((tempIni == oldIni) && (m_currentIndex < i) && !foundDoubleIni)
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

    for (int i = 0; i < m_combatants.size(); i++)
    {
        if (m_combatants.at(i).ini > m_combatants.at(index).ini)
        {
            index = i;
        }
    }

    return index;
}

// Add a combatant to the list
void CombatTracker::add(QString name, int ini, int health, bool sort)
{
    if ((name != NULL) && (ini > 0))
    {
        Combatant c;

        c.name   = name;
        c.ini    = ini;
        c.health = health;
        c.status = tr("Alive");
        c.notes  = "";

        m_combatants.append(c);

        if (!sort) emit combatantsChanged();
    }

    if (sort) sortByIni();
}

// Remove Combatant from list
void CombatTracker::remove(int index)
{
    if ((index > -1) && (m_combatants.size() > index))
    {
        m_combatants.removeAt(index);

        if ((m_currentIndex == index) && (m_currentIndex > 0))
        {
            m_currentIndex--;
            emit currentIndexChanged();
        }
    }

    emit combatantsChanged();
}

void CombatTracker::sortByIni()
{
    QList<Combatant> sorted;

    while (!m_combatants.isEmpty())
    {
        int ini   = -1;
        int index = 0;

        for (int i = 0; i < m_combatants.size(); i++)
        {
            Combatant c = m_combatants.at(i);

            if (c.ini > ini)
            {
                index = i;
                ini   = c.ini;
            }
        }

        sorted.append(m_combatants.at(index));
        m_combatants.removeAt(index);
    }

    m_combatants = sorted;
    emit combatantsChanged();
}

void CombatTracker::setIni(int index, int ini)
{
    Combatant c = m_combatants.at(index);

    c.ini = ini;

    m_combatants.replace(index, c);
}

void CombatTracker::setHealth(int index, int health)
{
    Combatant c = m_combatants.at(index);

    c.health = health;

    m_combatants.replace(index, c);
}

void CombatTracker::setStatus(int index, QString status)
{
    Combatant c = m_combatants.at(index);

    c.status = status;

    m_combatants.replace(index, c);
}

void CombatTracker::setNotes(int index, QString notes)
{
    Combatant c = m_combatants.at(index);

    c.notes = notes;

    m_combatants.replace(index, c);
}
