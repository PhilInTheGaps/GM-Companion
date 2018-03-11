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
    Q_PROPERTY(int combatantHealth READ combatantHealth NOTIFY combatantHealthChanged)
    Q_PROPERTY(int combatantMaxHealth READ combatantMaxHealth)
    Q_PROPERTY(QString combatantStatus READ combatantStatus NOTIFY combatantStatusChanged)
    Q_PROPERTY(QStringList latestAddedCombatant READ latestAddedCombatant NOTIFY latestAddedCombatantChanged)
    Q_PROPERTY(int currentRound READ currentRound NOTIFY currentRoundChanged)
    Q_PROPERTY(int currentIndex READ currentIndex)

public:
    explicit CombatTracker(QObject *parent = nullptr);

    QString combatantName();
    int combatantIni();
    int combatantHealth();
    int combatantMaxHealth();
    QString combatantStatus();

    QStringList latestAddedCombatant();
    int currentRound();
    int currentIndex();

    Q_INVOKABLE void nextCombatant();
    Q_INVOKABLE void addCombatant(QString name, int ini, int health);
    Q_INVOKABLE void clearCombatants();
    Q_INVOKABLE void resetRounds();
    Q_INVOKABLE void removeCombatant(int index);

    Q_INVOKABLE void setCombatantIni(int ini);
    Q_INVOKABLE void setCombatantHealth(int health);
    Q_INVOKABLE void setCombatantMaxHealth(int health);
    Q_INVOKABLE void setCombatantStatus(QString status);

signals:
    void combatantNameChanged();
    void combatantIniChanged();
    void combatantHealthChanged();
    void combatantStatusChanged();
    void currentRoundChanged();
    void latestAddedCombatantChanged();

private:
    QList<Combatant> l_combatants;

    QStringList l_latestAddedCombatant;

    int getNextIndex();
    int getStartIndex();

    int l_currentCombatantIndex = -1;
    int l_currentRound = 1;

};

#endif // COMBATTRACKER_H
