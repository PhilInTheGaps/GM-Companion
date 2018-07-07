#ifndef DICETOOL_H
#define DICETOOL_H

#include <QObject>
#include <QSettings>
#include <QList>

class DiceTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)

public:
    explicit DiceTool(QObject *parent = nullptr);

    int sides() const { return l_sides; }
    int roll();
    QString calculationString() const { return l_calculation_string; }

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count) { l_bonus_dice = count; }
    Q_INVOKABLE void setAmount(int amount) { l_amount = amount; }
    Q_INVOKABLE void setModifier(int modifier) { l_modifier = modifier; }

    Q_INVOKABLE void setDiceSettings(bool enableCriticals, int success, int failure);
    Q_INVOKABLE bool getCriticalEnabled() const { return settings->value("enableCriticals", true).toBool(); }
    Q_INVOKABLE int getSuccess() const { return settings->value("success", 20).toInt(); }
    Q_INVOKABLE int getFailure() const { return settings->value("failure", 1).toInt(); }

signals:
    void sidesChanged();
    void rollChanged();
    void calculationStringChanged();

    void mixedCriticalResult();
    void successfulCriticalResult();
    void failedCriticalResult();
    void normalResult();

private:
    int l_sides = 20;
    int l_bonus_dice = 0;
    int l_modifier = 0;
    int l_amount = 1;

    QSettings *settings;

    QString l_calculation_string;
};

#endif // DICETOOL_H
