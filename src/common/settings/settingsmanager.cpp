#include "settingsmanager.h"
#include <QLoggingCategory>
#include <QtConcurrent/QtConcurrent>
#include <qt6keychain/keychain.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmSettings, "gm.settings")

constexpr ConstQString ADDONS_GROUP = "Addons";
constexpr ConstQString DEFAULT_SERVER_URL = "https://gm-companion.rophil.lol";

auto SettingsManager::instance() -> QPointer<SettingsManager>
{
    if (!m_instance)
    {
        m_instance = QPointer(new SettingsManager(nullptr));
        m_instance->updateSettings();
    }
    return m_instance;
}

auto SettingsManager::getPath(const QString &setting, QString group) -> QString
{
    if (group.isEmpty()) group = getActivePathGroup();
    auto value = instance()->get<QString>(setting, ""_L1, group);

    if (value.isEmpty()) value = getDefaultPath(setting, group);
    return value;
}

void SettingsManager::setPath(const QString &setting, const QString &value, QString group)
{
    if (group.isEmpty()) group = getActivePathGroup();
    instance()->set(setting, value, group);
}

auto SettingsManager::getLanguage() -> QLocale
{
    auto value = instance()->get<QString>(u"language"_s, u"default"_s);

    // Workaround for old settings versions
    if (value == "Deutsch"_L1) value = u"de"_s;
    else if (value == "English"_L1)
        value = u"en"_s;

    const auto useDefault = value == "default"_L1;
    const auto isSystemLangAvailable =
        getLanguageNames().contains(QLocale::languageToString(QLocale::system().language()));
    const auto useSystemLang = useDefault && isSystemLangAvailable;

    return useSystemLang ? QLocale::system() : QLocale(value);
}

auto SettingsManager::getLanguageBcp47() -> QString
{
    return getLanguage().bcp47Name();
}

auto SettingsManager::getLanguageString() -> QString
{
    return QLocale::languageToString(getLanguage().language());
}

auto SettingsManager::getLanguages() -> QStringList
{
    QDir dir(u":/i18n"_s);

    dir.setFilter(QDir::Files);
    dir.setNameFilters({"*.qm"});

    QStringList languages;
    const auto availableLanguageFiles = dir.entryList();

    for (auto language : availableLanguageFiles)
    {
        auto temp = language.replace("gm-companion_"_L1, ""_L1).replace(".qm"_L1, ""_L1);

        languages.append(QLocale(temp).name());
    }

    return languages;
}

auto SettingsManager::getLanguageNames() -> QStringList
{
    const auto availableLanguages = getLanguages();

    QStringList strings;
    strings.reserve(availableLanguages.length());

    for (const auto &language : availableLanguages)
    {
        strings << QLocale::languageToString(QLocale(language).language());
    }

    return strings;
}

auto SettingsManager::getServerUrl(const QString &service, bool hasDefault) -> QString
{
    if (hasDefault && instance()->get(u"connection"_s, u"default"_s, service) == "default"_L1)
    {
        return DEFAULT_SERVER_URL;
    }

    auto url = instance()->get(u"server"_s, hasDefault ? DEFAULT_SERVER_URL : u""_s, service);

    // Remove trailing '/'
    if (url.endsWith('/')) url.chop(1);

    return url;
}

void SettingsManager::setServerUrl(const QString &url, const QString &service)
{
    instance()->set(u"server"_s, url, service);
}

auto SettingsManager::getPassword(const QString &username, const QString &service) -> QString
{
    using namespace QKeychain;

    // job is deleted automatically when finished
    auto *job = new ReadPasswordJob(u"gm-companion.%1"_s.arg(service));
    job->setKey(username);

    QEventLoop loop;
    connect(job, &ReadPasswordJob::finished, &loop, &QEventLoop::quit);
    job->start();
    loop.exec();

    if (job->error())
    {
        qCCritical(gmSettings) << "Could not read password:" << job->error() << job->errorString();
        return u""_s;
    }

    qCDebug(gmSettings) << "Successfully read password for service" << service;
    return job->textData();
}

auto SettingsManager::setPassword(const QString &username, const QString &password, const QString &service) -> bool
{
    using namespace QKeychain;

    // job is deleted automatically when finished
    auto *job = new WritePasswordJob(u"gm-companion.%1"_s.arg(service));
    job->setKey(username);
    job->setTextData(password);

    QEventLoop loop;
    connect(job, &WritePasswordJob::finished, &loop, &QEventLoop::quit);
    job->start();
    loop.exec();

    if (job->error())
    {
        qCCritical(gmSettings) << "Unable to save password:" << job->error() << job->errorString();
        return false;
    }

    qCDebug(gmSettings) << "Successfully saved password.";
    return true;
}

auto SettingsManager::getDefaultPath(const QString &setting, const QString &group) -> QString
{
    if (setting.isEmpty()) return u""_s;

    if (group != PATHS_GROUP) return u"/gm-companion/%1"_s.arg(setting);

    return u"%1/.gm-companion/%2"_s.arg(QDir::homePath(), setting);
}

/// Get the ini group for the currently set cloud mode.
/// Default value is PATHS_GROUP.
auto SettingsManager::getActivePathGroup() -> QString
{
    if (auto cloudMode = instance()->get<QString>(u"cloudMode"_s, u"local"_s);
        cloudMode == "GoogleDrive"_L1 || cloudMode == "NextCloud"_L1)
        return cloudMode;

    return PATHS_GROUP;
}

/// Set addon disabled or enabled
void SettingsManager::setAddonEnabled(const QString &addon, bool enabled)
{
    if (addon.isEmpty())
    {
        qCWarning(gmSettings()) << "setAddonEnabled(): addon id is empty!";
        return;
    }

    set(addon, enabled, ADDONS_GROUP);
}

/// Returns if addon is enabled
auto SettingsManager::getIsAddonEnabled(const QString &addon) -> bool
{
    return get(addon, false, ADDONS_GROUP);
}

/// Updates the settings if something changed from a previous version
void SettingsManager::updateSettings()
{
    if (const auto cloudMode = get(u"cloudMode"_s, u"0"_s); cloudMode == "0"_L1)
    {
        set(u"cloudMode"_s, u"local"_s);
    }
    else if (cloudMode == "1"_L1)
    {
        set(u"cloudMode"_s, u"GoogleDrive"_s);
    }

    rename<QString>(u"audioPath"_s, u"audio"_s, PATHS_GROUP);
    rename<QString>(u"charactersPath"_s, u"characters"_s, PATHS_GROUP);
    rename<QString>(u"mapsPath"_s, u"maps"_s, PATHS_GROUP);
    rename<QString>(u"musicPath"_s, u"music"_s, PATHS_GROUP);
    rename<QString>(u"notesPath"_s, u"notes"_s, PATHS_GROUP);
    rename<QString>(u"radioPath"_s, u"radio"_s, PATHS_GROUP);
    rename<QString>(u"resourcesPath"_s, u"resources"_s, PATHS_GROUP);
    rename<QString>(u"shopPath"_s, u"shops"_s, PATHS_GROUP);
    rename<QString>(u"soundPath"_s, u"sounds"_s, PATHS_GROUP);
}
