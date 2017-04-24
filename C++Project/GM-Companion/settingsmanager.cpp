#include "settingsmanager.h"

#include <QTextStream>
#include <QFileDialog>

SettingsManager::SettingsManager()
{

}
// Returns a specific setting
QString SettingsManager::getSetting(Setting setting){
    QString settingString;

    switch (setting) {
    case Setting::musicPath:
        settingString = musicPath;
        break;
    case Setting::soundPath:
        settingString = soundPath;
        break;
    case Setting::mapsPath:
        settingString = mapsPath;
        break;
    case Setting::resourcesPath:
        settingString = resourcesPath;
        break;
    case Setting::checkForUpdatesOnStart:
        settingString = QString::number(checkForUpdatesOnStart);
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
    switch (setting) {
    case Setting::musicPath:
        path = setFolderLocation("Set Music Folder");
        if (path.length()>1){
            musicPath = path;
        }
        break;
    case Setting::soundPath:
        path = setFolderLocation("Set Sound Folder");
        if (path.length()>1){
            soundPath = path;
        }
        break;
    case Setting::mapsPath:
        path = setFolderLocation("Set Maps Folder");
        if (path.length()>1){
            mapsPath = path;
        }
        break;
    case Setting::resourcesPath:
        path = setFolderLocation("Set Resources Folder");
        if (path.length()>1){
            resourcesPath = path;
        }
        break;
    case Setting::checkForUpdatesOnStart:
        checkForUpdatesOnStart = checked;
        break;
    default:
        break;
    }
    saveSettings();
}

// Writes all settings to settings.txt
void SettingsManager::saveSettings(){ //QString content, QString indicator, QString newSetting
    QFile file(settingsPath);
    file.open(QIODevice::WriteOnly);
    QTextStream output(&file);

    txtContent.clear();
    txtContent.append("MUSIC_PATH="+musicPath+";\n");
    txtContent.append("SOUND_PATH="+soundPath+";\n");
    txtContent.append("MAPS_PATH="+mapsPath+";\n");
    txtContent.append("RESOURCES_PATH="+resourcesPath+";\n");
    txtContent.append("CHECK_FOR_UPDATES_ON_START="+QString::number(checkForUpdatesOnStart)+";\n");

    output << txtContent;
    file.close();
}

// Reads all the settings and sets the variables accordingly
void SettingsManager::readSettings(){
    QFile file(settingsPath);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    txtContent = textStream.readAll();
    file.close();

    // Setting the settings read from settings.txt
    musicPath = readSettingFromString("MUSIC_PATH=");
    soundPath = readSettingFromString("SOUND_PATH=");
    mapsPath = readSettingFromString("MAPS_PATH=");
    resourcesPath = readSettingFromString("RESOURCES_PATH=");
    checkForUpdatesOnStart = readSettingFromString("CHECK_FOR_UPDATES_ON_START=").toInt();
}

// Reads a specific setting from settings.txt
QString SettingsManager::readSettingFromString(QString indicator){
    QString setting;
    if (txtContent.contains(indicator)){
        int i1 = txtContent.indexOf(indicator);
        int i2 = txtContent.indexOf(";", i1);

        setting = txtContent.mid(i1+indicator.length(), i2-i1-indicator.length());
    }
    return setting;
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
