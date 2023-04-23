#include "converterunit.h"

ConverterUnit::ConverterUnit(const QString &name, double value, QObject *parent)
    : QObject{parent}, a_name(name), a_value(value)
{
}

ConverterUnit::ConverterUnit(const QJsonObject &json, QObject *parent)
    : ConverterUnit(json[QStringLiteral("name")].toString(), json[QStringLiteral("value")].toDouble(), parent)
{
}

auto ConverterUnit::toJson() const -> QJsonObject
{
    return QJsonObject{{"name", name()}, {"value", value()}};
}
