#include "dicetool.h"
#include "logging.h"
#include <QQmlContext>

DiceTool::DiceTool(QQmlEngine *engine, QObject *parent) : QObject(parent)
{
    engine->rootContext()->setContextProperty("dice_tool", this);
}

void DiceTool::setSides(int sides)
{
    m_sides = sides;
    emit sidesChanged();
}

void DiceTool::setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax)
{
    SettingsManager::setSetting(ENABLE_CRITICALS_SETTING, enableCriticals, DICE_SETTINGS);
    SettingsManager::setSetting(SUCCESS_SETTING,          success,         DICE_SETTINGS);
    SettingsManager::setSetting(FAILURE_SETTING,          failure,         DICE_SETTINGS);
    SettingsManager::setSetting(USE_MIN_MAX_SETTING,      minMax,          DICE_SETTINGS);
    SettingsManager::setSetting(SUCCESS_MAX_SETTING,      successMax,      DICE_SETTINGS);
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

    m_calculation_string = tr("Roll:\n") + QString::number(m_amount) + "x " + tr("D") + QString::number(m_sides);

    qCDebug(gmDiceTool()) << m_calculation_string;

    if (m_modifier < 0)
    {
        m_calculation_string.append(" " + QString::number(m_modifier) + "\n\n");
    }
    else
    {
        m_calculation_string.append(" + " + QString::number(m_modifier) + "\n\n");
    }

    for (int i = 0; i < m_amount; i++)
    {
        // Generate random integer
        int temp = rand() % m_sides + 1;

        // Check for critical successes or failures
        if (temp == criticalSuccess) criticalSuccesses++;
        else if (temp == criticalFailure) criticalFailures++;


        // Add the temporary result to the overall result
        result += temp;

        // Add roll to calculation string
        m_calculation_string.append(tr("Roll ") + QString::number(i + 1) + ":\t" + QString::number(temp) + "\n");
    }

    m_calculation_string.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");

    // Remove bonus dice from calculation
    if (m_bonus_dice > 0)
    {
        m_calculation_string.append(tr("Bonus Dice:\n"));

        // TODO for later, is currently not important

        m_calculation_string.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");
    }

    // Add modifier
    result += m_modifier;
    m_calculation_string.append(tr("Modifier: ") + QString::number(m_modifier) + "\n\n" + tr("Result: ") + QString::number(result));

    emit rollChanged();
    emit calculationStringChanged();

    // Evaluate Crits
    if (getCriticalEnabled())
    {
        if ((criticalSuccesses > 0) && (criticalFailures > 0)) emit mixedCriticalResult();
        else if (criticalSuccesses > 0) emit                        successfulCriticalResult();
        else if (criticalFailures > 0) emit                         failedCriticalResult();
        else emit                                                   normalResult();
    }
    else
    {
        emit normalResult();
    }

    qCDebug(gmDiceTool()) << "Result:" << result;

    return result;
}
