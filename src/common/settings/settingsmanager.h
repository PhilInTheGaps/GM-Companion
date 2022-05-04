#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include <QLocale>
#include <QFuture>
#include "settings.h"

struct SettingRequest {
    SettingRequest() = default;
    SettingRequest(const QString& identifier, const QString& defaultValue, const QString& group = DEFAULT_GROUP) :
        identifier(identifier), defaultValue(defaultValue), group(group) {}

    QString identifier;
    QString defaultValue;
    QString group;
};

class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showToolNames READ showToolNames WRITE setShowToolNames NOTIFY showToolNamesChanged)
    Q_PROPERTY(bool classicIcons READ classicIcons WRITE setClassicIcons NOTIFY classicIconsChanged)

public:
    static auto getInstance() -> SettingsManager*;

    [[nodiscard]] auto showToolNames() const -> bool;
    void setShowToolNames(bool checked);

    [[nodiscard]] auto classicIcons() const -> bool;
    void setClassicIcons(bool checked);

public slots:
    static QString getSetting(const QString& setting, const QString& defaultValue = "", QString group = DEFAULT_GROUP);
    static QString getSetting(const SettingRequest &request);
    static void setSetting(const QString& setting, const QString& value, const QString& group = DEFAULT_GROUP);
    static void setSetting(const QString& setting, const int& value, const QString& group = DEFAULT_GROUP);

    static QString getPath(const QString& setting, QString group = "");
    static void setPath(const QString& setting, const QString& value, QString group = "");

    static QLocale getLanguage();
    static QString getLanguageString();
    static int getLanguageIndex();
    static QStringList getLanguages();
    static QStringList getLanguageNames();
    static void setLanguage(const QString& language);

    static QString getServerUrl(const QString &service, const bool &hasDefault = true);
    static void setServerUrl(const QString& url, const QString& service);

    static QFuture<QString> getPassword(const QString& username, const QString& service);
    static void setPassword(const QString& username, const QString& password, const QString& service);

    static bool isUpdateCheckEnabled();
    static bool getBoolSetting(const QString& setting, bool defaultValue = true, const QString& group = DEFAULT_GROUP);

    static bool hasSetting(const QString &setting, const QString &group = DEFAULT_GROUP);

    void setAddonEnabled(const QString& addon, bool enabled);
    bool getIsAddonEnabled(const QString& addon);

    QStringList getOfficialAddons();

signals:
    void showToolNamesChanged();
    void classicIconsChanged();

private:
    SettingsManager();
    inline static SettingsManager *m_instance = nullptr;

    static auto getDefaultPath(const QString& setting, const QString& group = PATHS_GROUP) -> QString;
    static auto getActivePathGroup() -> QString;

    static auto defaultServerUrl() -> QString;

    void updateSettings();
    void renameSetting(const QString& currentName, const QString& newName, const QString& group = DEFAULT_GROUP);
    void removeSetting(const QString& setting, const QString& group);

    // Normal Settings
    QSettings *m_settings;
    QStringList m_settingsList;

    // Addon Settings
    QSettings m_addonSettings;

    const QStringList m_officialAddons = {
        "DSA5",
        "SIFRP",
        "HowToBeAHero",
        "90MinutesOfMayhem",
    };
};

#endif // SETTINGSMANAGER_H
