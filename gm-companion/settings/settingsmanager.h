#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>

enum Setting{
    musicPath,
    soundPath,
    mapsPath,
    shopPath,
    resourcesPath,
    uiMode,
    charactersPath,
    openWhatIsNewWindow,
    language,
    version,
    notesPath,
    audioPath,
    radioPath,
    drive_music
};

class SettingsManager
{
public:
    SettingsManager();

    void setSetting(Setting setting, int checked = 1, QString value = "");
    QString getSetting(Setting);

    void setAddonEnabled(QString addon, bool enabled);
    bool getIsAddonEnabled(QString addon);

    QStringList getOfficialAddons();

    QStringList getInactiveCharacters();
    void setInactiveCharacters(QStringList characters);

    void updateSettings();

private:
    // Normal Settings
    QSettings *settings;
    QStringList settingsList;

    // Addon Settings
    QSettings addonSettings;

    QStringList officialAddons = {
        "DSA5",
        "SIFRP",
        "HowToBeAHero",
        "90MinutesOfMayhem",
    };
};

#endif // SETTINGSMANAGER_H
