#ifndef ISPOTIFYCONNECTOR_H
#define ISPOTIFYCONNECTOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class ISpotifyConnector : public QObject
{
    Q_OBJECT
public:
    explicit ISpotifyConnector(QNetworkAccessManager* networkManager):
        m_networkManager(networkManager) {}
    virtual ~ISpotifyConnector() {}

    virtual void grantAccess() = 0;
    virtual bool isAccessGranted() const = 0;

    virtual int get(QUrl url) = 0;
    virtual int get(QNetworkRequest request) = 0;
    virtual void get(QNetworkRequest request, int requestId) = 0;
    virtual int put(QUrl url, QString params = "") = 0;
    virtual int post(QNetworkRequest request, QByteArray data = "") = 0;
    virtual int getUniqueRequestId() = 0;

    virtual QString getConnectorType() const = 0;

protected:
    QNetworkAccessManager *m_networkManager = nullptr;

signals:
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data);
    void accessGranted();
};

#endif // ISPOTIFYCONNECTOR_H
