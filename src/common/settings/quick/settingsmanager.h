#pragma once

#include "../settingsmanager.h"
#include "colors.h"
#include "macros.h"
#include "qmlsingletonfactory.h"
#include "services.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include "window.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

/// Settings Manager that implements specific settings as properties
/// which can be used from the QML side
class SettingsManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_SINGLETON_FACTORY(SettingsManager)

    Q_PROPERTY(Common::Settings::Quick::Services *services READ services CONSTANT FINAL)
    Q_PROPERTY(Common::Settings::Quick::Colors *colors READ colors CONSTANT FINAL)
    Q_PROPERTY(Common::Settings::Quick::Window *window READ window CONSTANT FINAL)

    // General Settings
    SETTINGS_PROPERTY(QString, cloudMode, QStringLiteral("local"))
    READ_PROPERTY2(QString, languageBcp47, Common::Settings::SettingsManager::instance()->getLanguageBcp47())

#ifdef NO_UPDATE_CHECK
    SETTINGS_PROPERTY_VAL2(bool, checkForUpdates, false, "Updates")
#else
    SETTINGS_PROPERTY_VAL2(bool, checkForUpdates, true, "Updates")
#endif

    // Telemetry
    SETTINGS_PROPERTY_VAL2(bool, crashReports, false, "Telemetry")
    SETTINGS_PROPERTY_VAL2(bool, sessionTracking, false, "Telemetry")

public:
    SettingsManager() = delete;

    Q_INVOKABLE static bool has(const QString &setting, const QString &group);

    Q_INVOKABLE static QString getPath(const QString &path);
    Q_INVOKABLE static void setPath(const QString &path, const QString &value);

    Q_INVOKABLE static qsizetype getLanguageIndex();
    Q_INVOKABLE static QStringList getLanguageNames();
    Q_INVOKABLE void setLanguage(const QString &language);

    auto services() -> Services *
    {
        return &m_services;
    }

    auto colors() -> Colors *
    {
        return &m_colors;
    }

    auto window() -> Window *
    {
        return &m_window;
    }

    static auto instance() -> SettingsManager *;

signals:
    void languageChanged(QString language);

private:
    using QObject::QObject;

    Services m_services;
    Colors m_colors;
    Window m_window;

    inline static SettingsManager *m_instance = nullptr;
};

} // namespace Common::Settings::Quick
