#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <QObject>
#include <QList>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "combatant.h"
#include "effecttool.h"

class CombatTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentRound READ currentRound NOTIFY currentRoundChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)

public:
    explicit CombatTracker(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    int currentRound() const { return m_currentRound; }
    int currentIndex() const { return m_currentIndex; }

    Q_INVOKABLE void next();
    Q_INVOKABLE bool add(const QString& name, int ini, int health, int priority, const QString &notes, bool sort = true);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void resetRounds();
    Q_INVOKABLE bool remove(int index);

    Q_INVOKABLE bool setIni(int index, int ini);
    Q_INVOKABLE bool modifyIni(int index, int steps = 1);

    Q_INVOKABLE bool setHealth(int index, int health);
    Q_INVOKABLE bool modifyHealth(int index, int steps = 1);

    Q_INVOKABLE bool setPriority(int index, int priority);
    Q_INVOKABLE bool modifyPriority(int index, int steps = 1);

    Q_INVOKABLE bool setNotes(int index, const QString &notes);
    Q_INVOKABLE void sortByIni(bool keepDelay = false);
    Q_INVOKABLE bool delayTurn(int index);

signals:
    void currentRoundChanged();
    void currentIndexChanged();
    void combatantsChanged();

protected:
    Combatant *getCombatant(int index);

private:
    CombatantListModel *combatantListModel = nullptr;
    EffectTool *effectTool = nullptr;
    QList<Combatant*> m_combatants;

    int m_currentIndex = 0;
    int m_currentRound = 1;
};

#endif // COMBATTRACKER_H
