#include "service.h"
#include "settings/settingsmanager.h"

Service::Service(QString name, QObject *parent)
    : QObject(parent), m_serviceName(name)
{
    m_status = new ServiceStatus;
    m_connected = SettingsManager::getBoolSetting("connected", false, m_serviceName);
    updateConnectionStatus();
}

Service::~Service()
{
    m_status->deleteLater();
}

void Service::updateStatus(const ServiceStatus::Type& type, const QString& message)
{
    m_status->setType(static_cast<int>(type));
    m_status->setMessage(message);
}

void Service::updateConnectionStatus()
{
    if (m_connected)
    {
        updateStatus(ServiceStatus::Type::Success, tr("Connected"));
    }
    else
    {
        updateStatus(ServiceStatus::Type::Info, tr("Not connected"));
    }
}

void Service::setConnected(const bool& connected)
{
    m_connected = connected;
    SettingsManager::setSetting("connected", connected, m_serviceName);

    updateConnectionStatus();

    emit connectedChanged();
}
