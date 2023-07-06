#include "dicetool.h"
#include "settings/settingsmanager.h"
#include "utils/stringutils.h"
#include <QLoggingCategory>
#include <QQmlContext>

using namespace Qt::Literals::StringLiterals;

constexpr ConstQString DICE_SETTINGS = "Dice";
constexpr ConstQString ENABLE_CRITICALS_SETTING = "enableCriticals";
constexpr ConstQString SUCCESS_SETTING = "success";
constexpr ConstQString FAILURE_SETTING = "failure";
constexpr ConstQString USE_MIN_MAX_SETTING = "useMinMax";
constexpr ConstQString SUCCESS_MAX_SETTING = "successMax";

Q_LOGGING_CATEGORY(gmDiceTool, "gm.dice.tool")

DiceTool::DiceTool(QQmlEngine *engine, QObject *parent) : QObject(parent)
{
    engine->rootContext()->setContextProperty(u"dice_tool"_s, this);
}

void DiceTool::setSides(int sides)
{
    m_sides = sides;
    emit sidesChanged();
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
    return SettingsManager::instance()->get(USE_MIN_MAX_SETTING, false, DICE_SETTINGS);
}

auto DiceTool::getSuccessMax() -> int
{
    return SettingsManager::instance()->get(SUCCESS_MAX_SETTING, true, DICE_SETTINGS);
}

int DiceTool::roll()
{
    int result = 0;
    int criticalSuccesses = 0, criticalFailures = 0;
    int criticalSuccess = 0, criticalFailure = 0;

    // Initialize values for crits
    if (getMinMax())
    {
        if (getSuccessMax())
        {
            criticalSuccess = m_sides;
            criticalFailure = 1;
        }
        else
        {
            criticalSuccess = 1;
            criticalFailure = m_sides;
        }
    }
    else
    {
        criticalSuccess = getSuccess();
        criticalFailure = getFailure();
    }

    m_calculationString = tr("Roll:\n") + QString::number(m_amount) + "x " + tr("D") + QString::number(m_sides);

    qCDebug(gmDiceTool()) << m_calculationString;

    if (m_modifier < 0)
    {
        m_calculationString.append(" " + QString::number(m_modifier) + "\n\n");
    }
    else
    {
        m_calculationString.append(" + " + QString::number(m_modifier) + "\n\n");
    }

    for (int i = 0; i < m_amount; i++)
    {
        // Generate random integer
        int temp = rand() % m_sides + 1;

        // Check for critical successes or failures
        if (temp == criticalSuccess) criticalSuccesses++;
        else if (temp == criticalFailure)
            criticalFailures++;

        // Add the temporary result to the overall result
        result += temp;

        // Add roll to calculation string
        m_calculationString.append(tr("Roll ") + QString::number(i + 1) + ":\t" + QString::number(temp) + "\n");
    }

    m_calculationString.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");

    // Remove bonus dice from calculation
    if (m_bonusDice > 0)
    {
        m_calculationString.append(tr("Bonus Dice:\n"));

        // TODO for later, is currently not important

        m_calculationString.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");
    }

    // Add modifier
    result += m_modifier;
    m_calculationString.append(tr("Modifier: ") + QString::number(m_modifier) + "\n\n" + tr("Result: ") +
                               QString::number(result));

    emit rollChanged();
    emit calculationStringChanged();

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

    qCDebug(gmDiceTool()) << "Result:" << result;

    return result;
}
