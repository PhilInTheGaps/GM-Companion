#ifndef DICETOOL_H
#define DICETOOL_H

#include <QObject>
#include <QQmlEngine>
#include "settings/settingsmanager.h"

#define DICE_SETTINGS "Dice"
#define ENABLE_CRITICALS_SETTING "enableCriticals"
#define SUCCESS_SETTING "success"
#define FAILURE_SETTING "failure"
#define USE_MIN_MAX_SETTING "useMinMax"
#define SUCCESS_MAX_SETTING "successMax"

class DiceTool : public QObject
{
    Q_OBJECT

public:
    explicit DiceTool(QQmlEngine *engine, QObject *parent = nullptr);
    explicit DiceTool() {}

    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    int sides() const { return m_sides; }

    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    int roll();

    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)
    QString calculationString() const { return m_calculation_string; }

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count) { m_bonus_dice = count; }
    Q_INVOKABLE void setAmount(int amount) { m_amount = amount; }
    Q_INVOKABLE void setModifier(int modifier) { m_modifier = modifier; }

    Q_INVOKABLE void setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax);
    Q_INVOKABLE bool getCriticalEnabled() const { return SettingsManager::getBoolSetting(ENABLE_CRITICALS_SETTING, true, DICE_SETTINGS); }
    Q_INVOKABLE int getSuccess() const { return SettingsManager::getSetting(SUCCESS_SETTING, QString::number(20), DICE_SETTINGS).toInt(); }
    Q_INVOKABLE int getFailure() const { return SettingsManager::getSetting(FAILURE_SETTING, QString::number(1), DICE_SETTINGS).toInt(); }
    Q_INVOKABLE bool getMinMax() const { return SettingsManager::getBoolSetting(USE_MIN_MAX_SETTING, false, DICE_SETTINGS); }
    Q_INVOKABLE bool getSuccessMax() const { return SettingsManager::getBoolSetting(SUCCESS_MAX_SETTING, true, DICE_SETTINGS); }

signals:
    void sidesChanged();
    void rollChanged();
    void calculationStringChanged();

    void mixedCriticalResult();
    void successfulCriticalResult();
    void failedCriticalResult();
    void normalResult();

private:
    int m_sides = 20;
    int m_bonus_dice = 0;
    int m_modifier = 0;
    int m_amount = 1;

    QString m_calculation_string;
};

#endif // DICETOOL_H
