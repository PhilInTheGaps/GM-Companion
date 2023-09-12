#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class DiceTool : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)

public:
    DiceTool() = delete;
    using QObject::QObject;

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> DiceTool *;

    [[nodiscard]] auto sides() const -> int
    {
        return m_sides;
    }
    [[nodiscard]] auto roll() -> int;
    [[nodiscard]] auto calculationString() const -> QString
    {
        return m_calculationString;
    }

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count)
    {
        m_bonusDice = count;
    }
    Q_INVOKABLE void setAmount(int amount)
    {
        m_amount = amount;
    }
    Q_INVOKABLE void setModifier(int modifier)
    {
        m_modifier = modifier;
    }

    Q_INVOKABLE static void setDiceSettings(bool enableCriticals, int success, int failure, bool minMax,
                                            bool successMax);
    [[nodiscard]] Q_INVOKABLE static bool getCriticalEnabled();
    [[nodiscard]] Q_INVOKABLE static int getSuccess();
    [[nodiscard]] Q_INVOKABLE static int getFailure();
    [[nodiscard]] Q_INVOKABLE static int getMinMax();
    [[nodiscard]] Q_INVOKABLE static int getSuccessMax();

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
    int m_bonusDice = 0;
    int m_modifier = 0;
    int m_amount = 1;

    QString m_calculationString;
};
