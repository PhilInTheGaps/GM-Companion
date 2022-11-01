#include "namegeneratorfactory.h"
#include "listnamegenerator.h"
#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmNameGeneratorFactory, "gm.generators.names.factory")

auto NameGeneratorFactory::buildFromJson(QObject *parent, const QJsonDocument &json) -> QList<AbstractNameGenerator *>
{
    auto object = json.object();
    auto generators = object[QStringLiteral("generators")].toArray();

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
    auto type = json[QStringLiteral("type")].toString();

    if (type == QLatin1String("list"))
    {
        return new ListNameGenerator(parent, json);
    }

    qCWarning(gmNameGeneratorFactory()) << "Error: Unsupported name generator type" << type;
    return nullptr;
}
