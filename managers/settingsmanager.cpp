#include "settingsmanager.h"

#include <QTextStream>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

SettingsManager::SettingsManager()
{

}

// Returns a specific setting
QString SettingsManager::getSetting(Setting setting){
    QString settingString;

    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

    switch (setting) {
    case Setting::musicPath:
        settings.beginGroup("Paths");
        settingString = settings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString();
        settings.endGroup();
        break;
    case Setting::soundPath:
        settings.beginGroup("Paths");
        settingString = settings.value("soundPath", QDir::homePath()+"/.gm-companion/sounds").toString();
        settings.endGroup();
        break;
    case Setting::mapsPath:
        settings.beginGroup("Paths");
        settingString = settings.value("mapsPath", QDir::homePath()+"/.gm-companion/maps").toString();
        settings.endGroup();
        break;
    case Setting::resourcesPath:
        settings.beginGroup("Paths");
        settingString = settings.value("resourcesPath", QDir::homePath()+"/.gm-companion/resources").toString();
        settings.endGroup();
        break;
    case Setting::charactersPath:
        settings.beginGroup("Paths");
        settingString = settings.value("charactersPath", QDir::homePath()+"/.gm-companion/characters").toString();
        settings.endGroup();
        break;
    case Setting::notesPath:
        settings.beginGroup("Paths");
        settingString = settings.value("notesPath", QDir::homePath()+"/.gm-companion/notes").toString();
        settings.endGroup();
        break;
    case Setting::audioPath:
        settings.beginGroup("Paths");
        settingString = settings.value("audioPath", QDir::homePath()+"/.gm-companion/audio").toString();
        settings.endGroup();
        break;
    case Setting::radioPath:
        settings.beginGroup("Paths");
        settingString = settings.value("radioPath", QDir::homePath()+"/.gm-companion/radio").toString();
        settings.endGroup();
        break;
    case Setting::uiMode:
        settingString = settings.value("uiMode", "Dark").toString();
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
            settings.beginGroup("Paths");
            settings.setValue("musicPath", path);
            settings.endGroup();
        }
        break;
    case Setting::soundPath:
        path = setFolderLocation("Set Sound Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("soundPath", path);
            settings.endGroup();
        }
        break;
    case Setting::mapsPath:
        path = setFolderLocation("Set Maps Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("mapsPath", path);
            settings.endGroup();
        }
        break;
    case Setting::resourcesPath:
        path = setFolderLocation("Set Resources Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("resourcesPath", path);
            settings.endGroup();
        }
        break;
    case Setting::charactersPath:
        path = setFolderLocation("Set Characters Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("charactersPath", path);
            settings.endGroup();
        }
        break;
    case Setting::notesPath:
        path = setFolderLocation("Set Notes Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("notesPath", path);
            settings.endGroup();
        }
        break;
    case Setting::audioPath:
        path = setFolderLocation("Set Audio Projects Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("audioPath", path);
            settings.endGroup();
        }
        break;
    case Setting::radioPath:
        path = setFolderLocation("Set Radio Playlists Folder");
        if (path.length()>1){
            settings.beginGroup("Paths");
            settings.setValue("radioPath", path);
            settings.endGroup();
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

    if (addonSettings.value(addon, 0).toInt() == 1){
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

// Returns all inactive characters
QStringList SettingsManager::getInactiveCharacters()
{
    qDebug() << "Getting inactive characters ...";

    QStringList characters;

    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

    settings.beginGroup("Characters");

    int size = settings.beginReadArray("InactiveCharacters");

    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);

        characters.push_back(settings.value("filename").toString());
    }

    settings.endArray();
    settings.endGroup();

    return characters;
}

void SettingsManager::setInactiveCharacters(QStringList characters)
{
   qDebug() << "Writing inactive characters ...";

   QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

   settings.beginGroup("Characters");

   settings.beginWriteArray("InactiveCharacters");

    for (int i = 0; i < characters.size(); i++)
    {
        settings.setArrayIndex(i);

        settings.setValue("filename", characters.at(i));
    }

    settings.endArray();
    settings.endGroup();
}

// Updates the settings if something changed from a previous version
void SettingsManager::updateSettings(){
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);

    if (settings.value("version").toInt() < 320){
        qDebug() << "Updating settings file...";

        QStringList paths = {
            "musicPath", "charactersPath", "resourcesPath", "soundPath", "mapsPath", "notesPath"
        };

        for (QString path : paths){
            if (settings.value(path).isValid()){
                QString temp;
                temp = settings.value(path).toString();
                settings.remove(path);

                settings.beginGroup("Paths");
                settings.setValue(path, temp);
                settings.endGroup();
            }
        }
    }
}
