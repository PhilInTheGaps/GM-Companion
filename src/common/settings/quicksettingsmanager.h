#pragma once

#include <QObject>
#include <QVariant>
#include "settingsmanager.h"

#define SETTINGS_PROPERTY(TYPE, NAME, DEFAULT) \
 SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, DEFAULT_GROUP)

#define SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, GROUP) \
 SETTINGS_PROPERTY3(TYPE, NAME, #NAME, DEFAULT, GROUP)

#define SETTINGS_PROPERTY3(TYPE, NAME, SETTING, DEFAULT, GROUP)                \
 Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)               \
 public:                                                                       \
  [[nodiscard]] static auto NAME() -> TYPE {                                   \
    return SettingsManager::instance()->get<TYPE>(SETTING, DEFAULT, GROUP);\
  }                                                                            \
  void NAME(const TYPE& value) {                                               \
    SettingsManager::instance()->set(SETTING, value, GROUP);               \
    emit NAME##Changed(value);                                                 \
  }                                                                            \
  Q_SIGNAL void NAME##Changed(TYPE value);

/// Settings Manager that implements specific settings as properties
/// which can be used from the QML side
class QuickSettingsManager : public QObject
{
    Q_OBJECT

    // General Settings
    SETTINGS_PROPERTY(bool, showToolNames, false)
    SETTINGS_PROPERTY(bool, classicIcons, false)
    SETTINGS_PROPERTY(QString, cloudMode, QStringLiteral("local"))

#ifdef NO_UPDATE_CHECK
    SETTINGS_PROPERTY2(bool, checkForUpdates, false, QStringLiteral("Updates"))
#else
    SETTINGS_PROPERTY2(bool, checkForUpdates, true, QStringLiteral("Updates"))
#endif

    // Telemetry
    SETTINGS_PROPERTY2(bool, crashReports, false, QStringLiteral("Telemetry"))
    SETTINGS_PROPERTY2(bool, sessionTracking, false, QStringLiteral("Telemetry"))

    // Discord
    SETTINGS_PROPERTY3(bool, discordEnabled, QStringLiteral("enabled"), false, QStringLiteral("Discord"))
    SETTINGS_PROPERTY3(QString, discordChannel, QStringLiteral("channel"), QLatin1String(), QStringLiteral("Discord"))
    SETTINGS_PROPERTY3(QString, discordConnection, QStringLiteral("connection"), QStringLiteral("default"), QStringLiteral("Discord"))

    // Google
    SETTINGS_PROPERTY2(QString, googleID, QLatin1String(), QStringLiteral("Google"))
    SETTINGS_PROPERTY2(QString, googleSecret, QLatin1String(), QStringLiteral("Google"))

    // Spotify
    SETTINGS_PROPERTY2(QString, spotifyUsername, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY2(QString, spotifySecret, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY2(QString, spotifyID, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(QString, spotifyConnection, QStringLiteral("connection"), QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(bool, spotifyEnableCache, QStringLiteral("enableCache"), true, QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(bool, spotifyEnableVolumeNormalization, QStringLiteral("enableVolumeNormalization"), false, QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(QString, spotifyBitrate, QStringLiteral("bitrate"), QStringLiteral("160"), QStringLiteral("Spotify"))

public:
    explicit QuickSettingsManager(QObject *parent = nullptr);

    Q_INVOKABLE static bool has(const QString& setting, const QString& group);

    Q_INVOKABLE static QString getPath(const QString& path);
    Q_INVOKABLE static void setPath(const QString& path, const QString& value);

    Q_INVOKABLE static int getLanguageIndex();
    Q_INVOKABLE static QStringList getLanguageNames();
    Q_INVOKABLE static void setLanguage(const QString& language);

    Q_INVOKABLE static QString getServerUrl(const QString &service);
    Q_INVOKABLE static void setServerUrl(const QString& url, const QString& service);

    Q_INVOKABLE static void setPassword(const QString& username, const QString& password, const QString& service);
};

