#include "combattracker.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <algorithm>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

Q_LOGGING_CATEGORY(gmCombatTracker, "gm.combat.tracker")

CombatTracker::CombatTracker(QObject *parent) : AbstractTool(parent)
{
    connect(&m_state, &CombatTrackerState::currentIndexChanged, this, &CombatTracker::currentIndexChanged);
    connect(&m_state, &CombatTrackerState::currentRoundChanged, this, &CombatTracker::currentRoundChanged);
}

auto CombatTracker::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> CombatTracker *
{
    Q_UNUSED(jsEngine)
    return new CombatTracker(qmlEngine);
}

/**
 * @brief Reset the rounds and the current index
 */
void CombatTracker::reset()
{
    sortByIni(false);
    m_state.reset();
    saveToTempFile();
}

/**
 * @brief Delete all combatants, reset index and round
 */
void CombatTracker::clear(bool saveAfterClear)
{
    m_state.clear();

    if (saveAfterClear)
    {
        saveToTempFile();
    }
}

/**
 * @brief Switch to next combatant
 */
void CombatTracker::next()
{
    if (m_state.currentIndex() + 1 < combatants().size())
    {
        m_state.moveToNextCombatant();
    }
    else
    {
        sortByIni();
        m_state.startNewRound();
    }

    qCDebug(gmCombatTracker()) << "New index:" << currentIndex();
    saveToTempFile();
}

/**
 * @brief Add a combatant to the list
 * @param name Name of the combatant
 * @param ini Initiative of combatant
 * @param health Health of combatant
 * @param sort If combatant list should be sorted after adding
 */
auto CombatTracker::add(const QString &name, int ini, int health, int priority, const QString &notes, bool sort) -> bool
{
    if (!name.isEmpty() && (ini > -1))
    {
        auto *combatant = new Combatant(name, notes, ini, health, priority, this);
        m_state.addCombatant(combatant);

        if ((currentRound() == 1) && sort)
        {
            sortByIni();
            qCDebug(gmCombatTracker()) << "Setting currentIndex to 0!";
            m_state.currentIndex(0);
        }

        saveToTempFile();
        return true;
    }

    if (sort)
    {
        sortByIni();
    }

    saveToTempFile();
    return false;
}

/**
 * @brief Remove combatant from list
 * @param index Index of the combatant
 */
auto CombatTracker::remove(int index) -> bool
{
    if (Q_UNLIKELY(!Utils::isInBounds(combatants(), index))) return false;

    m_state.removeCombatant(index);
    saveToTempFile();
    return true;
}

/**
 * @brief Sort the list of combatants by initiative
 */
void CombatTracker::sortByIni(bool keepDelay)
{
    qCDebug(gmCombatTracker()) << "Sorting combatants ...";

    if (!keepDelay) resetDelayForAll();

    auto *combatant = getCombatant(currentIndex());

    m_state.sortCombatants(true);

    if (combatant)
    {
        m_state.currentIndex(static_cast<int>(combatants().indexOf(combatant)));
    }
}

auto CombatTracker::setIni(Combatant *combatant, int ini) -> bool
{
    qCDebug(gmCombatTracker()) << "Setting INI ...";
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->ini(ini);
    sortByIni(true);

    saveToTempFile();
    return true;
}

/**
 * @brief Set the initiative of combatant
 * @param index Index of combatant
 * @param ini New initiative of combatant
 */
auto CombatTracker::setIni(int index, int ini) -> bool
{
    return setIni(getCombatant(index), ini);
}

/**
 * @brief Increase or decrease INI at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
auto CombatTracker::modifyIni(int index, int steps) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    auto ini = combatant->ini();
    return setIni(combatant, ini + steps);
}

/**
 * @brief Set the health of combatant
 * @param index Index of combatant
 * @param ini New health of combatant
 */
auto CombatTracker::setHealth(int index, int health) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->health(health);
    saveToTempFile();
    return true;
}

/**
 * @brief Increase or decrease health at index
 * @param index Index of Combatant
 * @param steps Steps to increase INI by. Default: +1
 */
auto CombatTracker::modifyHealth(int index, int steps) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    auto health = combatant->health();
    health += steps;
    combatant->health(health);
    saveToTempFile();
    return true;
}

auto CombatTracker::setPriority(Combatant *combatant, int priority) -> bool
{
    if (Q_UNLIKELY(!combatant)) return false;

    qCDebug(gmCombatTracker()) << "Setting priority of" << QString(*combatant) << "to" << priority << "...";

    combatant->priority(priority);
    sortByIni(true);
    saveToTempFile();
    return true;
}

auto CombatTracker::setPriority(int index, int priority) -> bool
{
    return setPriority(getCombatant(index), priority);
}

auto CombatTracker::modifyPriority(int index, int steps) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    qCDebug(gmCombatTracker()) << "Modifying priority of" << QString(*combatant) << "by" << steps << "...";

    auto priority = combatant->priority();
    return setPriority(combatant, priority + steps);
}

/**
 * @brief Set the notes on combatant
 * @param index Index of combatant
 * @param ini New notes on combatant
 */
auto CombatTracker::setNotes(int index, const QString &notes) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->notes(notes);
    saveToTempFile();
    return true;
}

auto CombatTracker::delayTurn(int index) -> bool
{
    auto *combatant = getCombatant(index);
    if (Q_UNLIKELY(!combatant)) return false;

    combatant->delay(true);

    m_state.moveCombatantToBack(index);
    saveToTempFile();
    return true;
}

auto CombatTracker::combatants() const -> QList<Combatant *>
{
    return m_state.combatants();
}

auto CombatTracker::getCombatant(int index) -> Combatant *
{
    if (Q_LIKELY(Utils::isInBounds(combatants(), index)))
    {
        return combatants().at(index);
    }

    return nullptr;
}

void CombatTracker::resetDelayForAll() const
{
    foreach (auto *combatant, combatants())
    {
        if (Q_LIKELY(combatant)) combatant->delay(false);
    }

    saveToTempFile();
}

void CombatTracker::loadData()
{
    if (isDataLoaded()) return;

    if (auto tempFile = getCacheFile(); tempFile.exists() && tempFile.open(QIODevice::ReadOnly))
    {
        const auto data = tempFile.readAll();
        tempFile.close();

        const auto json = QJsonDocument::fromJson(data);
        m_state.load(json);
    }

    setIsDataLoaded(true);
}

auto CombatTracker::loadFile(const QString &file) -> QFuture<void>
{
    return File::getDataAsync(file).then([this](const FileDataResult &result) {
        if (!result.success())
        {
            qCWarning(gmCombatTracker()) << "Could not load file:" << result.errorMessage();
            return;
        }

        const auto json = QJsonDocument::fromJson(result.data());
        m_state.load(json);

        setIsDataLoaded(true);
    });
}

auto CombatTracker::saveFile(const QString &file) -> QFuture<FileResult>
{
    const auto data = m_state.serialize().toJson();
    return File::saveAsync(file, data);
}

void CombatTracker::saveToTempFile() const
{
    auto json = m_state.serialize();
    auto tempFile = getCacheFile();

    if (tempFile.open(QIODevice::WriteOnly))
    {
        tempFile.write(json.toJson());
        tempFile.close();
    }
}

auto CombatTracker::getCacheFile() -> QFile
{
    const auto filePath = FileUtils::fileInDir(u"combat-tracker-state.json"_s, QDir::tempPath());

    return {filePath};
}
