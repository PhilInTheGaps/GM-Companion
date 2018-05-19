#include "settingsmanager.h"

#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

SettingsManager::SettingsManager()
{
    settings = new QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
}

// Returns a specific setting
QString SettingsManager::getSetting(Setting setting)
{
    QString settingString;

    switch (setting) {
    case Setting::musicPath:
        settings->beginGroup("Paths");
        settingString = settings->value("musicPath", QDir::homePath() + "/.gm-companion/music").toString();
        settings->endGroup();
        break;

    case Setting::soundPath:
        settings->beginGroup("Paths");
        settingString = settings->value("soundPath", QDir::homePath() + "/.gm-companion/sounds").toString();
        settings->endGroup();
        break;

    case Setting::mapsPath:
        settings->beginGroup("Paths");
        settingString = settings->value("mapsPath", QDir::homePath() + "/.gm-companion/maps").toString();
        settings->endGroup();
        break;

    case Setting::resourcesPath:
        settings->beginGroup("Paths");
        settingString = settings->value("resourcesPath", QDir::homePath() + "/.gm-companion/resources").toString();
        settings->endGroup();
        break;

    case Setting::charactersPath:
        settings->beginGroup("Paths");
        settingString = settings->value("charactersPath", QDir::homePath() + "/.gm-companion/characters").toString();
        settings->endGroup();
        break;

    case Setting::notesPath:
        settings->beginGroup("Paths");
        settingString = settings->value("notesPath", QDir::homePath() + "/.gm-companion/notes").toString();
        settings->endGroup();
        break;

    case Setting::audioPath:
        settings->beginGroup("Paths");
        settingString = settings->value("audioPath", QDir::homePath() + "/.gm-companion/audio").toString();
        settings->endGroup();
        break;

    case Setting::radioPath:
        settings->beginGroup("Paths");
        settingString = settings->value("radioPath", QDir::homePath() + "/.gm-companion/radio").toString();
        settings->endGroup();
        break;

    case Setting::shopPath:
        settings->beginGroup("Paths");
        settingString = settings->value("shopPath", QDir::homePath() + "/.gm-companion/shop").toString();
        settings->endGroup();
        break;

    case Setting::uiMode:
        settingString = settings->value("uiMode", "Bright").toString();
        break;

    case Setting::openWhatIsNewWindow:
        settingString = settings->value("openWhatIsNewWindow", 1).toInt();
        break;

    case Setting::language:
        settingString = settings->value("language", "en").toString();
        break;

    case Setting::version:
        settingString = QString::number(settings->value("version", 0).toInt());
        break;

    default:
        settingString = "";
        break;
    }
    return settingString;
}

// Sets a specific setting
void SettingsManager::setSetting(Setting setting, int checked, QString value)
{
    switch (setting) {
    case Setting::musicPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("musicPath", value);
            settings->endGroup();
        }
        break;

    case Setting::soundPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("soundPath", value);
            settings->endGroup();
        }
        break;

    case Setting::mapsPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("mapsPath", value);
            settings->endGroup();
        }
        break;

    case Setting::resourcesPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("resourcesPath", value);
            settings->endGroup();
        }
        break;

    case Setting::shopPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("shopPath", value);
            settings->endGroup();
        }
        break;

    case Setting::charactersPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("charactersPath", value);
            settings->endGroup();
        }
        break;

    case Setting::notesPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("notesPath", value);
            settings->endGroup();
        }
        break;

    case Setting::audioPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("audioPath", value);
            settings->endGroup();
        }
        break;

    case Setting::radioPath:

        if (value.length() > 1) {
            settings->beginGroup("Paths");
            settings->setValue("radioPath", value);
            settings->endGroup();
        }
        break;

    case Setting::uiMode:
    {
        settings->setValue("uiMode", value);
        break;
    }

    case Setting::openWhatIsNewWindow:

        if (checked) settings->setValue("openWhatIsNewWindow", 1);
        else settings->setValue("openWhatIsNewWindow", 0);
        break;

    case Setting::language:
        settings->setValue("language", value);
        break;

    case Setting::version:
        settings->setValue("version", value);
        break;

    default:
        break;
    }
}

// Set addon disabled or enabled
void SettingsManager::setAddonEnabled(QString addon, bool enabled)
{
    settings->beginGroup("Addons");
    settings->setValue(addon, enabled);
    settings->endGroup();
}

// Returns if addon is enabled
bool SettingsManager::getIsAddonEnabled(QString addon)
{
    settings->beginGroup("Addons");
    bool enabled = true;

    if (settings->value(addon, true).toBool() == false) enabled = false;
    settings->endGroup();

    return enabled;
}

// Returns Official Addons
QStringList SettingsManager::getOfficialAddons()
{
    return officialAddons;
}

// Returns all inactive characters
QStringList SettingsManager::getInactiveCharacters()
{
    qDebug() << "Getting inactive characters ...";

    QStringList characters;

    settings->beginGroup("Characters");

    int size = settings->beginReadArray("InactiveCharacters");

    for (int i = 0; i < size; i++)
    {
        settings->setArrayIndex(i);

        characters.push_back(settings->value("filename").toString());
    }

    settings->endArray();
    settings->endGroup();

    return characters;
}

void SettingsManager::setInactiveCharacters(QStringList characters)
{
    qDebug() << "Writing inactive characters ...";

    settings->beginGroup("Characters");

    settings->beginWriteArray("InactiveCharacters");

    for (int i = 0; i < characters.size(); i++)
    {
        settings->setArrayIndex(i);

        settings->setValue("filename", characters.at(i));
    }

    settings->endArray();
    settings->endGroup();
}

// Updates the settings if something changed from a previous version
void SettingsManager::updateSettings()
{
    if (settings->value("version").toInt() < 320) // Last major settings change
    {                                             // was in Beta 3.2
        qDebug() << "Updating settings file...";

        QStringList paths = {
            "musicPath", "charactersPath", "resourcesPath", "soundPath", "mapsPath", "notesPath", "shopPath"
        };

        for (QString path : paths) {
            if (settings->value(path).isValid()) {
                QString temp;
                temp = settings->value(path).toString();
                settings->remove(path);

                settings->beginGroup("Paths");
                settings->setValue(path, temp);
                settings->endGroup();
            }
        }
    }
}
