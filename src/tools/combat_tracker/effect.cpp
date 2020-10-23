#include "effect.h"

#include <utility>

Effect::Effect(QString name, int dice, int sides, int mod,
               QStringList effects, QString icon, QObject *parent)
    : QObject(parent), m_name(std::move(name)), m_dice(dice), m_sides(sides),
      m_mod(mod), m_effects(std::move(effects)), m_icon(std::move(icon))
{
    diceTool = new DiceTool(this);
    diceTool->setAmount(m_dice);
    diceTool->setSides(m_sides);
    diceTool->setModifier(m_mod);
}

auto Effect::getEffect() const -> QString
{
    int roll = diceTool->roll() - m_dice;
    return QString("%1: %2\n%3").arg(tr("Dice Result"), QString::number(roll + 2), m_effects[roll]);
}
