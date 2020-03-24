#include "settingsmanager.h"
#include "logging.h"
#include "services/services.h"

#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#ifdef Q_OS_WIN
# include <keychain.h>
#else // ifdef Q_OS_WIN
# include <qt5keychain/keychain.h>
#endif // ifdef Q_OS_WIN


SettingsManager::SettingsManager()
{
    settings = new QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
}

// Returns a specific setting
QString SettingsManager::getSetting(Setting setting, QString value)
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

    case Setting::language:
    {
        settingString = settings->value("language", "default").toString();

        if (settingString == "default")
        {
            settingString = QLocale::languageToString(QLocale::system().language());

            if (settingString == "German") settingString = "Deutsch";
        }

        break;
    }

    case Setting::version:
        settingString = QString::number(settings->value("version", 0).toInt());
        break;

    case Setting::spotifyUsername:
        settings->beginGroup("Spotify");
        settingString = settings->value("spotifyUsername", "").toString();
        settings->endGroup();
        break;

    case Setting::spotifyPassword:
    {
        QKeychain::ReadPasswordJob passwordJob("gm-companion.spotify");
        passwordJob.setAutoDelete(false);
        passwordJob.setKey(getSetting(Setting::spotifyUsername));
        QEventLoop passwordLoop;
        passwordLoop.connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
                             &passwordLoop, SLOT(quit()));
        passwordJob.start();
        passwordLoop.exec();

        if (passwordJob.error()) {
            settingString = "";
            qCCritical(gmSettings) << "Could not read spotify password:" << passwordJob.error() << passwordJob.errorString();
        } else {
            settingString = passwordJob.textData();
            qCDebug(gmSettings) << "Successfully read spotify password.";
        }
        break;
    }

    case Setting::spotifyID:
        settings->beginGroup("Spotify");
        settingString = settings->value("spotifyID", "").toString();
        settings->endGroup();
        break;

    case Setting::spotifySecret:
        settings->beginGroup("Spotify");
        settingString = settings->value("spotifySecret", "").toString();
        settings->endGroup();
        break;

    case Setting::showToolNames:
        settingString = settings->value("showToolNames", "0").toString();
        break;

    case Setting::classicIcons:
        settingString = settings->value("classicIcons", "0").toString();
        break;

    case Setting::googleID:
        settings->beginGroup("Google");
        settingString = settings->value("googleID", "").toString();
        settings->endGroup();
        break;

    case Setting::googleSecret:
        settings->beginGroup("Google");
        settingString = settings->value("googleSecret", "").toString();
        settings->endGroup();
        break;

    case Setting::googleConnect:
        settings->beginGroup("Google");
        settingString = QString::number(settings->value("googleConnect", "").toInt());
        settings->endGroup();
        break;

    case Setting::googlePath:
        settings->beginGroup("Google");
        settingString = settings->value(value).toString();
        settings->endGroup();
        break;

    case Setting::cloudMode:
        settingString = QString::number(settings->value("cloudMode", 0).toInt());
        break;

    case Setting::serverUrl:

        if (getSetting(Setting::serviceConnection) == "default")
        {
            settingString = DEFAULT_SERVER_URL;
        }
        else
        {
            settingString = settings->value("serverUrl", DEFAULT_SERVER_URL).toString();
        }

        break;

    case Setting::serviceConnection:
        settingString = settings->value("serviceConnection", "default").toString();
        break;

    default:
        settingString = "";
        break;
    }
    return settingString;
}

// Sets a specific setting
void SettingsManager::setSetting(Setting setting, int checked, QString value, QString value2)
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

    case Setting::spotifyUsername:

        if (value.length() > 1) {
            settings->beginGroup("Spotify");
            settings->setValue("spotifyUsername", value);
            settings->endGroup();
        }
        break;

    case Setting::spotifyPassword:
    {
        QKeychain::WritePasswordJob passwordJob("gm-companion.spotify");
        passwordJob.setAutoDelete(false);
        passwordJob.setKey(getSetting(Setting::spotifyUsername));
        passwordJob.setTextData(value);
        QEventLoop passwordLoop;
        passwordLoop.connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
                             &passwordLoop, SLOT(quit()));
        passwordJob.start();
        passwordLoop.exec();

        if (passwordJob.error()) {
            qCCritical(gmSettings) << "Unable to save spotify password:" << passwordJob.error() << passwordJob.errorString();
        }
        else
        {
            qCDebug(gmSettings) << "Successfully saved spotify password.";
        }
        break;
    }

    case Setting::spotifyID:

        if (value.length() > 1) {
            settings->beginGroup("Spotify");
            settings->setValue("spotifyID", value);
            settings->endGroup();
        }
        break;

    case Setting::spotifySecret:

        if (value.length() > 1) {
            settings->beginGroup("Spotify");
            settings->setValue("spotifySecret", value);
            settings->endGroup();
        }
        break;

    case Setting::showToolNames:
        settings->setValue("showToolNames", checked);
        break;

    case Setting::classicIcons:
        settings->setValue("classicIcons", checked);
        break;

    case Setting::googleID:

        if (value.length() > 1) {
            settings->beginGroup("Google");
            settings->setValue("googleID", value);
            settings->endGroup();
        }
        break;

    case Setting::googleSecret:

        if (value.length() > 1) {
            settings->beginGroup("Google");
            settings->setValue("googleSecret", value);
            settings->endGroup();
        }
        break;

    case Setting::googleConnect:
        settings->beginGroup("Google");
        settings->setValue("googleConnect", checked);
        settings->endGroup();
        break;

    case Setting::googlePath:
        settings->beginGroup("Google");
        settings->setValue(value, value2);
        settings->endGroup();
        break;

    case Setting::cloudMode:
        settings->setValue("cloudMode", value);
        break;

    case Setting::serverUrl:
        settings->setValue("serverUrl", value);
        break;

    case Setting::serviceConnection:
        settings->setValue("serviceConnection", value);
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
