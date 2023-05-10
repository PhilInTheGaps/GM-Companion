#pragma once

#include <QFuture>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include "restnetworkreply.h"
#include "servicestatus.h"

class RESTServiceConnector : public QObject
{
    Q_OBJECT
public:
    RESTServiceConnector(QNetworkAccessManager &networkManager, const QLoggingCategory &loggingCategory,
                         QObject *parent = nullptr)
        : QObject(parent), m_networkManager(networkManager), m_loggingCategory(loggingCategory)
    {
    }

    virtual void grantAccess() = 0;
    virtual void disconnectService() = 0;
    [[nodiscard]] virtual bool isAccessGranted() const = 0;

    virtual QFuture<RestNetworkReply *> get(const QNetworkRequest &request) = 0;
    virtual QFuture<RestNetworkReply *> put(QNetworkRequest request, const QByteArray &data) = 0;
    virtual QFuture<RestNetworkReply *> post(QNetworkRequest request, const QByteArray &data) = 0;
    virtual QFuture<RestNetworkReply *> customRequest(const QNetworkRequest &req, const QByteArray &verb,
                                                      const QByteArray &data) = 0;

protected:
    QNetworkAccessManager &m_networkManager;
    const QLoggingCategory &m_loggingCategory;
    bool m_wasConfigured = false;

    void setStatus(ServiceStatus::Type type, const QString &message)
    {
        switch (type)
        {
        case ServiceStatus::Type::Warning:
            qCWarning(m_loggingCategory) << message;
            break;
        case ServiceStatus::Type::Error:
            qCCritical(m_loggingCategory) << message;
            break;
        default:
            qCDebug(m_loggingCategory) << message;
        }

        emit statusChanged(type, message);
    }

signals:
    void accessGranted();
    void statusChanged(const ServiceStatus::Type &type, const QString &message);
    void isConnectedChanged(const bool &connected);
};
