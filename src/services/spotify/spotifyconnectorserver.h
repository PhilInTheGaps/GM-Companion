#pragma once

#include <QJsonObject>
#include <QObject>
#include <QQueue>
#include <chrono>
#include <o0settingsstore.h>

#include "rest/callbackserver.h"
#include "rest/requestcontainer.h"
#include "rest/restserviceconnector.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/stringutils.h"

class SpotifyConnectorServer : public RESTServiceConnector
{
    Q_OBJECT
public:
    explicit SpotifyConnectorServer(QNetworkAccessManager &networkManager, QObject *parent = nullptr);

    void grantAccess() override;
    void disconnectService() override;
    [[nodiscard]] bool isAccessGranted() const override
    {
        return m_isAccessGranted;
    }

    QFuture<RestNetworkReply *> get(const QUrl &url);
    QFuture<RestNetworkReply *> get(const QNetworkRequest &request) override;
    QFuture<RestNetworkReply *> put(QNetworkRequest request, const QByteArray &data = "") override;
    QFuture<RestNetworkReply *> post(QNetworkRequest request, const QByteArray &data = "") override;
    QFuture<RestNetworkReply *> customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                              const QByteArray &data) override;

private:
    O0SettingsStore m_settingsStore = O0SettingsStore(QStringLiteral("gm-companion"));
    CallbackServer m_callbackServer;

    QDateTime m_expireTime;
    bool m_isAccessGranted = false;
    bool m_isWaitingForToken = false;
    bool m_isOnCooldown = false;
    int m_requestCount = 0;
    int m_currentRequestCount = 0;
    QQueue<QPair<RequestContainer *, AsyncFuture::Deferred<RestNetworkReply *>>> m_requestQueue;

    static constexpr ConstQString ACCESS_TOKEN_KEY = "SPOTIFY_ACCESS_TOKEN";
    static constexpr ConstQString REFRESH_TOKEN_KEY = "SPOTIFY_REFRESH_TOKEN";
    static constexpr ConstQString SUCCESS_PAGE_PATH = ":/services/auth-success.html";
    static constexpr int MAX_CALLBACK_RETRIES = 3;

    void authenticate();

    [[nodiscard]] auto getAccessToken() -> QString;
    void saveAccessToken(const QString &token);
    void requestAccessToken(const QString &code);

    [[nodiscard]] auto getRefreshToken() -> QString;
    void saveRefreshToken(const QString &token);
    void refreshAccessToken(bool updateAuthentication = false);
    auto handleRefreshErrors(const QJsonObject &params) -> bool;

    void updateExpireTime(int expiresIn);
    [[nodiscard]] auto isTokenExpired() const -> bool;

    auto canSendRequest() -> bool;
    void enqueueRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred);
    void dequeueRequests();

    void sendRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred);

    [[nodiscard]] auto addAuthHeader(QNetworkRequest request) -> QNetworkRequest;

    void handleRateLimit(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred,
                         const QList<QPair<QByteArray, QByteArray>> &headers);
    void startCooldown(std::chrono::seconds seconds);

private slots:
    void onReceivedReply(QNetworkReply *reply, RequestContainer *container,
                         AsyncFuture::Deferred<RestNetworkReply *> deferred);
    void onCooldownFinished();
    void onServerError(const QString &error);
    void onServerClosed(bool hasParameters);
};
