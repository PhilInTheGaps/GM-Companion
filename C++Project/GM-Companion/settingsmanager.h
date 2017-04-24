#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QDir>

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

    void readSettings();
    void saveSettings();

private:
    QString settingsPath = QDir::currentPath()+"/settings.txt";
    QString musicPath;
    QString soundPath;
    QString mapsPath;
    QString resourcesPath;
    bool checkForUpdatesOnStart;
    UI_Mode uiMode;

    QString txtContent;

    QString setFolderLocation(QString windowTitle);
    QString readSettingFromString(QString indicator);
};

#endif // SETTINGSMANAGER_H
