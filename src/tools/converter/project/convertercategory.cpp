#include "convertercategory.h"
#include <QJsonArray>

ConverterCategory::ConverterCategory(const QString &name, QObject *parent) : ConverterCategory(name, {}, parent)
{
}

ConverterCategory::ConverterCategory(const QString &name, const QList<ConverterUnit *> &units, QObject *parent)
    : QObject{parent}, a_name(name), a_units(units)
{
}

ConverterCategory::ConverterCategory(const QJsonObject &json, QObject *parent)
    : ConverterCategory(json[QStringLiteral("name")].toString(), parent)
{
    const auto units = json[QStringLiteral("units")].toArray();
    a_units.reserve(units.size());

    for (const auto &unit : units)
    {
        a_units << new ConverterUnit(unit.toObject(), this);
    }
}

auto ConverterCategory::toJson() const -> QJsonObject
{
    QJsonArray unitsJson;
    const auto units = this->units();
    for (const auto *unit : units)
    {
        unitsJson << unit->toJson();
    }

    return QJsonObject{{"name", name()}, {"units", unitsJson}};
}
