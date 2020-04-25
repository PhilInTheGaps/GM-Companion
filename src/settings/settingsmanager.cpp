#include "settingsmanager.h"
#include "logging.h"
#include "services/services.h"
#include <QCoreApplication>

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

void SettingsManager::setSetting(const QString& setting, const int& value, const QString& group)
{
    setSetting(setting, QString::number(value), group);
}

auto SettingsManager::getPath(const QString& setting, QString group)->QString
{
    if (group.isEmpty()) group = SettingsManager::getActivePathGroup();
    auto value = SettingsManager::getSetting(setting, "", group);

    if (value.isEmpty()) value = SettingsManager::getDefaultPath(setting, group);
    return value;
}

void SettingsManager::setPath(const QString& setting, const QString& value, QString group)
{
    if (group.isEmpty()) group = SettingsManager::getActivePathGroup();
    setSetting(setting, value, group);
}

auto SettingsManager::getLanguage()->QLocale
{
    auto value = SettingsManager::getSetting("language", "default");

    // Workaround for old settings versions
    if (value == "Deutsch") value = "de";
    else if (value == "English") value = "en";

    qCDebug(gmSettings()) << "Language raw:" << value << "locale:" << QLocale(value);

    auto useSystemLang = value == "default" && getLanguageNames().contains(QLocale::languageToString(QLocale::system().language()));

    qCDebug(gmSettings()) << "LANG NAME" << QLocale(value).name() << QLocale::system().name();

    return useSystemLang ? QLocale::system() : QLocale(value);
}

auto SettingsManager::getLanguageString()->QString
{
    return QLocale::languageToString(getLanguage().language());
}

auto SettingsManager::getLanguageIndex()->int
{
    auto language  = getLanguageString();
    auto languages = getLanguageNames();
    auto index     = languages.indexOf(language);

    qCDebug(gmSettings()) << "Index:" << index << "language:" << language;

    if (index < 0) index = languages.indexOf("English");

    qCDebug(gmSettings()) << " - Index:" << index << "language:" << language;

    return index;
}

auto SettingsManager::getLanguages()->QStringList
{
    QDir dir(":/translations");

    dir.setFilter(QDir::Files);
    dir.setNameFilters({ "*.qm" });

    QStringList languages;

    for (auto language : dir.entryList())
    {
        auto temp = language.replace("gm-companion_", "").replace(".qm", "");
        languages.append(QLocale(temp).name());
    }
    qCDebug(gmSettings()) << "Languages found:" << languages;

    return languages;
}

auto SettingsManager::getLanguageNames()->QStringList
{
    QStringList strings;

    for (auto language : getLanguages())
    {
        strings.append(QLocale::languageToString(QLocale(language).language()));
    }

    qCDebug(gmSettings()) << "Languages names found:" << strings;

    return strings;
}

void SettingsManager::setLanguage(const QString& language)
{
    auto languages     = getLanguages();
    auto languageNames = getLanguageNames();
    auto index         = languageNames.indexOf(language);

    if (index > -1) setSetting("language", languages[index]);
    else setSetting("language", "default");
}

auto SettingsManager::getServerUrl(const QString& service, const bool& hasDefault)->QString
{
    auto connection = SettingsManager::getSetting("connection", "default", service);

    if (hasDefault && (connection == "default"))
    {
        return DEFAULT_SERVER_URL;
    }

    auto url = SettingsManager::getSetting("server", hasDefault ? DEFAULT_SERVER_URL : "", service);

    // Remove trailing '/'
    if (url.endsWith("/")) url.chop(1);

    return url;
}

void SettingsManager::setServerUrl(const QString& url, const QString& service)
{
    setSetting("server", url, service);
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

auto SettingsManager::isUpdateCheckEnabled()->bool
{
    auto defaultValue = true;

#ifdef NO_UPDATE_CHECK
    defaultValue = false;
#endif // ifdef NO_UPDATE_CHECK

    return getBoolSetting("checkForUpdates", defaultValue, "Updates");
}

auto SettingsManager::getBoolSetting(const QString& setting, bool defaultValue, const QString& group)->bool
{
    getInstance()->m_settings->beginGroup(group);
    auto isTrue = getInstance()->m_settings->value(setting, defaultValue).toBool();
    getInstance()->m_settings->endGroup();
    return isTrue;
}

auto SettingsManager::getDefaultPath(const QString& setting, const QString& group)->QString
{
    if (setting.isEmpty()) return "";

    if (group != PATHS_GROUP) return "/gm-companion/" + setting;

    return QDir::homePath() + "/.gm-companion/" + setting;
}

/**
 * @brief Get the ini group for the currently set cloud mode.
 * Default value is PATHS_GROUP.
 */
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

void SettingsManager::renameSetting(const QString& currentName, const QString& newName, const QString& group)
{
    auto value = getSetting(currentName, "", group);

    if (!value.isEmpty()) setSetting(newName, value, group);
    removeSetting(currentName, group);
}

void SettingsManager::removeSetting(const QString& setting, const QString& group)
{
    m_settings->beginGroup(group);
    m_settings->remove(setting);
    m_settings->endGroup();
}
