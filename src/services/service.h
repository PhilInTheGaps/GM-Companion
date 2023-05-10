#pragma once

#include "servicestatus.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(const QString &name, QObject *parent = nullptr);

    AUTO_PROPERTY_VAL2(bool, connected, false)
    READ_PROPERTY(ServiceStatus *, status)
    AUTO_PROPERTY(QString, serviceName)

public slots:
    virtual void connectService() = 0;
    virtual void disconnectService() = 0;

protected:
    void disconnect();

protected slots:
    void updateStatus(ServiceStatus::Type type, const QString &message);

private:
    void updateConnectionStatus();

private slots:
    void onConnectedChanged(bool connected);
};
