#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include <QLocale>
#include "settings.h"
#include "../tools/audio/project/audioelement.h"

struct SettingRequest {
    SettingRequest() {}
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
    static SettingsManager* getInstance();
    ~SettingsManager();

    bool showToolNames() const { return getBoolSetting("showToolNames", false); }
    void setShowToolNames(bool checked) { setSetting("showToolNames", QString::number(checked)); emit showToolNamesChanged(); }

    bool classicIcons() const { return getBoolSetting("classicIcons", false); }
    void setClassicIcons(bool checked) { setSetting("classicIcons", QString::number(checked)); emit classicIconsChanged(); }

public slots:
    static QString getSetting(const QString& setting, const QString& defaultValue = "", QString group = DEFAULT_GROUP);
    static QString getSetting(SettingRequest request) { return getSetting(request.identifier, request.defaultValue, request.group); }
    static void setSetting(const QString& setting, const QString& value, const QString& group = DEFAULT_GROUP);
    static void setSetting(const QString& setting, const int& value, const QString& group = DEFAULT_GROUP);

    static QString getPath(const QString& setting, QString group = "");
    static QString getPath(AudioElement::Type type);
    static void setPath(const QString& setting, const QString& value, QString group = "");

    static QLocale getLanguage();
    static QString getLanguageString();
    static int getLanguageIndex();
    static QStringList getLanguages();
    static QStringList getLanguageNames();
    static void setLanguage(const QString& language);

    static QString getServerUrl(const QString &service, const bool &hasDefault = true);
    static void setServerUrl(const QString& url, const QString& service);

    static QString getPassword(const QString& username, const QString& service);
    static void setPassword(const QString& username, const QString& password, const QString& service);

    static bool isUpdateCheckEnabled();
    static bool getBoolSetting(const QString& setting, bool defaultValue = true, const QString& group = "");

    void setAddonEnabled(const QString& addon, bool enabled);
    bool getIsAddonEnabled(const QString& addon);

    QStringList getOfficialAddons();

signals:
    void showToolNamesChanged();
    void classicIconsChanged();

private:
    SettingsManager();
    static bool instanceFlag;
    static SettingsManager *single;

    static QString getDefaultPath(const QString& setting, const QString& group = PATHS_GROUP);
    static QString getActivePathGroup();

    void updateSettings();
    void renameSetting(const QString& currentName, const QString& newName, const QString& group = DEFAULT_GROUP);
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
