#include "spotifyconnectorserver.h"
#include "config.h"
#include "exceptions/notimplementedexception.h"
#include "settings/settingsmanager.h"
#include "thirdparty/http-status-codes/HttpStatusCodes_Qt.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyServer, "gm.service.spotify.server")

SpotifyConnectorServer::SpotifyConnectorServer(QNetworkAccessManager &networkManager, QObject *parent)
    : RESTServiceConnector(networkManager, gmSpotifyServer(), parent)
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    connect(&m_callbackServer, &CallbackServer::serverError, this, &SpotifyConnectorServer::onServerError);
    connect(&m_callbackServer, &CallbackServer::serverClosed, this, &SpotifyConnectorServer::onServerClosed);
}

void SpotifyConnectorServer::grantAccess()
{
    if (getRefreshToken().isEmpty())
    {
        qCDebug(gmSpotifyServer()) << "No refresh token found, initializing auth procedure ...";
        authenticate();
    }
    else
    {
        qCDebug(gmSpotifyServer()) << "Refresh token exists, requesting new access token ...";
        refreshAccessToken(true);
    }
}

void SpotifyConnectorServer::disconnectService()
{
    qCDebug(gmSpotifyServer()) << "Disconnecting ...";
    saveAccessToken(QLatin1String());
    saveRefreshToken(QLatin1String());
}

void SpotifyConnectorServer::sendRequest(RequestContainer *container,
                                         const AsyncFuture::Deferred<RestNetworkReply *> &deferred)
{
    if (!canSendRequest())
    {
        enqueueRequest(container, deferred);
        return;
    }

    m_currentRequestCount++;
    auto request = addAuthHeader(container->request());
    QNetworkReply *reply = nullptr;

    switch (container->requestType())
    {
    case GET:
        qCDebug(gmSpotifyServer) << "Sending GET Request to URL" << request.url();
        reply = m_networkManager.get(request);
        break;
    case PUT:
        qCDebug(gmSpotifyServer) << "Sending PUT Request to URL" << request.url() << "Data:" << container->data();
        reply = m_networkManager.put(request, container->data());
        break;
    case POST:
        qCDebug(gmSpotifyServer) << "Sending POST Request to URL" << request.url() << "Data:" << container->data();
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        reply = m_networkManager.post(request, container->data());
        break;
    default:
        throw NotImplementedException();
    }

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, container, deferred]() mutable { onReceivedReply(reply, container, deferred); });
}

auto SpotifyConnectorServer::get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>
{
    const auto deferred = AsyncFuture::deferred<RestNetworkReply *>();
    auto *container = new RequestContainer(request, GET, "", this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto SpotifyConnectorServer::get(const QUrl &url) -> QFuture<RestNetworkReply *>
{
    return get(QNetworkRequest(url));
}

auto SpotifyConnectorServer::put(QNetworkRequest request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    const auto deferred = AsyncFuture::deferred<RestNetworkReply *>();
    auto *container = new RequestContainer(request, PUT, data, this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto SpotifyConnectorServer::post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    const auto deferred = AsyncFuture::deferred<RestNetworkReply *>();
    auto *container = new RequestContainer(request, POST, data, this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto SpotifyConnectorServer::customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                           const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    Q_UNUSED(request)
    Q_UNUSED(verb)
    Q_UNUSED(data)
    throw NotImplementedException();
}

void SpotifyConnectorServer::authenticate()
{
    qCDebug(gmSpotifyServer()) << "Authenticating ...";
    emit statusChanged(ServiceStatus::Type::Info, tr("Connecting ..."));

    if (!m_callbackServer.isRunning())
    {
        m_callbackServer.start(SERVER_PORT, MAX_CALLBACK_RETRIES, SUCCESS_PAGE_PATH);
    }

    auto url = QUrl(SettingsManager::getServerUrl(QStringLiteral("Spotify")) + "/spotify/login");

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("redirect_uri"), QUrl::toPercentEncoding(m_callbackServer.getUrl()));
    url.setQuery(query);

    qCDebug(gmSpotifyServer()) << "Started server, opening browser ...";
    QDesktopServices::openUrl(url);
}

auto SpotifyConnectorServer::getAccessToken() -> QString
{
    return m_settingsStore.value(ACCESS_TOKEN_KEY);
}

void SpotifyConnectorServer::saveAccessToken(const QString &token)
{
    m_settingsStore.setValue(ACCESS_TOKEN_KEY, token);
}

void SpotifyConnectorServer::requestAccessToken(const QString &code)
{
    qCDebug(gmSpotifyServer()) << "Requesting access token from server ...";

    QUrl url(SettingsManager::getServerUrl(QStringLiteral("Spotify")) + "/spotify/token");

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("code"), code);
    query.addQueryItem(QStringLiteral("redirect_uri"), QUrl::toPercentEncoding(m_callbackServer.getUrl()));
    url.setQuery(query);

    auto *reply = m_networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        qCDebug(gmSpotifyServer()) << "Received access token and refresh token ...";
        auto params = QJsonDocument::fromJson(reply->readAll()).object();

        saveAccessToken(params[QStringLiteral("access_token")].toString());
        saveRefreshToken(params[QStringLiteral("refresh_token")].toString());
        updateExpireTime(params[QStringLiteral("expires_in")].toInt());

        m_isWaitingForToken = false;
        m_isAccessGranted = true;
        emit accessGranted();

        reply->deleteLater();
    });
}

auto SpotifyConnectorServer::getRefreshToken() -> QString
{
    return m_settingsStore.value(REFRESH_TOKEN_KEY);
}

void SpotifyConnectorServer::saveRefreshToken(const QString &token)
{
    m_settingsStore.setValue(REFRESH_TOKEN_KEY, token);
}

void SpotifyConnectorServer::refreshAccessToken(bool updateAuthentication)
{
    qCDebug(gmSpotifyServer()) << "Refreshing access token ...";

    auto refreshToken = getRefreshToken();

    if (refreshToken.isEmpty())
    {
        qCDebug(gmSpotifyServer()) << "No refresh token found, not connected to spotify.";
        m_isAccessGranted = false;
        emit isConnectedChanged(false);
        return;
    }

    m_isWaitingForToken = true;
    QUrl url(SettingsManager::getServerUrl(QStringLiteral("Spotify")) + "/spotify/refresh");
    qCDebug(gmSpotifyServer()) << url;

    url.setQuery(QUrlQuery({{"refresh_token", refreshToken}}));

    auto *reply = m_networkManager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, updateAuthentication]() {
        // Network error, maybe server is unreachable?
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmSpotifyServer()) << "Could not refresh access token:" << reply->error() << reply->errorString();
            emit statusChanged(ServiceStatus::Type::Error, reply->errorString());
            reply->deleteLater();
            return;
        }

        auto params = QJsonDocument::fromJson(reply->readAll()).object();
        reply->deleteLater();

        saveAccessToken(params[QStringLiteral("access_token")].toString());
        updateExpireTime(params[QStringLiteral("expires_in")].toInt());

        if (handleRefreshErrors(params)) return;

        if (getAccessToken().isEmpty())
        {
            qCWarning(gmSpotifyServer()) << "Something went wrong, access token is empty.";
            emit statusChanged(ServiceStatus::Type::Error, QStringLiteral("Unexpected error, access token is empty."));
            return;
        }

        // Everything is fine
        qCDebug(gmSpotifyServer()) << "Received new access token ...";

        if (updateAuthentication)
        {
            m_isAccessGranted = true;
            emit accessGranted();
        }

        m_isWaitingForToken = false;
        dequeueRequests();
    });
}

/// Handle spotify related errors during a token refresh request
/// @returns true if an error occurred, otherwise false
auto SpotifyConnectorServer::handleRefreshErrors(const QJsonObject &params) -> bool
{
    // If user revoked permission, the refresh token is no longer valid
    if ((params[QStringLiteral("error")].toString() == QStringLiteral("invalid_grant")) ||
        (params[QStringLiteral("error")].toString() == QStringLiteral("invalid_client")))
    {
        qCWarning(gmSpotifyServer()) << "Refresh token was not valid, trying to authenticate again ...";
        saveRefreshToken(QLatin1String());
        authenticate();
        return true;
    }

    if (!params[QStringLiteral("error")].toString().isEmpty())
    {
        qCWarning(gmSpotifyServer()) << "Could not refresh access token, an unexpected error occurred:"
                                     << params[QStringLiteral("error")].toString();
        emit statusChanged(ServiceStatus::Type::Error, params[QStringLiteral("error")].toString());
        return true;
    }

    return false;
}

void SpotifyConnectorServer::updateExpireTime(int expiresIn)
{
    m_expireTime = QDateTime::currentDateTime().addSecs(expiresIn);
}

auto SpotifyConnectorServer::isTokenExpired() const -> bool
{
    return QDateTime::currentDateTime() > m_expireTime;
}

auto SpotifyConnectorServer::addAuthHeader(QNetworkRequest request) -> QNetworkRequest
{
    request.setRawHeader("Authorization", QByteArray("Bearer " + getAccessToken().toUtf8()));
    return request;
}

void SpotifyConnectorServer::handleRateLimit(RequestContainer *container,
                                             const AsyncFuture::Deferred<RestNetworkReply *> &deferred,
                                             const QList<std::pair<QByteArray, QByteArray>> &headers)
{
    using namespace std;

    qCDebug(gmSpotifyServer) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";

    for (const auto &header : headers)
    {
        if (header.first == "Retry-After")
        {
            qCDebug(gmSpotifyServer()) << header;
            const auto seconds = chrono::seconds(header.second.toInt());
            startCooldown(seconds);
            enqueueRequest(container, deferred);
            return;
        }
    }

    startCooldown(2s);
}

void SpotifyConnectorServer::startCooldown(std::chrono::seconds seconds)
{
    m_isOnCooldown = true;
    QTimer::singleShot(seconds, this, &SpotifyConnectorServer::onCooldownFinished);
}

auto SpotifyConnectorServer::canSendRequest() -> bool
{
    if (!m_isAccessGranted) return false;

    if (isTokenExpired() || getAccessToken().isEmpty())
    {
        refreshAccessToken();
        return false;
    }

    return !m_isWaitingForToken && !m_isOnCooldown && m_currentRequestCount < MAX_REQUESTS;
}

void SpotifyConnectorServer::enqueueRequest(RequestContainer *container,
                                            const AsyncFuture::Deferred<RestNetworkReply *> &deferred)
{
    m_requestQueue.enqueue(std::pair(container, deferred));
}

void SpotifyConnectorServer::dequeueRequests()
{
    while (!m_requestQueue.isEmpty() && m_currentRequestCount < MAX_REQUESTS)
    {
        const auto pair = m_requestQueue.dequeue();
        auto *container = pair.first;
        const auto &deferred = pair.second;

        sendRequest(container, deferred);
    }
}

void SpotifyConnectorServer::onReceivedReply(QNetworkReply *reply, RequestContainer *container,
                                             AsyncFuture::Deferred<RestNetworkReply *> deferred)
{
    m_currentRequestCount--;

    if (!reply)
    {
        qCCritical(gmSpotifyServer()) << "Error: QNetworkReply is null!";
        return;
    }

    auto *result =
        new RestNetworkReply(reply->error(), reply->errorString(), reply->readAll(), reply->rawHeaderPairs(), this);

    // Check if rate limit was exceeded
    if (result->error() == QNetworkReply::UnknownContentError)
    {
        const auto status = QJsonDocument::fromJson(result->data())
                                .object()[QStringLiteral("error")]
                                .toObject()[QStringLiteral("status")]
                                .toInt();

        if (status == HttpStatus::TooManyRequests)
        {
            handleRateLimit(container, deferred, result->headers());
            return;
        }
    }

    if (result->hasError())
    {
        // The "No active device found" should not be logged as a warning
        // because we can recover from that by setting an active device and trying again.
        if (result->data().contains("No active device found"))
        {
            qCDebug(gmSpotifyServer) << result->errorText() << result->data();
        }
        else
        {
            qCWarning(gmSpotifyServer()) << "Error:" << result->errorText() << result->data();
        }
    }

    deferred.complete(result);
    reply->deleteLater();

    // If there are requests in queue, send next
    dequeueRequests();
}

void SpotifyConnectorServer::onCooldownFinished()
{
    qCDebug(gmSpotifyServer()) << "Cooldown finished. Continuing ...";
    m_isOnCooldown = false;
    dequeueRequests();
}

void SpotifyConnectorServer::onServerError(const QString &error)
{
    emit statusChanged(ServiceStatus::Type::Error, error);
}

void SpotifyConnectorServer::onServerClosed(bool hasParameters)
{
    if (!hasParameters) return;

    const auto parameters = m_callbackServer.getParameters();

    if (!parameters.contains(QStringLiteral("code"))) return;

    requestAccessToken(parameters[QStringLiteral("code")]);
}
