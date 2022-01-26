#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QLoggingCategory>
#include <QFuture>

#include "settings/settingsmanager.h"
#include "service.h"
#include "restnetworkreply.h"
#include "requestcontainer.h"

class RESTServiceConnector : public QObject
{
    Q_OBJECT
public:
    RESTServiceConnector(QNetworkAccessManager *networkManager, const QLoggingCategory &loggingCategory, QObject *parent = nullptr)
        : QObject(parent), m_networkManager(networkManager), m_loggingCategory(loggingCategory) {}

    virtual void grantAccess() = 0;
    virtual void disconnectService() = 0;
    virtual bool isAccessGranted() const = 0;

    virtual QFuture<RestNetworkReply*> get(const QNetworkRequest &request) = 0;
    virtual QFuture<RestNetworkReply*> put(QNetworkRequest request, const QByteArray &data) = 0;
    virtual QFuture<RestNetworkReply*> post(QNetworkRequest request, const QByteArray &data) = 0;
    virtual QFuture<RestNetworkReply*> customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data) = 0;

protected:
    QNetworkAccessManager *m_networkManager = nullptr;
    const QLoggingCategory &m_loggingCategory;
    bool m_wasConfigured = false;

    void setStatus(const ServiceStatus::Type& type, const QString& message)
    {
        switch (type)
        {
        case ServiceStatus::Warning:
            qCWarning(m_loggingCategory) << message;
            break;
        case ServiceStatus::Error:
            qCCritical(m_loggingCategory) << message;
            break;
        default:
            qCDebug(m_loggingCategory) << message;
        }

        emit statusChanged(type, message);
    }

signals:
    void accessGranted();
    void statusChanged(const ServiceStatus::Type& type, const QString& message);
    void isConnectedChanged(const bool& connected);
};
