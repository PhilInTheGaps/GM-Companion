#include "service.h"
#include "settings/settingsmanager.h"

Service::Service(const QString &name, QObject *parent)
    : QObject(parent), a_connected(false),
      a_status(new ServiceStatus(this)), m_serviceName(name)
{
    connected(SettingsManager::getBoolSetting("connected", false, m_serviceName));
    updateConnectionStatus();

    connect(this, &Service::connectedChanged, this, &Service::onConnectedChanged);
}

void Service::updateStatus(const ServiceStatus::Type& type, const QString& message)
{
    status()->type(static_cast<int>(type));
    status()->message(message);
}

void Service::updateConnectionStatus()
{
    if (connected())
    {
        updateStatus(ServiceStatus::Type::Success, tr("Connected"));
    }
    else
    {
        updateStatus(ServiceStatus::Type::Info, tr("Not connected"));
    }
}

void Service::onConnectedChanged(bool connected)
{
    SettingsManager::setSetting("connected", connected, m_serviceName);
    updateConnectionStatus();
}
