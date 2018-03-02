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
};

class CombatTracker : public QObject
{
    Q_OBJECT

public:
    explicit CombatTracker(QObject *parent = nullptr);

signals:

private:
    QList<Combatant> l_combatants;

};

#endif // COMBATTRACKER_H
