#include "converterproject.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

ConverterProject::ConverterProject(const QString &name, QObject *parent) : ConverterProject(name, {}, parent)
{
}

ConverterProject::ConverterProject(const QString &name, const QList<ConverterCategory *> &categories, QObject *parent)
    : QObject{parent}, a_name(name), a_categories(categories)
{
}

ConverterProject::ConverterProject(const QJsonObject &json, QObject *parent)
    : ConverterProject(json["name"_L1].toString(), parent)
{
    a_version = json["version"_L1].toInt();

    const auto categories = json["categories"_L1].toArray();
    a_categories.reserve(categories.size());

    foreach (const auto &category, categories)
    {
        a_categories << new ConverterCategory(category.toObject(), this);
    }
}

auto ConverterProject::toJson() const -> QJsonDocument
{
    QJsonArray categoriesJson;

    foreach (const auto *category, this->categories())
    {
        categoriesJson << category->toJson();
    }

    const QJsonObject json{{"name", name()}, {"version", version()}, {"categories", categoriesJson}};
    return QJsonDocument(json);
}

void ConverterProject::setCategories(const QList<ConverterCategory *> &categories)
{
    a_categories = categories;
    emit categoriesChanged();
}
