#include "abstractsettingsmanager.h"

AbstractSettingsManager::AbstractSettingsManager(QObject *parent)
    : QObject{parent}
{

}

auto AbstractSettingsManager::has(const QString &setting, const QString &group) -> bool
{
    m_settings.beginGroup(group);
    const auto result = m_settings.contains(setting);
    m_settings.endGroup();

    return result;
}

void AbstractSettingsManager::remove(const QString &setting, const QString &group)
{
    m_settings.beginGroup(group);
    m_settings.remove(setting);
    m_settings.endGroup();
}
