#ifndef SPOTIFYCONNECTORSERVER_H
#define SPOTIFYCONNECTORSERVER_H

#include <QObject>
#include "restserviceconnector.h"
#include "settings/settingsmanager.h"

#include <QQueue>
#include <QTcpServer>
#include <o0settingsstore.h>

class SpotifyConnectorServer : public RESTServiceConnector
{
    Q_OBJECT
public:
    SpotifyConnectorServer(QNetworkAccessManager* networkManager, QObject *parent = nullptr);
    ~SpotifyConnectorServer();

    void grantAccess();
    bool isAccessGranted() const { return m_isAccessGranted; }

    int get(QUrl url) { return get(QNetworkRequest(url)); }
    int get(QNetworkRequest request);
    void get(QNetworkRequest request, int requestId);
    int put(QNetworkRequest request, QByteArray data = "");
    void put(QNetworkRequest request, QByteArray data, int requestId);
    int post(QNetworkRequest request, QByteArray data = "");
    void post(QNetworkRequest request, QByteArray data, int requestId);

    void customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data, int requestId) {}

    int getUniqueRequestId() { return ++m_requestCount; }

private:
    O0SettingsStore *m_settingsStore = nullptr;

    QTcpServer m_server;
    int m_tries = 0;

    QDateTime m_expireTime;
    bool m_isAccessGranted = false;
    bool m_isWaitingForToken = false;
    bool m_isOnCooldown = false;
    int m_requestCount = 0;
    int m_currentRequestCount = 0;
    QQueue<RequestContainer> m_requestQueue;

    void authenticate();

    QString getAccessToken() const { return m_settingsStore->value("SPOTIFY_ACCESS_TOKEN"); }
    void saveAccessToken(QString token) { m_settingsStore->setValue("SPOTIFY_ACCESS_TOKEN", token); }
    void requestAccessToken(const QString& code);

    QString getRefreshToken() const { return m_settingsStore->value("SPOTIFY_REFRESH_TOKEN"); }
    void saveRefreshToken(QString token) { m_settingsStore->setValue("SPOTIFY_REFRESH_TOKEN", token); }
    void refreshAccessToken(bool updateAuthentication = false);

    void updateExpireTime(int expiresIn);
    bool isTokenExpired() const { return QDateTime::currentDateTime() > m_expireTime; }

    static QString getServerUrl();

    bool canSendRequest();
    void enqueueRequest(RequestContainer container) { m_requestQueue.enqueue(container); }
    void dequeueRequests();

    QNetworkRequest addAuthHeader(QNetworkRequest request);

    void handleRateLimit(const RequestContainer& container, const QList<QPair<QByteArray, QByteArray> >& headers);
    void startCooldown(int seconds);

private slots:
    void onIncomingConnection();
    void onBytesReady();
    void closeServer(QTcpSocket *socket, bool hasparameters);
    static QMap<QString, QString> parseQueryParams(QByteArray *data);
    void onReceivedReply(const RequestContainer& container, const QByteArray& data, const QList<QPair<QByteArray, QByteArray>>& headers, QNetworkReply::NetworkError error);
    void onCooldownFinished();

signals:
    void receivedReplyInternal(RequestContainer container, QByteArray data, QList<QPair<QByteArray, QByteArray>> headers, QNetworkReply::NetworkError error);
};

#endif // SPOTIFYCONNECTORSERVER_H
