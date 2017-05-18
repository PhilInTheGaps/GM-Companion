#include "settingsmanager.h"

#include <QTextStream>
#include <QFileDialog>

SettingsManager::SettingsManager()
{

}
// Returns a specific setting
QString SettingsManager::getSetting(Setting setting){
    QString settingString;

    QSettings settings("settings.ini", QSettings::IniFormat);

    switch (setting) {
    case Setting::musicPath:
        settingString = settings.value("musicPath", "").toString();
        break;
    case Setting::soundPath:
        settingString = settings.value("soundPath", "").toString();
        break;
    case Setting::mapsPath:
        settingString = settings.value("mapsPath", "").toString();
        break;
    case Setting::resourcesPath:
        settingString = settings.value("resourcesPath", "").toString();
        break;
    case Setting::checkForUpdatesOnStart:
        settingString = settings.value("checkForUpdatesOnStart", "true").toString();
        break;
    case Setting::uiMode:
        settingString = settings.value("uiMode", "dark").toString();
        break;
    default:
        settingString = "";
        break;
    }
    return settingString;
}

// Sets a specific setting
void SettingsManager::setSetting(Setting setting, bool checked){
    QString path;
    QSettings settings("settings.ini", QSettings::IniFormat);

    switch (setting) {
    case Setting::musicPath:
        path = setFolderLocation("Set Music Folder");
        if (path.length()>1){
            settings.setValue("musicPath", path);
        }
        break;
    case Setting::soundPath:
        path = setFolderLocation("Set Sound Folder");
        if (path.length()>1){
            settings.setValue("soundPath", path);
        }
        break;
    case Setting::mapsPath:
        path = setFolderLocation("Set Maps Folder");
        if (path.length()>1){
            settings.setValue("mapsPath", path);
        }
        break;
    case Setting::resourcesPath:
        path = setFolderLocation("Set Resources Folder");
        if (path.length()>1){
            settings.setValue("resourcesPath", path);
        }
        break;
    case Setting::checkForUpdatesOnStart:
        settings.setValue("checkForUpdatesOnStart", checked);
        break;
    default:
        break;
    }
}

// Opens a Directory Chooser to set the new folder location
QString SettingsManager::setFolderLocation(QString windowTitle){
    QString path;
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle(windowTitle);
    if (fileDialog->exec() == QDialog::Accepted){
        QStringList paths = fileDialog->selectedFiles();
        path = paths.at(0);
    }

    return path;
}
