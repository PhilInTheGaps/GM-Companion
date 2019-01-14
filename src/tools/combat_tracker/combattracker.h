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

    EffectTool* getEffectTool() const { return effectTool; }

    int currentRound() const { return m_currentRound; }
    int currentIndex() const { return m_currentIndex; }

    Q_INVOKABLE void next();
    Q_INVOKABLE void add(QString name, int ini, int health, bool sort = true);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void resetRounds();
    Q_INVOKABLE void remove(int index);

    Q_INVOKABLE void setIni(int index, int ini);
    Q_INVOKABLE void setHealth(int index, int health);
    Q_INVOKABLE void setNotes(int index, QString notes);
    void sortByIni();

signals:
    void currentRoundChanged();
    void currentIndexChanged();
    void combatantsChanged();

private:
    QQmlApplicationEngine *qmlEngine;
    CombatantListModel *combatantListModel;
    EffectTool *effectTool;
    QList<Combatant*> m_combatants;

    int m_currentIndex = 0;
    int m_currentRound = 0;

};

#endif // COMBATTRACKER_H
