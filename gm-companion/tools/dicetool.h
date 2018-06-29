#ifndef DICETOOL_H
#define DICETOOL_H

#include <QObject>
#include <QList>

class DiceTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)

public:
    explicit DiceTool(QObject *parent = nullptr);

    int sides();
    int roll();
    QString calculationString();

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count);
    Q_INVOKABLE void setAmount(int amount);
    Q_INVOKABLE void setModifier(int modifier);

signals:
    void sidesChanged();
    void rollChanged();
    void calculationStringChanged();

private:
    int l_sides = 20;
    int l_bonus_dice = 0;
    int l_modifier = 0;
    int l_amount = 1;

    QString l_calculation_string;
};

#endif // DICETOOL_H
