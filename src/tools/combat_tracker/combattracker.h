#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <QObject>
#include <QList>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "combatant.h"
#include "combattrackerstate.h"
#include "effecttool.h"
#include "abstracttool.h"

class CombatTracker : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(int currentRound READ currentRound NOTIFY currentRoundChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)

public:
    explicit CombatTracker(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    [[nodiscard]] int currentRound() const { return m_state.currentRound(); }
    [[nodiscard]] int currentIndex() const { return m_state.currentIndex(); }

    Q_INVOKABLE void next();
    Q_INVOKABLE bool add(const QString& name, int ini, int health, int priority, const QString &notes, bool sort = true);
    Q_INVOKABLE void clear(bool saveAfterClear = true);
    Q_INVOKABLE void reset();
    Q_INVOKABLE bool remove(int index);

    bool setIni(Combatant *combatant, int ini);
    Q_INVOKABLE bool setIni(int index, int ini);
    Q_INVOKABLE bool modifyIni(int index, int steps = 1);

    Q_INVOKABLE bool setHealth(int index, int health);
    Q_INVOKABLE bool modifyHealth(int index, int steps = 1);

    bool setPriority(Combatant *combatant, int priority);
    Q_INVOKABLE bool setPriority(int index, int priority);
    Q_INVOKABLE bool modifyPriority(int index, int steps = 1);

    Q_INVOKABLE bool setNotes(int index, const QString &notes);
    Q_INVOKABLE void sortByIni(bool keepDelay = false);
    Q_INVOKABLE bool delayTurn(int index);

public slots:
    void loadData() override;

signals:
    void currentRoundChanged();
    void currentIndexChanged();

protected:
    QList<Combatant*> combatants() const;
    Combatant *getCombatant(int index);

    void resetDelayForAll();
    void saveToDisk();

    static QFile getCacheFile();

private:
    EffectTool *effectTool = nullptr;

    CombatTrackerState m_state;
};

#endif // COMBATTRACKER_H
