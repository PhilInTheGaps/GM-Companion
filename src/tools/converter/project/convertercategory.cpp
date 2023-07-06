#include "convertercategory.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

ConverterCategory::ConverterCategory(const QString &name, QObject *parent) : ConverterCategory(name, {}, parent)
{
}

ConverterCategory::ConverterCategory(const QString &name, const QList<ConverterUnit *> &units, QObject *parent)
    : QObject{parent}, a_name(name), a_units(units)
{
}

ConverterCategory::ConverterCategory(const QJsonObject &json, QObject *parent)
    : ConverterCategory(json["name"_L1].toString(), parent)
{
    const auto units = json["units"_L1].toArray();
    a_units.reserve(units.size());

    foreach (const auto &unit, units)
    {
        a_units << new ConverterUnit(unit.toObject(), this);
    }
}

auto ConverterCategory::toJson() const -> QJsonObject
{
    QJsonArray unitsJson;

    foreach (const auto *unit, this->units())
    {
        unitsJson << unit->toJson();
    }

    return QJsonObject{{"name", name()}, {"units", unitsJson}};
}
