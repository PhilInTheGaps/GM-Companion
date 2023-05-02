#include "addon.h"
#include "updates/version.h"
#include <QJsonObject>
#include <QLoggingCategory>
#include <utility>

Q_LOGGING_CATEGORY(gmAddons, "gm.addons")

Addon::Addon(QObject *parent, QString id, QString name, QString shortName, QString version, QString author,
             QString description, QString path, bool isLocal, Type type)
    : QObject{parent}, a_id(std::move(id)), a_name(std::move(name)), a_shortName(std::move(shortName)),
      a_version(std::move(version)), a_author(std::move(author)), a_description(std::move(description)),
      a_path(std::move(path)), a_isLocal(isLocal), a_type(type)
{
    qCDebug(gmAddons) << "Loaded addon:"
                      << QStringLiteral("%1: %2 (%3) [%4] - %5")
                             .arg(this->id(), this->name(), this->shortName(), this->version(), this->path());
}

auto Addon::fromJson(QObject *parent, const QJsonDocument &json, QString path, bool isLocal, Type type) -> Addon *
{
    auto root = json.object();
    auto id = root[QStringLiteral("id")].toString();
    auto name = root[QStringLiteral("name")].toString();
    auto shortName = root[QStringLiteral("name_short")].toString();
    auto version = root[QStringLiteral("version")].toString();
    auto author = root[QStringLiteral("author")].toString();
    auto description = root[QStringLiteral("description")].toString();

    return new Addon(parent, id, name, shortName, version, author, description, std::move(path), isLocal, type);
}

auto Addon::fromReleaseInfo(QObject *parent, const AddonReleaseInfo &release) -> Addon *
{
    auto *addon = new Addon(parent, release.id, release.name, release.shortName, QLatin1String(), release.author,
                            release.description, QLatin1String(), false, Type::Archive);
    addon->setReleaseInfo(release);

    return addon;
}

void Addon::setReleaseInfo(const AddonReleaseInfo &info)
{
    m_releaseInfo = info;
    emit isUpdateAvailableChanged();
}

void Addon::updateWithReleaseInfo()
{
    if (m_releaseInfo.name.isEmpty()) return;

    name(m_releaseInfo.name);
    shortName(m_releaseInfo.shortName);
    version(m_releaseInfo.version);
    author(m_releaseInfo.author);
    description(m_releaseInfo.description);

    emit isUpdateAvailableChanged();
}

auto Addon::newVersion() const -> QString
{
    return m_releaseInfo.version;
}

auto Addon::downloadUrl() const -> QString
{
    return m_releaseInfo.downloadUrl;
}

auto Addon::isInstalled() const -> bool
{
    return !path().isEmpty();
}

auto Addon::isInstalledAndEnabled() const -> bool
{
    return isInstalled() && enabled();
}

auto Addon::isUpdateAvailable() const -> bool
{
    return isInstalled() && Version::isGreater(Version(m_releaseInfo.version), Version(version()));
}
