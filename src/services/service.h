#pragma once

#include <QObject>
#include "servicestatus.h"

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QString name, QObject *parent = nullptr);
    ~Service();

    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    bool connected() const { return m_connected; }
    void setConnected(const bool& connected);

    Q_PROPERTY(ServiceStatus* status READ status NOTIFY statusChanged)
    ServiceStatus* status() const { return m_status; }

public slots:
    virtual void connectService() = 0;
    virtual void disconnectService() = 0;

signals:
    void statusChanged();
    void connectedChanged();

protected slots:
    void updateStatus(const ServiceStatus::Type& type, const QString& message);

protected:
    bool m_connected = false;

private:
    ServiceStatus *m_status = nullptr;
    QString m_serviceName;

    void updateConnectionStatus();
};
