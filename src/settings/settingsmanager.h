#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include "settings.h"

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager* getInstance();
    ~SettingsManager();

    static QString getSetting(QString setting, QString defaultValue = "", QString group = DEFAULT_GROUP);
    static void setSetting(QString setting, QString value, QString group = DEFAULT_GROUP);
    static void setSetting(QString setting, int value, QString group = DEFAULT_GROUP);

    static QString getPath(QString setting);
    static void setPath(QString setting, QString value);

    static QString getLanguage();
    static void setLanguage(QString language);

    static QString getServerUrl();
    static void setServerUrl(QString url);

    static QString getPassword(QString username, QString service);
    static void setPassword(QString username, QString password, QString service);

    void setAddonEnabled(QString addon, bool enabled);
    bool getIsAddonEnabled(QString addon);

    QStringList getOfficialAddons();

    QStringList getInactiveCharacters();
    void setInactiveCharacters(QStringList characters);

    void updateSettings();

private:
    SettingsManager();
    static bool instanceFlag;
    static SettingsManager *single;

    QString getDefaultPath(QString setting);

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
