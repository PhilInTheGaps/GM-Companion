#include "addon.h"
#include <utility>
#include <QJsonObject>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAddons, "gm.addons")

Addon::Addon(QObject *parent, QString name, QString shortName, QString version, QString author, QString description, QString path)
    : QObject{parent}, a_name(std::move(name)), a_shortName(std::move(shortName)), a_version(std::move(version)),
      a_author(std::move(author)), a_description(std::move(description)), a_path(std::move(path)), a_enabled(true)
{
    qCDebug(gmAddons) << "Loaded addon:" << QString("%1 (%2) [%3] - %4")
                         .arg(this->name(), this->shortName(), this->version(), this->path());
}

auto Addon::fromJson(QObject *parent, const QJsonDocument &json, QString path) -> Addon*
{
    auto root = json.object();
    auto name = root["name"].toString();
    auto shortName = root["name_short"].toString();
    auto version = root["version"].toString();
    auto author = root["author"].toString();
    auto description = root["description"].toString();

    return new Addon(parent, name, shortName, version, author, description, std::move(path));
}
