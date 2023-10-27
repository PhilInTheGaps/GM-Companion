#pragma once

#include "qmlsingletonfactory.h"
#include "settingsmanager.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QtQml/qqmlregistration.h>

#define SETTINGS_PROPERTY(TYPE, NAME, DEFAULT) SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, DEFAULT_GROUP)

#define SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, GROUP) SETTINGS_PROPERTY3(TYPE, NAME, #NAME, DEFAULT, GROUP)

#define SETTINGS_PROPERTY3(TYPE, NAME, SETTING, DEFAULT, GROUP)                                                        \
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)                                                    \
public:                                                                                                                \
    [[nodiscard]] static auto NAME()->TYPE                                                                             \
    {                                                                                                                  \
        return SettingsManager::instance()->get<TYPE>(SETTING, DEFAULT, GROUP);                                        \
    }                                                                                                                  \
    void NAME(const TYPE &value)                                                                                       \
    {                                                                                                                  \
        SettingsManager::instance()->set(SETTING, value, GROUP);                                                       \
        emit NAME##Changed(value);                                                                                     \
    }                                                                                                                  \
    Q_SIGNAL void NAME##Changed(TYPE value);

/// Settings Manager that implements specific settings as properties
/// which can be used from the QML side
class QuickSettingsManager : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SettingsManager)
    QML_SINGLETON
    QML_SINGLETON_FACTORY(QuickSettingsManager)

    // General Settings
    SETTINGS_PROPERTY(bool, showToolNames, false)
    SETTINGS_PROPERTY(bool, classicIcons, false)
    SETTINGS_PROPERTY(QString, cloudMode, QStringLiteral("local"))
    READ_PROPERTY2(QString, languageBcp47, SettingsManager::instance()->getLanguageBcp47())

#ifdef NO_UPDATE_CHECK
    SETTINGS_PROPERTY2(bool, checkForUpdates, false, QStringLiteral("Updates"))
#else
    SETTINGS_PROPERTY2(bool, checkForUpdates, true, QStringLiteral("Updates"))
#endif

    // Telemetry
    SETTINGS_PROPERTY2(bool, crashReports, false, QStringLiteral("Telemetry"))
    SETTINGS_PROPERTY2(bool, sessionTracking, false, QStringLiteral("Telemetry"))

    // Google
    SETTINGS_PROPERTY2(QString, googleID, QLatin1String(), QStringLiteral("Google"))
    SETTINGS_PROPERTY2(QString, googleSecret, QLatin1String(), QStringLiteral("Google"))

    // Spotify
    SETTINGS_PROPERTY2(QString, spotifyUsername, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY2(QString, spotifySecret, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY2(QString, spotifyID, QLatin1String(), QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(QString, spotifyConnection, QStringLiteral("connection"), QLatin1String(),
                       QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(bool, spotifyEnableCache, QStringLiteral("enableCache"), true, QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(bool, spotifyEnableVolumeNormalization, QStringLiteral("enableVolumeNormalization"), false,
                       QStringLiteral("Spotify"))
    SETTINGS_PROPERTY3(QString, spotifyBitrate, QStringLiteral("bitrate"), QStringLiteral("160"),
                       QStringLiteral("Spotify"))

public:
    QuickSettingsManager() = delete;
    Q_INVOKABLE static bool has(const QString &setting, const QString &group);

    Q_INVOKABLE static QString getPath(const QString &path);
    Q_INVOKABLE static void setPath(const QString &path, const QString &value);

    Q_INVOKABLE static int getLanguageIndex();
    Q_INVOKABLE static QStringList getLanguageNames();
    Q_INVOKABLE void setLanguage(const QString &language);

    Q_INVOKABLE static QString getServerUrl(const QString &service, bool hasDefault);
    Q_INVOKABLE static void setServerUrl(const QString &url, const QString &service);

    Q_INVOKABLE static void setPassword(const QString &username, const QString &password, const QString &service);

    static auto instance() -> QuickSettingsManager *;

signals:
    void languageChanged(QString language);

private:
    using QObject::QObject;
    inline static QuickSettingsManager *m_instance = nullptr;
};
