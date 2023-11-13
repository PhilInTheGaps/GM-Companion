#pragma once

#include "../settingsmanager.h"
#include "macros.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class Spotify : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    SETTINGS_PROPERTY2(QString, username, QLatin1String(), GROUP)
    SETTINGS_PROPERTY2(QString, clientSecret, QLatin1String(), GROUP)
    SETTINGS_PROPERTY2(QString, clientId, QLatin1String(), GROUP)
    SETTINGS_PROPERTY2(QString, connection, QLatin1String(), GROUP)
    SETTINGS_PROPERTY_VAL2(bool, enableCache, true, GROUP)
    SETTINGS_PROPERTY_VAL2(bool, enableVolumeNormalization, false, GROUP)
    SETTINGS_PROPERTY2(QString, bitrate, QStringLiteral("160"), GROUP)

public:
    using QObject::QObject;

    Q_INVOKABLE static QString getServerUrl()
    {
        return SettingsManager::getServerUrl(GROUP, true);
    }

    Q_INVOKABLE static void setServerUrl(const QString &url)
    {
        SettingsManager::setServerUrl(url, GROUP);
    }

    Q_INVOKABLE static void setPassword(const QString &username, const QString &password)
    {
        SettingsManager::setPassword(username, password, GROUP);
    }

private:
    inline static constexpr auto GROUP = "Spotify";
};

} // namespace Common::Settings::Quick
