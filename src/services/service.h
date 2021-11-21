#pragma once

#include <QObject>
#include "servicestatus.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(const QString &name, QObject *parent = nullptr);

    AUTO_PROPERTY(bool, connected)
    READ_PROPERTY(ServiceStatus*, status)

public slots:
    virtual void connectService() = 0;
    virtual void disconnectService() = 0;

protected slots:
    void updateStatus(const ServiceStatus::Type& type, const QString& message);

private:
    QString m_serviceName;

    void updateConnectionStatus();

private slots:
    void onConnectedChanged(bool connected);
};
