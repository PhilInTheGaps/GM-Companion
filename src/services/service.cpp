#include "service.h"

Service::Service(QObject *parent) : QObject(parent)
{
    m_status = new ServiceStatus;
}

Service::~Service()
{
    m_status->deleteLater();
}

void Service::updateStatus(const Service::StatusType& type, const QString& message)
{
    m_status->setType(static_cast<int>(type));
    m_status->setMessage(message);
}
