#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <QObject>
#include <QList>

struct Combatant
{
    QString name;
    int ini;
    int health;
    QString status;
    QString notes;
};

class CombatTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentRound READ currentRound NOTIFY currentRoundChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)

public:
    explicit CombatTracker(QObject *parent = nullptr);

    int currentRound() { return l_currentRound; }
    int currentIndex() { return l_currentIndex; }

    Q_INVOKABLE void next();
    Q_INVOKABLE void add(QString name, int ini, int health);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void resetRounds();
    Q_INVOKABLE void remove(int index);

    Q_INVOKABLE int getListSize() { return l_combatants.size(); }

    Q_INVOKABLE void setIni(int index, int ini);
    Q_INVOKABLE void setHealth(int index, int health);
    Q_INVOKABLE void setStatus(int index, QString status);
    Q_INVOKABLE void setNotes(int index, QString notes);

    Q_INVOKABLE QString getName(int index) { return l_combatants.at(index).name; }
    Q_INVOKABLE int getIni(int index) { return l_combatants.at(index).ini; }
    Q_INVOKABLE int getHealth(int index) { return l_combatants.at(index).health; }
    Q_INVOKABLE QString getStatus(int index) { return l_combatants.at(index).status; }
    Q_INVOKABLE QString getNotes(int index) { return l_combatants.at(index).notes; }

signals:
    void currentRoundChanged();
    void currentIndexChanged();
    void combatantsChanged();

private:
    QList<Combatant> l_combatants;

    int getNextIndex();
    int getStartIndex();

    int l_currentIndex = 0;
    int l_currentRound = 1;

};

#endif // COMBATTRACKER_H
