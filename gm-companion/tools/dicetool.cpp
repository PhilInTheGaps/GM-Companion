#include "dicetool.h"

#include <QDebug>
#include <QDir>

DiceTool::DiceTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Dice Tool ...";

    settings = new QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
    settings->beginGroup("Dice");
}

void DiceTool::setSides(int sides)
{
    l_sides = sides;
    emit sidesChanged();
}

void DiceTool::setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax)
{
    settings->setValue("enableCriticals", enableCriticals);
    settings->setValue("success",         success);
    settings->setValue("failure",         failure);
    settings->setValue("useMinMax",       minMax);
    settings->setValue("successMax",      successMax);
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
            criticalSuccess = l_sides;
            criticalFailure = 1;
        }
        else
        {
            criticalSuccess = 1;
            criticalFailure = l_sides;
        }
    }
    else
    {
        criticalSuccess = getSuccess();
        criticalFailure = getFailure();
    }


    l_calculation_string = tr("Roll:\n") + QString::number(l_amount) + "x " + tr("D") + QString::number(l_sides);

    if (l_modifier < 0)
    {
        l_calculation_string.append(" " + QString::number(l_modifier) + "\n\n");
    }
    else
    {
        l_calculation_string.append(" + " + QString::number(l_modifier) + "\n\n");
    }

    for (int i = 0; i < l_amount; i++)
    {
        // Generate random integer
        int temp = rand() % l_sides + 1;

        // Check for critical successes or failures
        if (temp == criticalSuccess) criticalSuccesses++;
        else if (temp == criticalFailure) criticalFailures++;


        // Add the temporary result to the overall result
        result += temp;

        // Add roll to calculation string
        l_calculation_string.append(tr("Roll ") + QString::number(i + 1) + ":\t" + QString::number(temp) + "\n");
    }

    l_calculation_string.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");

    // Remove bonus dice from calculation
    if (l_bonus_dice > 0)
    {
        l_calculation_string.append(tr("Bonus Dice:\n"));

        // TODO for later, is currently not important

        l_calculation_string.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");
    }

    // Add modifier
    result += l_modifier;
    l_calculation_string.append(tr("Modifier: ") + QString::number(l_modifier) + "\n\n" + tr("Result: ") + QString::number(result));

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

    return result;
}
