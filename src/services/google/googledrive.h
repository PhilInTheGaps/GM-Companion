#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include <QObject>
#include <QNetworkAccessManager>

#include "googledriveconnectorlocal.h"

class GoogleDrive : public Service
{
    Q_OBJECT
public:
    static GoogleDrive *getInstance();
    ~GoogleDrive();

    void grant() { m_connector->grantAccess(); }
    bool isGranted() const { return m_connector->isAccessGranted(); }

    Q_INVOKABLE void updateConnector();

    int get(QNetworkRequest request) { return m_connector->get(request); }
    int put(QUrl url, QByteArray data = "") { return m_connector->put(QNetworkRequest(url), data); }
    int post(QNetworkRequest request, QByteArray data) { return m_connector->post(request, data); }

    int customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data);

    int getUniqueRequestId() { return m_connector->getUniqueRequestId(); }

private:
    GoogleDrive(QObject *parent = nullptr);

    static bool instanceFlag;
    static GoogleDrive *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;

private slots:
    static void onAccessGranted();
    void onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data, QList<QNetworkReply::RawHeaderPair> headers);

signals:
    void authorized();
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data, QList<QNetworkReply::RawHeaderPair> headers);
};

#endif // GOOGLEDRIVE_H
