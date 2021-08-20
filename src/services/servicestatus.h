#pragma once

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

    enum Type {
        Info = 0,
        Success = 1,
        Warning = 2,
        Error = 3
    };

signals:
    void typeChanged();
    void messageChanged();

private:
    int m_type = 0;
    QString m_message = "";
};
