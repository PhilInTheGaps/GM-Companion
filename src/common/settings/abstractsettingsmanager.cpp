#include "abstractsettingsmanager.h"

using namespace Common::Settings;

auto AbstractSettingsManager::has(QAnyStringView setting, QAnyStringView group) -> bool
{
    m_settings.beginGroup(group);
    const auto result = m_settings.contains(setting);
    m_settings.endGroup();

    return result;
}

void AbstractSettingsManager::forceSync()
{
    m_settings.sync();
}

void AbstractSettingsManager::remove(QAnyStringView setting, QAnyStringView group)
{
    m_settings.beginGroup(group);
    m_settings.remove(setting);
    m_settings.endGroup();
}
