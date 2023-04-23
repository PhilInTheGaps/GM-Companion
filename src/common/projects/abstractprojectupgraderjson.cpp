#include "abstractprojectupgraderjson.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAbstractProjectUpgraderJSON, "gm.common.projects.upgrader.json")

AbstractProjectUpgraderJSON::AbstractProjectUpgraderJSON(int version) : AbstractProjectUpgrader(version)
{
}

void AbstractProjectUpgraderJSON::parse(const QByteArray &data)
{
    QJsonParseError error;
    m_json = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError)
    {
        qCWarning(gmAbstractProjectUpgraderJSON()) << "Could not parse project:" << error.errorString();
    }
}

auto AbstractProjectUpgraderJSON::isProjectCompatible() const -> bool
{
    const auto project = m_json.object();
    const auto version = project[QStringLiteral("version")].toInt(0);
    return version == this->version();
}

auto AbstractProjectUpgraderJSON::json() const -> QJsonDocument
{
    return m_json;
}
