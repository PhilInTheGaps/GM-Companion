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

    int get(QNetworkRequest request) { return m_connector->get(request); }
    int put(QUrl url, QByteArray data = "") { return m_connector->put(QNetworkRequest(url), data); }
    int post(QNetworkRequest request, QByteArray data) { return m_connector->post(request, data); }

    int customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data);
    int getUniqueRequestId() { return m_connector->getUniqueRequestId(); }

    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    QString clientId() const { return m_clientId; }
    void setClientId(const QString& clientId) { m_clientId = clientId; emit clientIdChanged(); }

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    GoogleDrive(QObject *parent = nullptr);

    static bool instanceFlag;
    static GoogleDrive *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;
    QString m_clientId;

    void updateConnector();

private slots:
    void onAccessGranted();
    void onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data, QList<QNetworkReply::RawHeaderPair> headers);

signals:
    void authorized();
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data, QList<QNetworkReply::RawHeaderPair> headers);
    void clientIdChanged();
};

#endif // GOOGLEDRIVE_H
