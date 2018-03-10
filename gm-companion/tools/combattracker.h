#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <QObject>
#include <QList>

struct Combatant
{
    QString name;
    int ini;
    int health;
    int max_health;
    QString status;
};

class CombatTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString combatantName READ combatantName NOTIFY combatantNameChanged)
    Q_PROPERTY(int combatantIni READ combatantIni NOTIFY combatantIniChanged)
    Q_PROPERTY(QString combatantHealth READ combatantHealth NOTIFY combatantHealthChanged)
    Q_PROPERTY(QString combatantStatus READ combatantStatus NOTIFY combatantStatusChanged)

public:
    explicit CombatTracker(QObject *parent = nullptr);

    QString combatantName();
    int combatantIni();
    QString combatantHealth();
    QString combatantStatus();

    Q_INVOKABLE void nextCombatant();
    Q_INVOKABLE void addCombatant(QString name, int ini, int health);

signals:
    void combatantNameChanged();
    void combatantIniChanged();
    void combatantHealthChanged();
    void combatantStatusChanged();
    void currentRoundChanged();

private:
    QList<Combatant> l_combatants;

    int l_currentCombatantIndex = 0;
    int l_currentRound = 0;

};

#endif // COMBATTRACKER_H
