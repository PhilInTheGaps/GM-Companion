#include "quicksettingsmanager.h"
#include "utils/utils.h"

QuickSettingsManager::QuickSettingsManager(QObject *parent)
    : QObject{parent}
{
}

auto QuickSettingsManager::has(const QString &setting, const QString &group) -> bool
{
    return SettingsManager::instance()->has(setting, group);
}

auto QuickSettingsManager::getPath(const QString &path) -> QString
{
    return SettingsManager::getPath(path);
}

void QuickSettingsManager::setPath(const QString &path, const QString &value)
{
    SettingsManager::setPath(path, value);
}

auto QuickSettingsManager::getLanguageIndex() -> int
{
    auto language  = SettingsManager::getLanguageString();
    auto languages = getLanguageNames();
    auto index     = languages.indexOf(language);

    if (index < 0) index = languages.indexOf(QStringLiteral("English"));

    return index;
}

auto QuickSettingsManager::getLanguageNames() -> QStringList
{
    return SettingsManager::getLanguageNames();
}

void QuickSettingsManager::setLanguage(const QString& language)
{
    auto languages     = SettingsManager::getLanguages();
    auto languageNames = getLanguageNames();
    auto index         = languageNames.indexOf(language);
    auto chosenLanguage = Utils::isInBounds(languages, index) ? languages[index] : QStringLiteral("default");

    SettingsManager::instance()->set(QStringLiteral("language"), chosenLanguage);
}

auto QuickSettingsManager::getServerUrl(const QString &service) -> QString
{
    return SettingsManager::getServerUrl(service);
}

void QuickSettingsManager::setServerUrl(const QString &url, const QString &service)
{
    SettingsManager::setServerUrl(url, service);
}

void QuickSettingsManager::setPassword(const QString &username, const QString &password, const QString &service)
{
    SettingsManager::setPassword(username, password, service);
}
