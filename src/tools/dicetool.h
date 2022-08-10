#ifndef DICETOOL_H
#define DICETOOL_H

#include <QObject>
#include <QQmlEngine>

class DiceTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)

public:
    explicit DiceTool(QQmlEngine *engine, QObject *parent = nullptr);
    explicit DiceTool(QObject *parent) : QObject(parent) {}

    [[nodiscard]] auto sides() const -> int { return m_sides; }
    [[nodiscard]] auto roll() -> int;
    [[nodiscard]] auto calculationString() const -> QString { return m_calculationString; }

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count) { m_bonusDice = count; }
    Q_INVOKABLE void setAmount(int amount) { m_amount = amount; }
    Q_INVOKABLE void setModifier(int modifier) { m_modifier = modifier; }

    Q_INVOKABLE static void setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax);
    Q_INVOKABLE [[nodiscard]] static bool getCriticalEnabled();
    Q_INVOKABLE [[nodiscard]] static int getSuccess();
    Q_INVOKABLE [[nodiscard]] static int getFailure();
    Q_INVOKABLE [[nodiscard]] static int getMinMax();
    Q_INVOKABLE [[nodiscard]] static int getSuccessMax();

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

#endif // DICETOOL_H
