#include "effect.h"
#include <QDebug>

Effect::Effect(QString name, int dice, int sides, int mod, QStringList effects, QString icon)
    : m_name(name), m_dice(dice), m_sides(sides), m_mod(mod), m_effects(effects), m_icon(icon)
{
    diceTool = new DiceTool;
    diceTool->setAmount(m_dice);
    diceTool->setSides(m_sides);
    diceTool->setModifier(m_mod);
}

Effect::~Effect()
{
    delete diceTool;
}

QString Effect::getEffect()
{
    int roll = diceTool->roll();

    roll = roll - m_dice;

    return m_effects[roll];
}
