#include "service.h"
#include "settings/settingsmanager.h"

Service::Service(const QString &name, QObject *parent)
    : QObject(parent), a_status(new ServiceStatus(this)), a_serviceName(name)
{
    connected(SettingsManager::instance()->get(QStringLiteral("connected"), false, a_serviceName));
    updateConnectionStatus();

    connect(this, &Service::connectedChanged, this, &Service::onConnectedChanged);
}

void Service::disconnect()
{
    SettingsManager::instance()->set(QStringLiteral("connected"), false, a_serviceName);
    connected(false);
}

void Service::updateStatus(ServiceStatus::Type type, const QString &message)
{
    status()->type(type);
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
    SettingsManager::SettingsManager::instance()->set(QStringLiteral("connected"), connected, a_serviceName);
    updateConnectionStatus();
}
