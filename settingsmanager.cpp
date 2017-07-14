#include "settingsmanager.h"

#include <QTextStream>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>

SettingsManager::SettingsManager()
{

}
// Returns a specific setting
QString SettingsManager::getSetting(Setting setting){
    QString settingString;

    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

    switch (setting) {
    case Setting::musicPath:
        settingString = settings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString();
        break;
    case Setting::soundPath:
        settingString = settings.value("soundPath", QDir::homePath()+"/.gm-companion/sounds").toString();
        break;
    case Setting::mapsPath:
        settingString = settings.value("mapsPath", QDir::homePath()+"/.gm-companion/maps").toString();
        break;
    case Setting::resourcesPath:
        settingString = settings.value("resourcesPath", QDir::homePath()+"/.gm-companion/resources").toString();
        break;
    case Setting::charactersPath:
        settingString = settings.value("charactersPath", QDir::homePath()+"/.gm-companion/characters").toString();
        break;
    case Setting::checkForUpdatesOnStart:
        settingString = settings.value("checkForUpdatesOnStart", 1).toInt();
        break;
    case Setting::uiMode:
        settingString = settings.value("uiMode", "DarkStyle").toString();
        break;
    case Setting::buttonStyle:
        settingString = settings.value("buttonStyle", "small").toString();
        break;
    case Setting::enableMusicTrackList:
        settingString = settings.value("enableMusicTrackList", 0).toInt();
        break;
    case Setting::openWhatIsNewWindow:
        settingString = settings.value("openWhatIsNewWindow", 1).toInt();
        break;
    case Setting::language:
        settingString = settings.value("language", "en").toString();
        break;
    case Setting::version:
        settingString = QString::number(settings.value("version", 0).toInt());
        break;
    case Setting::notesPath:
        settingString = settings.value("notesPath", QDir::homePath()+"/.gm-companion/notes").toString();
        break;
    default:
        settingString = "";
        break;
    }
    return settingString;
}

// Sets a specific setting
void SettingsManager::setSetting(Setting setting, int checked, QString value){
    QString path;
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

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
    case Setting::charactersPath:
        path = setFolderLocation("Set Characters Folder");
        if (path.length()>1){
            settings.setValue("charactersPath", path);
        }
        break;
    case Setting::checkForUpdatesOnStart:
        if (checked){
            settings.setValue("checkForUpdatesOnStart", 1);
        }else{
            settings.setValue("checkForUpdatesOnStart", 0);
        }
        break;
    case Setting::uiMode:
        settings.setValue("uiMode", value);
        break;
    case Setting::buttonStyle:
        settings.setValue("buttonStyle", value);
        break;
    case Setting::enableMusicTrackList:
        if (checked){
            settings.setValue("enableMusicTrackList", 1);
        }else{
            settings.setValue("enableMusicTrackList", 0);
        }
        break;
    case Setting::openWhatIsNewWindow:
        if (checked){
            settings.setValue("openWhatIsNewWindow", 1);
        }else{
            settings.setValue("openWhatIsNewWindow", 0);
        }
        break;
    case Setting::language:
        settings.setValue("language", value);
        break;
    case Setting::version:
        settings.setValue("version", value);
        break;
    case Setting::notesPath:
        path = setFolderLocation("Set Notes Folder");
        if (path.length()>1){
            settings.setValue("notesPath", path);
        }
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

// Set addon disabled or enabled
void SettingsManager::setAddonEnabled(QString addon, bool enabled){
    QSettings addonSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    addonSettings.beginGroup("Addons");

    if (enabled){
        addonSettings.setValue(addon, 1);
    }else{
        addonSettings.setValue(addon, 0);
    }

    addonSettings.endGroup();
}

// Returns if addon is enabled
bool SettingsManager::getIsAddonEnabled(QString addon){
    bool enabled;

    QSettings addonSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    addonSettings.beginGroup("Addons");

    if (addonSettings.value(addon).toInt() == 1){
        enabled = true;
    }else{
        enabled = false;
    }

    addonSettings.endGroup();

    return enabled;
}

// Returns Official Addons
QStringList SettingsManager::getOfficialAddons(){
    return officialAddons;
}
