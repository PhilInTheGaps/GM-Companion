#include "namegeneratorfactory.h"
#include "listnamegenerator.h"
#include <QJsonArray>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmNameGeneratorFactory, "gm.generators.names.factory")

auto NameGeneratorFactory::buildFromJson(QObject *parent, const QJsonDocument &json) -> QList<AbstractNameGenerator *>
{
    auto object = json.object();
    auto generators = object["generators"_L1].toArray();

    QList<AbstractNameGenerator *> list;
    list.reserve(generators.count());

    for (const auto &generator : generators)
    {
        list << buildFromJson(parent, generator.toObject());
    }

    return list;
}

auto NameGeneratorFactory::buildFromJson(QObject *parent, const QJsonObject &json) -> AbstractNameGenerator *
{
    auto type = json["type"_L1].toString();

    if (type == "list"_L1)
    {
        return new ListNameGenerator(parent, json);
    }

    qCWarning(gmNameGeneratorFactory()) << "Error: Unsupported name generator type" << type;
    return nullptr;
}
