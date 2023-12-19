#pragma once

#include "abstracttool.h"
#include "combatant.h"
#include "combattrackerstate.h"
#include "filesystem/results/fileresult.h"
#include <QFile>
#include <QList>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class CombatTracker : public AbstractTool
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CombatTrackerTool)
    QML_SINGLETON

public:
    CombatTracker() = delete;
    explicit CombatTracker(QObject *parent = nullptr);
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> CombatTracker *;

    Q_PROPERTY(int currentRound READ currentRound NOTIFY currentRoundChanged)
    [[nodiscard]] auto currentRound() const -> int
    {
        return m_state.currentRound();
    }

    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    [[nodiscard]] auto currentIndex() const -> int
    {
        return m_state.currentIndex();
    }

    Q_PROPERTY(CombatantListModel *model READ model CONSTANT)
    [[nodiscard]] auto model() -> CombatantListModel *
    {
        return &m_state.model();
    }

    Q_INVOKABLE void next();
    Q_INVOKABLE bool add(const QString &name, int ini, int health, int priority, const QString &notes,
                         bool sort = true);
    Q_INVOKABLE void clear(bool saveAfterClear = true);
    Q_INVOKABLE void reset();
    Q_INVOKABLE bool remove(int index);

    auto setIni(Combatant *combatant, int ini) -> bool;
    Q_INVOKABLE bool setIni(int index, int ini);
    Q_INVOKABLE bool modifyIni(int index, int steps = 1);

    Q_INVOKABLE bool setHealth(int index, int health);
    Q_INVOKABLE bool modifyHealth(int index, int steps = 1);

    auto setPriority(Combatant *combatant, int priority) -> bool;
    Q_INVOKABLE bool setPriority(int index, int priority);
    Q_INVOKABLE bool modifyPriority(int index, int steps = 1);

    Q_INVOKABLE bool setNotes(int index, const QString &notes);
    Q_INVOKABLE void sortByIni(bool keepDelay = false);
    Q_INVOKABLE bool delayTurn(int index);

    Q_INVOKABLE QFuture<void> loadFile(const QString &file);
    Q_INVOKABLE QFuture<Files::FileResult> saveFile(const QString &file);

public slots:
    void loadData() override;

signals:
    void currentRoundChanged();
    void currentIndexChanged();

protected:
    [[nodiscard]] auto combatants() const -> QList<Combatant *>;
    [[nodiscard]] auto getCombatant(int index) -> Combatant *;

    void resetDelayForAll() const;
    void saveToTempFile() const;

    static auto getCacheFile() -> QFile;

private:
    CombatTrackerState m_state;
};
