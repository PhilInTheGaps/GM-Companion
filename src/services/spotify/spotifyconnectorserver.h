#pragma once

#include "rest/callbackserver.h"
#include "rest/restrequest.h"
#include "rest/restserviceconnector.h"
#include "utils/stringutils.h"
#include <QJsonObject>
#include <QObject>
#include <QPromise>
#include <QQueue>
#include <chrono>
#include <o0settingsstore.h>

namespace Services
{

class SpotifyConnectorServer : public RESTServiceConnector
{
    Q_OBJECT
public:
    explicit SpotifyConnectorServer(QNetworkAccessManager &networkManager, QObject *parent = nullptr);

    void grantAccess() override;
    void disconnectService() override;
    [[nodiscard]] auto isAccessGranted() const -> bool override
    {
        return m_isAccessGranted;
    }

    auto get(const QUrl &url, bool isAuthRequired) -> QFuture<RestReply>;
    auto get(const QNetworkRequest &request, bool isAuthRequired) -> QFuture<RestReply> override;
    auto put(QNetworkRequest request, const QByteArray &data = "") -> QFuture<RestReply> override;
    auto post(QNetworkRequest request, const QByteArray &data = "") -> QFuture<RestReply> override;
    auto customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
        -> QFuture<RestReply> override;

private:
    O0SettingsStore m_settingsStore = O0SettingsStore(QStringLiteral("gm-companion"));
    CallbackServer m_callbackServer;

    bool m_isAccessGranted = false;
    bool m_isWaitingForToken = false;
    bool m_isOnCooldown = false;
    int m_requestCount = 0;

    void authenticate();

    [[nodiscard]] auto getAccessToken() -> QString override;
    void saveAccessToken(const QString &token);
    void requestAccessToken(const QString &code);

    [[nodiscard]] auto getRefreshToken() -> QString;
    void saveRefreshToken(const QString &token);
    void refreshAccessToken(bool updateAuthentication = false) override;
    auto handleRefreshErrors(const QJsonObject &params) -> bool;

    void sendRequest(RestRequest &&container, QPromise<RestReply> &&promise) override;

    [[nodiscard]] auto addAuthHeader(QNetworkRequest request) -> QNetworkRequest;

private slots:
    void onServerError(const QString &error);
    void onServerClosed(bool hasParameters);
};

} // namespace Services
