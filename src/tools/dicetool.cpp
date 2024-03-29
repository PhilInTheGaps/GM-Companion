#include "dicetool.h"
#include "settings/settingsmanager.h"
#include "utils/stringutils.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

constexpr ConstQString DICE_SETTINGS = "Dice";
constexpr ConstQString ENABLE_CRITICALS_SETTING = "enableCriticals";
constexpr ConstQString SUCCESS_SETTING = "success";
constexpr ConstQString FAILURE_SETTING = "failure";
constexpr ConstQString USE_MIN_MAX_SETTING = "useMinMax";
constexpr ConstQString SUCCESS_MAX_SETTING = "successMax";

Q_LOGGING_CATEGORY(gmDiceTool, "gm.dice.tool")

auto DiceTool::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> DiceTool *
{
    Q_UNUSED(jsEngine)
    return new DiceTool(qmlEngine);
}

void DiceTool::setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax)
{
    SettingsManager::instance()->set(ENABLE_CRITICALS_SETTING, enableCriticals, DICE_SETTINGS);
    SettingsManager::instance()->set(SUCCESS_SETTING, success, DICE_SETTINGS);
    SettingsManager::instance()->set(FAILURE_SETTING, failure, DICE_SETTINGS);
    SettingsManager::instance()->set(USE_MIN_MAX_SETTING, minMax, DICE_SETTINGS);
    SettingsManager::instance()->set(SUCCESS_MAX_SETTING, successMax, DICE_SETTINGS);
}

auto DiceTool::getCriticalEnabled() -> bool
{
    return SettingsManager::instance()->get(ENABLE_CRITICALS_SETTING, true, DICE_SETTINGS);
}

auto DiceTool::getSuccess() -> int
{
    return SettingsManager::instance()->get(SUCCESS_SETTING, 20, DICE_SETTINGS);
}

auto DiceTool::getFailure() -> int
{
    return SettingsManager::instance()->get(FAILURE_SETTING, 1, DICE_SETTINGS);
}

auto DiceTool::getMinMax() -> int
{
    return SettingsManager::instance()->get(USE_MIN_MAX_SETTING, true, DICE_SETTINGS);
}

auto DiceTool::getSuccessMax() -> int
{
    return SettingsManager::instance()->get(SUCCESS_MAX_SETTING, true, DICE_SETTINGS);
}

void DiceTool::roll()
{
    int value = 0;
    int criticalSuccesses = 0;
    int criticalFailures = 0;
    int criticalSuccess = 0;
    int criticalFailure = 0;

    // Initialize values for crits
    if (getMinMax())
    {
        if (getSuccessMax())
        {
            criticalSuccess = a_sides;
            criticalFailure = 1;
        }
        else
        {
            criticalSuccess = 1;
            criticalFailure = a_sides;
        }
    }
    else
    {
        criticalSuccess = getSuccess();
        criticalFailure = getFailure();
    }

    a_calculation = tr("Roll:\n") + QString::number(m_amount) + "x " + tr("D") + QString::number(a_sides);
    a_calculation.append((m_modifier < 0 ? " " : " + ") + QString::number(m_modifier) + "\n\n");

    for (int i = 0; i < m_amount; i++)
    {
        // Generate random integer
        int temp = QRandomGenerator::system()->bounded(a_sides) + 1;

        // Check for critical successes or failures
        if (temp == criticalSuccess) criticalSuccesses++;
        else if (temp == criticalFailure)
            criticalFailures++;

        // Add the temporary result to the overall result
        value += temp;

        // Add roll to calculation string
        a_calculation.append(tr("Roll ") + QString::number(i + 1) + ":\t" + QString::number(temp) + "\n");
    }

    a_calculation.append(tr("\nTemporary Result: ") + QString::number(value) + "\n");

    // Remove bonus dice from calculation
    if (m_bonusDice > 0)
    {
        a_calculation.append(tr("Bonus Dice:\n"));

        // TODO for later, is currently not important

        a_calculation.append(tr("\nTemporary Result: ") + QString::number(value) + "\n");
    }

    // Add modifier
    value += m_modifier;
    a_calculation.append(tr("Modifier: ") + QString::number(m_modifier) + "\n" + tr("Result: ") +
                         QString::number(value));

    result(QString::number(value));
    emit calculationChanged(a_calculation);
    qCDebug(gmDiceTool()) << "Result:" << result();

    // Evaluate Crits
    if (getCriticalEnabled())
    {
        if ((criticalSuccesses > 0) && (criticalFailures > 0)) emit mixedCriticalResult();
        else if (criticalSuccesses > 0)
            emit successfulCriticalResult();
        else if (criticalFailures > 0)
            emit failedCriticalResult();
        else
            emit normalResult();
    }
    else
    {
        emit normalResult();
    }
}
