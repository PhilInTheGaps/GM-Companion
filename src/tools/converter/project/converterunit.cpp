#include "converterunit.h"

using namespace Qt::Literals::StringLiterals;

ConverterUnit::ConverterUnit(const QString &name, double value, QObject *parent)
    : QObject{parent}, a_name(name), a_value(value)
{
}

ConverterUnit::ConverterUnit(const QJsonObject &json, QObject *parent)
    : ConverterUnit(json["name"_L1].toString(), json["value"_L1].toDouble(), parent)
{
}

auto ConverterUnit::toJson() const -> QJsonObject
{
    return QJsonObject{{"name", name()}, {"value", value()}};
}
