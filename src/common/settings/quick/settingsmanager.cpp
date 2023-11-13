#include "settingsmanager.h"
#include "utils/utils.h"

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

auto Quick::SettingsManager::has(const QString &setting, const QString &group) -> bool
{
    return Common::Settings::SettingsManager::instance()->has(setting, group);
}

auto Quick::SettingsManager::getPath(const QString &path) -> QString
{
    return Common::Settings::SettingsManager::getPath(path);
}

void Quick::SettingsManager::setPath(const QString &path, const QString &value)
{
    Common::Settings::SettingsManager::setPath(path, value);
}

auto Quick::SettingsManager::getLanguageIndex() -> qsizetype
{
    auto language = Common::Settings::SettingsManager::getLanguageString();
    auto languages = getLanguageNames();
    auto index = languages.indexOf(language);

    if (index < 0) index = languages.indexOf("English"_L1);

    return index;
}

auto Quick::SettingsManager::getLanguageNames() -> QStringList
{
    return Common::Settings::SettingsManager::getLanguageNames();
}

void Quick::SettingsManager::setLanguage(const QString &language)
{
    auto languages = Common::Settings::SettingsManager::getLanguages();
    auto languageNames = getLanguageNames();
    auto index = languageNames.indexOf(language);
    auto chosenLanguage = Utils::isInBounds(languages, index) ? languages[index] : u"default"_s;

    Common::Settings::SettingsManager::instance()->set(u"language"_s, chosenLanguage);
    emit languageChanged(chosenLanguage);

    a_languageBcp47 = Common::Settings::SettingsManager::getLanguageBcp47();
}

auto Quick::SettingsManager::instance() -> SettingsManager *
{
    if (!m_instance)
    {
        m_instance = new SettingsManager(nullptr);
    }

    return m_instance;
}
