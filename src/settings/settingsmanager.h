#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include "settings.h"

struct SettingRequest {
    SettingRequest() {}
    SettingRequest(QString identifier, QString defaultValue, QString group = DEFAULT_GROUP)
    {
        this->identifier = identifier;
        this->defaultValue = defaultValue;
        this->group = group;
    }

    QString identifier;
    QString defaultValue;
    QString group;
};

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager* getInstance();
    ~SettingsManager();

    static QString getSetting(const QString& setting, const QString& defaultValue = "", QString group = DEFAULT_GROUP);
    static QString getSetting(SettingRequest request) { return getSetting(request.identifier, request.defaultValue, request.group); }
    static void setSetting(const QString& setting, const QString& value, const QString& group = DEFAULT_GROUP);
    static void setSetting(QString setting, int value, QString group = DEFAULT_GROUP);

    static QString getPath(const QString& setting, QString group = "");
    static void setPath(QString setting, QString value, QString group);

    static QString getLanguage();
    static void setLanguage(QString language);

    static QString getServerUrl();
    static void setServerUrl(QString url);

    static QString getPassword(const QString& username, const QString& service);
    static void setPassword(const QString& username, const QString& password, const QString& service);

    void setAddonEnabled(const QString& addon, bool enabled);
    bool getIsAddonEnabled(const QString& addon);

    QStringList getOfficialAddons();

private:
    SettingsManager();
    static bool instanceFlag;
    static SettingsManager *single;

    static QString getDefaultPath(const QString& setting, const QString& group = PATHS_GROUP);
    static QString getActivePathGroup();

    void updateSettings();
    void renameSetting(const QString& currentName, QString newName, const QString& group = DEFAULT_GROUP);
    void removeSetting(const QString& setting, const QString& group);

    // Normal Settings
    QSettings *m_settings;
    QStringList m_settingsList;

    // Addon Settings
    QSettings m_addonSettings;

    QStringList m_officialAddons = {
        "DSA5",
        "SIFRP",
        "HowToBeAHero",
        "90MinutesOfMayhem",
    };
};

#endif // SETTINGSMANAGER_H
