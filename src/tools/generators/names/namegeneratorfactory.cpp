#include "namegeneratorfactory.h"
#include "listnamegenerator.h"
#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmNameGeneratorFactory, "gm.generators.names.factory")

auto NameGeneratorFactory::buildFromJson(QObject *parent, const QJsonDocument &json) -> QList<AbstractNameGenerator*>
{
    QList<AbstractNameGenerator*> list;

    auto object = json.object();
    auto generators = object["generators"].toArray();

    for (const auto& generator : generators)
    {
        list << buildFromJson(parent, generator.toObject());
    }

    return list;
}

auto NameGeneratorFactory::buildFromJson(QObject *parent, const QJsonObject &json) -> AbstractNameGenerator*
{
    auto type = json["type"].toString();

    if (type == "list")
    {
        return new ListNameGenerator(parent, json);
    }

    qCWarning(gmNameGeneratorFactory()) << "Error: Unsupported name generator type" << type;
    return nullptr;
}
