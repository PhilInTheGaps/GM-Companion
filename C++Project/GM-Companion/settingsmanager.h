#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>

enum UI_Mode{
    dark
};

enum Setting{
    musicPath,
    soundPath,
    mapsPath,
    resourcesPath,
    checkForUpdatesOnStart,
    uiMode,
    charactersPath
};

class SettingsManager
{
public:
    SettingsManager();

    void setSetting(Setting setting, int checked = 1);
    QString getSetting(Setting);

    void setAddonEnabled(QString addon, bool enabled);
    bool getIsAddonEnabled(QString addon);

    QStringList getOfficialAddons();

private:
    // Normal Settings
    QSettings settings;

    QString setFolderLocation(QString windowTitle);

    // Addon Settings
    QSettings addonSettings;

    QStringList officialAddons = {
        "DSA5",
        "SIFRP"
    };
};

#endif // SETTINGSMANAGER_H
