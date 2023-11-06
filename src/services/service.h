#pragma once

#include "servicestatus.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Services
{
class Service : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit Service(const QString &name, QObject *parent = nullptr);

    AUTO_PROPERTY_VAL2(bool, connected, false)
    READ_PROPERTY(Services::Status *, status)
    AUTO_PROPERTY(QString, serviceName)

public slots:
    virtual void connectService() = 0;
    virtual void disconnectService() = 0;

protected slots:
    void updateStatus(Status::Type type, const QString &message);

private:
    void updateConnectionStatus();

private slots:
    void onConnectedChanged(bool connected);
};
} // namespace Services
