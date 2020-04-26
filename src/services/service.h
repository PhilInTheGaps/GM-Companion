#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

class ServiceStatus : public QObject
{
    Q_OBJECT
public:
    ServiceStatus() {}
    virtual ~ServiceStatus() {}

    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    int type() const { return m_type; }
    void setType(const int& type) { m_type = type; emit typeChanged(); }

    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    QString message() const { return m_message; }
    void setMessage(const QString& message) { m_message = message; emit messageChanged(); }

signals:
    void typeChanged();
    void messageChanged();

private:
    int m_type = 0;
    QString m_message = "";
};

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

    enum StatusType {
        Info = 0,
        Success = 1,
        Warning = 2,
        Error = 3
    };

public slots:
    virtual void connectService() = 0;
    virtual void disconnectService() = 0;

signals:
    void statusChanged();
    void connectedChanged();

protected slots:
    void updateStatus(const Service::StatusType& type, const QString& message);

protected:
    bool m_connected = false;

private:
    ServiceStatus *m_status = nullptr;
    QString m_serviceName;

    void updateConnectionStatus();
};

#endif // SERVICE_H
