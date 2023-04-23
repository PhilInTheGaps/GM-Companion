#include "converterproject.h"
#include <QJsonArray>

ConverterProject::ConverterProject(const QString &name, QObject *parent) : ConverterProject(name, {}, parent)
{
}

ConverterProject::ConverterProject(const QString &name, const QList<ConverterCategory *> &categories, QObject *parent)
    : QObject{parent}, a_name(name), a_categories(categories), a_version(2)
{
}

ConverterProject::ConverterProject(const QJsonObject &json, QObject *parent)
    : ConverterProject(json[QStringLiteral("name")].toString(), parent)
{
    a_version = json[QStringLiteral("version")].toInt();

    const auto categories = json[QStringLiteral("categories")].toArray();
    a_categories.reserve(categories.size());

    for (const auto &category : categories)
    {
        a_categories << new ConverterCategory(category.toObject(), this);
    }
}

auto ConverterProject::toJson() const -> QJsonDocument
{
    QJsonArray categoriesJson;
    const auto categories = this->categories();
    for (const auto *category : categories)
    {
        categoriesJson << category->toJson();
    }

    const QJsonObject json{{"name", name()}, {"version", version()}, {"categories", categoriesJson}};
    return QJsonDocument(json);
}
