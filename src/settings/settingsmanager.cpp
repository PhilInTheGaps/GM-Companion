#include "settingsmanager.h"
#include "logging.h"
#include "services/services.h"

#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <utility>

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

auto SettingsManager::getInstance()->SettingsManager *
{
    if (!instanceFlag)
    {
        single       = new SettingsManager;
        instanceFlag = true;
        single->updateSettings();
    }
    return single;
}

SettingsManager::~SettingsManager()
{
    instanceFlag = false;
    m_settings->deleteLater();
}

auto SettingsManager::getSetting(const QString& setting, const QString& defaultValue, QString group)->QString
{
    if (group.isEmpty()) group = DEFAULT_GROUP;

    getInstance()->m_settings->beginGroup(group);
    auto value = getInstance()->m_settings->value(setting, defaultValue).toString();
    getInstance()->m_settings->endGroup();
    return value;
}

void SettingsManager::setSetting(const QString& setting, const QString& value, const QString& group)
{
    getInstance()->m_settings->beginGroup(group);
    getInstance()->m_settings->setValue(setting, value);
    getInstance()->m_settings->endGroup();
}

void SettingsManager::setSetting(QString setting, int value, QString group)
{
    setSetting(std::move(setting), QString::number(value), std::move(group));
}

auto SettingsManager::getPath(const QString& setting, QString group)->QString
{
    if (group.isEmpty()) group = SettingsManager::getInstance()->getActivePathGroup();
    auto value = SettingsManager::getInstance()->getSetting(setting, "", group);

    if (value.isEmpty()) value = SettingsManager::getInstance()->getDefaultPath(setting, group);
    return value;
}

void SettingsManager::setPath(QString setting, QString value, QString group)
{
    if (group.isEmpty()) group = SettingsManager::getInstance()->getActivePathGroup();
    setSetting(std::move(setting), std::move(value), PATHS_GROUP);
}

auto SettingsManager::getLanguage()->QString
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
    setSetting("language", std::move(language));
}

auto SettingsManager::getServerUrl()->QString
{
    if (SettingsManager::getSetting("serviceConnection") == "default") {
        return DEFAULT_SERVER_URL;
    }

    return SettingsManager::getSetting("serverUrl", DEFAULT_SERVER_URL);
}

void SettingsManager::setServerUrl(QString url)
{
    setSetting("serverUrl", std::move(url));
}

auto SettingsManager::getPassword(const QString& username, const QString& service)->QString
{
    QKeychain::ReadPasswordJob passwordJob("gm-companion." + service);

    passwordJob.setAutoDelete(false);
    passwordJob.setKey(username);
    QEventLoop passwordLoop;
    QEventLoop::connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
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

void SettingsManager::setPassword(const QString& username, const QString& password, const QString& service)
{
    QKeychain::WritePasswordJob passwordJob("gm-companion." + service);

    passwordJob.setAutoDelete(false);
    passwordJob.setKey(username);
    passwordJob.setTextData(password);
    QEventLoop passwordLoop;
    QEventLoop::connect(&passwordJob, SIGNAL(finished(QKeychain::Job*)),
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

auto SettingsManager::getDefaultPath(const QString& setting, const QString& group)->QString
{
    if (setting.isEmpty()) return "";

    if (group != PATHS_GROUP) return "/gm-companion/" + setting;

    return QDir::homePath() + "/.gm-companion/" + setting;
}

auto SettingsManager::getActivePathGroup()->QString
{
    auto cloudMode = getSetting("cloudMode", "local");

    if (cloudMode == "GoogleDrive") return "Google";

    if (cloudMode == "NextCloud") return "NextCloud";

    return PATHS_GROUP;
}

// Set addon disabled or enabled
void SettingsManager::setAddonEnabled(const QString& addon, bool enabled)
{
    m_settings->beginGroup("Addons");
    m_settings->setValue(addon, enabled);
    m_settings->endGroup();
}

// Returns if addon is enabled
auto SettingsManager::getIsAddonEnabled(const QString& addon)->bool
{
    m_settings->beginGroup("Addons");
    bool enabled = m_settings->value(addon, true).toBool();
    m_settings->endGroup();

    return enabled;
}

// Returns Official Addons
auto SettingsManager::getOfficialAddons()->QStringList
{
    return m_officialAddons;
}

// Updates the settings if something changed from a previous version
void SettingsManager::updateSettings()
{
    if (getSetting("cloudMode") == "0")
    {
        setSetting("cloudMode", "local");
    }
    else if (getSetting("cloudMode") == "1")
    {
        setSetting("cloudMode", "GoogleDrive");
    }

    renameSetting("audioPath",      "audio",      PATHS_GROUP);
    renameSetting("charactersPath", "characters", PATHS_GROUP);
    renameSetting("mapsPath",       "maps",       PATHS_GROUP);
    renameSetting("musicPath",      "music",      PATHS_GROUP);
    renameSetting("notesPath",      "notes",      PATHS_GROUP);
    renameSetting("radioPath",      "radio",      PATHS_GROUP);
    renameSetting("resourcesPath",  "resources",  PATHS_GROUP);
    renameSetting("shopPath",       "shops",      PATHS_GROUP);
    renameSetting("soundPath",      "sounds",     PATHS_GROUP);
}

void SettingsManager::renameSetting(const QString& currentName, QString newName, const QString& group)
{
    auto value = getSetting(currentName, "", group);

    if (!value.isEmpty()) setSetting(std::move(newName), value, group);
    removeSetting(currentName, group);
}

void SettingsManager::removeSetting(const QString& setting, const QString& group)
{
    m_settings->beginGroup(group);
    m_settings->remove(setting);
    m_settings->endGroup();
}
