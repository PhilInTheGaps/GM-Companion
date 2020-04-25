#ifndef RESTSERVICECONNECTOR_H
#define RESTSERVICECONNECTOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QLoggingCategory>

#include "settings/settingsmanager.h"
#include "../service.h"

enum RequestType {
    GET,
    PUT,
    POST,
    CUSTOM
};

struct RequestContainer {
    RequestContainer(
            int id, QNetworkRequest request,
            RequestType requestType,
            QByteArray data, QByteArray verb = ""):
        requestId(id), request(request),
        requestType(requestType), data(data), verb(verb) {}
    RequestContainer() {}

    int requestId = -1;
    QNetworkRequest request;
    RequestType requestType = RequestType::GET;
    QByteArray data, verb;
};

class RESTServiceConnector : public QObject
{
    Q_OBJECT
public:
    RESTServiceConnector(QNetworkAccessManager *networkManager, const QLoggingCategory &loggingCategory, QObject *parent = nullptr)
        : QObject(parent), m_networkManager(networkManager), m_loggingCategory(loggingCategory) {}

    virtual void grantAccess() = 0;
    virtual bool isAccessGranted() const = 0;

    virtual int get(QNetworkRequest request) = 0;
    virtual void get(QNetworkRequest request, int requestId) = 0;

    virtual int put(QNetworkRequest request, QByteArray data) = 0;
    virtual void put(QNetworkRequest request, QByteArray data, int requestId) = 0;

    virtual int post(QNetworkRequest request, QByteArray data) = 0;
    virtual void post(QNetworkRequest request, QByteArray data, int requestId) = 0;

    virtual void customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data, int requestId) = 0;

    virtual int getUniqueRequestId() = 0;

protected:
    QNetworkAccessManager *m_networkManager = nullptr;
    const QLoggingCategory &m_loggingCategory;
    bool m_wasConfigured = false;

signals:
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data, QList<QNetworkReply::RawHeaderPair> headers);
    void accessGranted();
    void statusChanged(const Service::StatusType& type, const QString& message);
};

#endif // RESTSERVICECONNECTOR_H
