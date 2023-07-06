#include "service.h"
#include "settings/settingsmanager.h"

using namespace Qt::Literals::StringLiterals;

Service::Service(const QString &name, QObject *parent)
    : QObject(parent), a_status(new ServiceStatus(this)), a_serviceName(name)
{
    connected(SettingsManager::instance()->get(u"connected"_s, false, a_serviceName));
    updateConnectionStatus();

    connect(this, &Service::connectedChanged, this, &Service::onConnectedChanged);
}

void Service::disconnect()
{
    SettingsManager::instance()->set(u"connected"_s, false, a_serviceName);
    connected(false);
}

void Service::updateStatus(ServiceStatus::Type type, const QString &message)
{
    a_status->type(type);
    a_status->message(message);
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
    SettingsManager::SettingsManager::instance()->set(u"connected"_s, connected, a_serviceName);
    updateConnectionStatus();
}
