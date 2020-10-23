#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QStringList>
#include "../dicetool.h"

class Effect : QObject
{
    Q_OBJECT

public:
    Effect(QString name, int dice, int sides, int mod,
           QStringList effects, QString icon, QObject *parent);

    QString name() const { return m_name; }
    int dice() const { return m_dice; }
    int sides() const { return m_sides; }
    int mod() const { return m_mod; }
    QString icon() const { return m_icon; }

    QString getEffect() const;

private:
    QString m_name;
    int m_dice, m_sides, m_mod;
    QStringList m_effects;
    QString m_icon;

    DiceTool *diceTool = nullptr;
};

#endif // EFFECT_H
