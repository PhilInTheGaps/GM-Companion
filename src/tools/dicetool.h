#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class DiceTool : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    AUTO_PROPERTY_VAL2(int, sides, 20);
    AUTO_PROPERTY2(QString, result, QStringLiteral("-"))
    READ_PROPERTY(QString, calculation)

public:
    DiceTool() = delete;
    using QObject::QObject;

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> DiceTool *;

    Q_INVOKABLE void roll();

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
    void mixedCriticalResult();
    void successfulCriticalResult();
    void failedCriticalResult();
    void normalResult();

private:
    int m_bonusDice = 0;
    int m_modifier = 0;
    int m_amount = 1;
};
