#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
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
    uiMode
};

class SettingsManager
{
public:
    SettingsManager();

    void setSetting(Setting setting, bool checked = true);
    QString getSetting(Setting);

private:
    QSettings settings;

    QString setFolderLocation(QString windowTitle);
};

#endif // SETTINGSMANAGER_H
