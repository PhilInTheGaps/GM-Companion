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
    resourcesPath,
    uiMode,
    charactersPath,
    buttonStyle,
    enableMusicTrackList,
    openWhatIsNewWindow,
    language,
    version,
    notesPath,
    audioPath,
    radioPath
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
    QSettings settings;

    QString setFolderLocation(QString windowTitle);

    // Addon Settings
    QSettings addonSettings;

    QStringList officialAddons = {
        "DSA5",
        "SIFRP",
        "HowToBeAHero"
    };
};

#endif // SETTINGSMANAGER_H
