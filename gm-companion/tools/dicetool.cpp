#include "dicetool.h"

#include <QDebug>

DiceTool::DiceTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Dice Tool ...";
}

int DiceTool::sides()
{
    return l_sides;
}

void DiceTool::setSides(int sides)
{
    l_sides = sides;
    emit sidesChanged();
}

void DiceTool::setAmount(int amount)
{
    l_amount = amount;
}

void DiceTool::setBonusDice(int count)
{
    l_bonus_dice = count;
}

void DiceTool::setModifier(int modifier)
{
    l_modifier = modifier;
}

QString DiceTool::calculationString()
{
    return l_calculation_string;
}

int DiceTool::roll()
{
    int result = 0;

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

        // TODO

        l_calculation_string.append(tr("\nTemporary Result: ") + QString::number(result) + "\n\n");
    }

    // Add modifier
    result += l_modifier;
    l_calculation_string.append(tr("Modifier: ") + QString::number(l_modifier) + "\n\n" + tr("Result: ") + QString::number(result));

    emit rollChanged();
    emit calculationStringChanged();

    return result;
}
