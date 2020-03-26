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

bool SettingsManager::instanceFlag       = false;
SettingsManager *SettingsManager::single = nullptr;

SettingsManager::SettingsManager()
{
    m_settings = new QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
}

SettingsManager * SettingsManager::getInstance()
{
    if (!instanceFlag)
    {
        single       = new SettingsManager;
        instanceFlag = true;
    }
    return single;
}

SettingsManager::~SettingsManager()
{
    instanceFlag = false;
    m_settings->deleteLater();
}

QString SettingsManager::getSetting(QString setting, QString defaultValue, QString group)
{
    getInstance()->m_settings->beginGroup(group);
    auto value = getInstance()->m_settings->value(setting, defaultValue).toString();
    getInstance()->m_settings->endGroup();
    return value;
}

void SettingsManager::setSetting(QString setting, QString value, QString group)
{
    getInstance()->m_settings->beginGroup(group);
    getInstance()->m_settings->setValue(setting, value);
    getInstance()->m_settings->endGroup();
}

void SettingsManager::setSetting(QString setting, int value, QString group)
{
    setSetting(setting, QString::number(value), group);
}

QString SettingsManager::getPath(QString setting)
{
    auto value = SettingsManager::getInstance()->getSetting(setting, "", PATHS_GROUP);

    if (value.isEmpty()) value = SettingsManager::getInstance()->getDefaultPath(setting);
    return value;
}

void SettingsManager::setPath(QString setting, QString value)
{
    setSetting(setting, value, PATHS_GROUP);
}

QString SettingsManager::getLanguage()
{
    auto value = SettingsManager::getSetting("language", "default");

    if (value == "default")
    {
        value = QLocale::languageToString(QLocale::system().language());

        if (value == "German") return "Deutsch";
    }

    return value;
}

void SettingsManager::setLanguage(QString language)
{
    setSetting("language", language);
}

QString SettingsManager::getServerUrl()
{
    if (SettingsManager::getSetting("serviceConnection") == "default") {
        return DEFAULT_SERVER_URL;
    }

    return SettingsManager::getSetting("serverUrl", DEFAULT_SERVER_URL);
}

void SettingsManager::setServerUrl(QString url)
{
    setSetting("serverUrl", url);
}

QString SettingsManager::getPassword(QString username, QString service)
{
    QKeychain::ReadPasswordJob passwordJob("gm-companion." + service);

    passwordJob.setAutoDelete(false);
    passwordJob.setKey(username);
    QEventLoop passwordLoop;
    passwordLoop.connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
                         &passwordLoop, SLOT(quit()));
    passwordJob.start();
    passwordLoop.exec();

    if (passwordJob.error()) {
        qCCritical(gmSettings) << "Could not read password:" << passwordJob.error() << passwordJob.errorString();
        return "";
    }

    qCDebug(gmSettings) << "Successfully read password.";
    return passwordJob.textData();
}

void SettingsManager::setPassword(QString username, QString password, QString service)
{
    QKeychain::WritePasswordJob passwordJob("gm-companion." + service);

    passwordJob.setAutoDelete(false);
    passwordJob.setKey(username);
    passwordJob.setTextData(password);
    QEventLoop passwordLoop;
    passwordLoop.connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
                         &passwordLoop, SLOT(quit()));
    passwordJob.start();
    passwordLoop.exec();

    if (passwordJob.error()) {
        qCCritical(gmSettings) << "Unable to save password:" << passwordJob.error() << passwordJob.errorString();
    }
    else
    {
        qCDebug(gmSettings) << "Successfully saved password.";
    }
}

QString SettingsManager::getDefaultPath(QString setting)
{
    if (setting.isEmpty()) return "";

    return QDir::homePath() + "/.gm-companion/" + setting;
}

// Set addon disabled or enabled
void SettingsManager::setAddonEnabled(QString addon, bool enabled)
{
    m_settings->beginGroup("Addons");
    m_settings->setValue(addon, enabled);
    m_settings->endGroup();
}

// Returns if addon is enabled
bool SettingsManager::getIsAddonEnabled(QString addon)
{
    m_settings->beginGroup("Addons");
    bool enabled = true;

    if (m_settings->value(addon, true).toBool() == false) enabled = false;
    m_settings->endGroup();

    return enabled;
}

// Returns Official Addons
QStringList SettingsManager::getOfficialAddons()
{
    return m_officialAddons;
}

// Returns all inactive characters
QStringList SettingsManager::getInactiveCharacters()
{
    qDebug() << "Getting inactive characters ...";

    QStringList characters;

    m_settings->beginGroup("Characters");

    int size = m_settings->beginReadArray("InactiveCharacters");

    for (int i = 0; i < size; i++)
    {
        m_settings->setArrayIndex(i);

        characters.push_back(m_settings->value("filename").toString());
    }

    m_settings->endArray();
    m_settings->endGroup();

    return characters;
}

void SettingsManager::setInactiveCharacters(QStringList characters)
{
    qDebug() << "Writing inactive characters ...";

    m_settings->beginGroup("Characters");

    m_settings->beginWriteArray("InactiveCharacters");

    for (int i = 0; i < characters.size(); i++)
    {
        m_settings->setArrayIndex(i);

        m_settings->setValue("filename", characters.at(i));
    }

    m_settings->endArray();
    m_settings->endGroup();
}

// Updates the settings if something changed from a previous version
void SettingsManager::updateSettings()
{
    if (m_settings->value("version").toInt() < 320) // Last major settings
                                                    // change
    {                                               // was in Beta 3.2
        qDebug() << "Updating settings file...";

        QStringList paths = {
            "musicPath", "charactersPath", "resourcesPath", "soundPath", "mapsPath", "notesPath", "shopPath"
        };

        for (QString path : paths) {
            if (m_settings->value(path).isValid()) {
                QString temp;
                temp = m_settings->value(path).toString();
                m_settings->remove(path);

                m_settings->beginGroup("Paths");
                m_settings->setValue(path, temp);
                m_settings->endGroup();
            }
        }
    }
}
