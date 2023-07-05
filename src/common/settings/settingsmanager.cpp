#include "settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include <QLoggingCategory>
#include <qt6keychain/keychain.h>

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
    auto value = instance()->get<QString>(setting, QLatin1String(), group);

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
    auto value = instance()->get<QString>(QStringLiteral("language"), QStringLiteral("default"));

    // Workaround for old settings versions
    if (value == QStringLiteral("Deutsch")) value = QStringLiteral("de");
    else if (value == QStringLiteral("English"))
        value = QStringLiteral("en");

    const auto useDefault = value == QStringLiteral("default");
    const auto isSystemLangAvailable =
        getLanguageNames().contains(QLocale::languageToString(QLocale::system().language()));
    const auto useSystemLang = useDefault && isSystemLangAvailable;

    qCDebug(gmSettings()) << "Language:" << QLocale(value).name() << QLocale::system().name();

    return useSystemLang ? QLocale::system() : QLocale(value);
}

auto SettingsManager::getLanguageString() -> QString
{
    return QLocale::languageToString(getLanguage().language());
}

auto SettingsManager::getLanguages() -> QStringList
{
    QDir dir(QStringLiteral(":/i18n"));

    dir.setFilter(QDir::Files);
    dir.setNameFilters({"*.qm"});

    QStringList languages;
    const auto availableLanguageFiles = dir.entryList();

    for (auto language : availableLanguageFiles)
    {
        auto temp = language.replace(QStringLiteral("gm-companion_"), QLatin1String())
                        .replace(QStringLiteral(".qm"), QLatin1String());

        languages.append(QLocale(temp).name());
    }

    qCDebug(gmSettings()) << "Languages found:" << languages;
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
    if (hasDefault &&
        instance()->get(QStringLiteral("connection"), QStringLiteral("default"), service) == QStringLiteral("default"))
    {
        return DEFAULT_SERVER_URL;
    }

    auto url = instance()->get(QStringLiteral("server"), hasDefault ? DEFAULT_SERVER_URL : QStringLiteral(), service);

    // Remove trailing '/'
    if (url.endsWith('/')) url.chop(1);

    return url;
}

void SettingsManager::setServerUrl(const QString &url, const QString &service)
{
    instance()->set(QStringLiteral("server"), url, service);
}

auto SettingsManager::getPassword(const QString &username, const QString &service) -> QFuture<QString>
{
    auto *job = new QKeychain::ReadPasswordJob(QStringLiteral("gm-companion.%1").arg(service), instance());
    job->setAutoDelete(false);
    job->setKey(username);

    const auto callback = [job, service](QKeychain::Job *) -> QString {
        if (job->error())
        {
            qCCritical(gmSettings) << "Could not read password:" << job->error() << job->errorString();
            job->deleteLater();
            return QLatin1String();
        }

        qCDebug(gmSettings) << "Successfully read password for service" << service;
        const auto pw = job->textData();
        job->deleteLater();

        return pw;
    };

    const auto future = AsyncFuture::observe(job, &QKeychain::ReadPasswordJob::finished)
                            .subscribe(callback,
                                       [job, service]() {
                                           qCCritical(gmSettings) << "Password job cancelled for service" << service;
                                           job->deleteLater();
                                           return QLatin1String();
                                       })
                            .future();

    job->start();
    return future;
}

void SettingsManager::setPassword(const QString &username, const QString &password, const QString &service)
{
    auto *passwordJob = new QKeychain::WritePasswordJob(QStringLiteral("gm-companion.%1").arg(service));
    passwordJob->setKey(username);
    passwordJob->setTextData(password);

    const auto callback = [passwordJob](QKeychain::Job *) {
        if (passwordJob->error())
        {
            qCCritical(gmSettings) << "Unable to save password:" << passwordJob->error() << passwordJob->errorString();
        }
        else
        {
            qCDebug(gmSettings) << "Successfully saved password.";
        }
    };

    QObject::connect(passwordJob, &QKeychain::WritePasswordJob::finished, callback);
    passwordJob->start();
}

auto SettingsManager::getDefaultPath(const QString &setting, const QString &group) -> QString
{
    if (setting.isEmpty()) return QLatin1String();

    if (group != PATHS_GROUP) return QStringLiteral("/gm-companion/%1").arg(setting);

    return QStringLiteral("%1/.gm-companion/%2").arg(QDir::homePath(), setting);
}

/// Get the ini group for the currently set cloud mode.
/// Default value is PATHS_GROUP.
auto SettingsManager::getActivePathGroup() -> QString
{
    auto cloudMode = instance()->get<QString>(QStringLiteral("cloudMode"), QStringLiteral("local"));

    if (cloudMode == QStringLiteral("GoogleDrive")) return cloudMode;

    if (cloudMode == QStringLiteral("NextCloud")) return cloudMode;

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
    const auto cloudMode = get(QStringLiteral("cloudMode"), QStringLiteral("0"));

    if (cloudMode == QStringLiteral("0"))
    {
        set(QStringLiteral("cloudMode"), QStringLiteral("local"));
    }
    else if (cloudMode == QStringLiteral("1"))
    {
        set(QStringLiteral("cloudMode"), QStringLiteral("GoogleDrive"));
    }

    rename<QString>(QStringLiteral("audioPath"), QStringLiteral("audio"), PATHS_GROUP);
    rename<QString>(QStringLiteral("charactersPath"), QStringLiteral("characters"), PATHS_GROUP);
    rename<QString>(QStringLiteral("mapsPath"), QStringLiteral("maps"), PATHS_GROUP);
    rename<QString>(QStringLiteral("musicPath"), QStringLiteral("music"), PATHS_GROUP);
    rename<QString>(QStringLiteral("notesPath"), QStringLiteral("notes"), PATHS_GROUP);
    rename<QString>(QStringLiteral("radioPath"), QStringLiteral("radio"), PATHS_GROUP);
    rename<QString>(QStringLiteral("resourcesPath"), QStringLiteral("resources"), PATHS_GROUP);
    rename<QString>(QStringLiteral("shopPath"), QStringLiteral("shops"), PATHS_GROUP);
    rename<QString>(QStringLiteral("soundPath"), QStringLiteral("sounds"), PATHS_GROUP);
}
