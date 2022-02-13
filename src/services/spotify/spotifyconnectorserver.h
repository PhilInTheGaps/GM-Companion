#pragma once

#include <QObject>
#include <chrono>
#include "rest/restserviceconnector.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QQueue>
#include <QTcpServer>
#include <o0settingsstore.h>

class SpotifyConnectorServer : public RESTServiceConnector
{
    Q_OBJECT
public:
    explicit SpotifyConnectorServer(QNetworkAccessManager* networkManager, QObject *parent = nullptr);
    ~SpotifyConnectorServer() override;

    void grantAccess() override;
    void disconnectService() override;
    bool isAccessGranted() const override { return m_isAccessGranted; }

    QFuture<RestNetworkReply*> get(const QUrl &url);
    QFuture<RestNetworkReply*> get(const QNetworkRequest &request) override;
    QFuture<RestNetworkReply*> put(QNetworkRequest request, const QByteArray &data = "") override;
    QFuture<RestNetworkReply*> post(QNetworkRequest request, const QByteArray &data = "") override;

    QFuture<RestNetworkReply*> customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                             const QByteArray &data) override;

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
    QQueue<QPair<RequestContainer*, AsyncFuture::Deferred<RestNetworkReply*>>> m_requestQueue;

    void authenticate();

    [[nodiscard]] auto getAccessToken() const -> QString { return m_settingsStore->value("SPOTIFY_ACCESS_TOKEN"); }
    void saveAccessToken(const QString &token) { m_settingsStore->setValue("SPOTIFY_ACCESS_TOKEN", token); }
    void requestAccessToken(const QString& code);

    [[nodiscard]] auto getRefreshToken() const -> QString { return m_settingsStore->value("SPOTIFY_REFRESH_TOKEN"); }
    void saveRefreshToken(const QString &token) { m_settingsStore->setValue("SPOTIFY_REFRESH_TOKEN", token); }
    void refreshAccessToken(bool updateAuthentication = false);

    void updateExpireTime(int expiresIn);
    [[nodiscard]] bool isTokenExpired() const { return QDateTime::currentDateTime() > m_expireTime; }

    auto canSendRequest() -> bool;
    void enqueueRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred);
    void dequeueRequests();

    void sendRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply*>& deferred);

    auto addAuthHeader(QNetworkRequest request) -> QNetworkRequest;

    void handleRateLimit(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred,
                         const QList<QPair<QByteArray, QByteArray> >& headers);
    void startCooldown(std::chrono::seconds seconds);

private slots:
    void onIncomingConnection();
    void onBytesReady();
    void closeServer(QTcpSocket *socket, bool hasparameters);
    static QMap<QString, QString> parseQueryParams(QByteArray *data);
    void onReceivedReply(QNetworkReply *reply, RequestContainer *container, AsyncFuture::Deferred<RestNetworkReply *> deferred);
    void onCooldownFinished();
};
