#include "spotifyconnectorserver.h"
#include "config.h"
#include "exceptions/notimplementedexception.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmSpotifyServer, "gm.service.spotify.server")

constexpr auto ACCESS_TOKEN_KEY = "SPOTIFY_ACCESS_TOKEN";
constexpr auto REFRESH_TOKEN_KEY = "SPOTIFY_REFRESH_TOKEN";
constexpr auto SUCCESS_PAGE_PATH = ":/services/auth-success.html";
constexpr int MAX_CALLBACK_RETRIES = 3;

SpotifyConnectorServer::SpotifyConnectorServer(QNetworkAccessManager &networkManager, QObject *parent)
    : RESTServiceConnector(networkManager, gmSpotifyServer(), {u"No active device found"_s}, parent)
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    setMaxConcurrentRequests(MAX_REQUESTS);

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
    saveAccessToken(u""_s);
    saveRefreshToken(u""_s);
}

void SpotifyConnectorServer::sendRequest(RestRequest &&container, QPromise<RestReply> &&promise)
{
    auto request = container.isAuthRequired() ? addAuthHeader(container.request()) : container.request();
    QNetworkReply *reply = nullptr;

    switch (container.type())
    {
    case RestRequest::Type::GET:
        qCDebug(gmSpotifyServer) << "Sending GET Request to URL" << request.url();
        reply = m_networkManager.get(request);
        break;
    case RestRequest::Type::PUT:
        qCDebug(gmSpotifyServer) << "Sending PUT Request to URL" << request.url() << "Data:" << container.data();
        reply = m_networkManager.put(request, container.data());
        break;
    case RestRequest::Type::POST:
        qCDebug(gmSpotifyServer) << "Sending POST Request to URL" << request.url() << "Data:" << container.data();
        NetworkUtils::makeJsonRequest(request);
        reply = m_networkManager.post(request, container.data());
        break;
    default:
        throw NotImplementedException();
    }

    auto id = container.id();
    connect(reply, &QNetworkReply::finished, this, [this, reply, id]() mutable {
        onReplyReceived(id, reply->error(), reply->errorString(), reply->readAll(), reply->rawHeaderPairs());
    });

    markRequestActive(std::move(container), std::move(promise));
}

auto SpotifyConnectorServer::get(const QNetworkRequest &request, bool isAuthRequired) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::GET);
    container.isAuthRequired(isAuthRequired);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto SpotifyConnectorServer::get(const QUrl &url, bool isAuthRequired) -> QFuture<RestReply>
{
    return get(QNetworkRequest(url), isAuthRequired);
}

auto SpotifyConnectorServer::put(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::PUT, data);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto SpotifyConnectorServer::post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::POST, data);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto SpotifyConnectorServer::customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                           const QByteArray &data) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(verb)
    Q_UNUSED(data)
    throw NotImplementedException();
}

void SpotifyConnectorServer::authenticate()
{
    qCDebug(gmSpotifyServer()) << "Authenticating ...";
    emit statusChanged(Status::Type::Info, tr("Connecting ..."));

    if (!m_callbackServer.isRunning())
    {
        m_callbackServer.start(SERVER_PORT, MAX_CALLBACK_RETRIES, SUCCESS_PAGE_PATH);
    }

    auto url = QUrl(SettingsManager::getServerUrl(u"Spotify"_s) + "/spotify/login"_L1);

    QUrlQuery query;
    query.addQueryItem(u"redirect_uri"_s, QUrl::toPercentEncoding(m_callbackServer.getUrl()));
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

    QUrl url(SettingsManager::getServerUrl(u"Spotify"_s) + "/spotify/token"_L1);

    QUrlQuery query;
    query.addQueryItem(u"code"_s, code);
    query.addQueryItem(u"redirect_uri"_s, QUrl::toPercentEncoding(m_callbackServer.getUrl()));
    url.setQuery(query);

    auto *reply = m_networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        qCDebug(gmSpotifyServer()) << "Received access token and refresh token ...";
        auto params = QJsonDocument::fromJson(reply->readAll()).object();

        saveAccessToken(params["access_token"_L1].toString());
        saveRefreshToken(params["refresh_token"_L1].toString());
        updateTokenExpireTime(std::chrono::seconds(params["expires_in"_L1].toInt()));

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
    QUrl url(SettingsManager::getServerUrl(u"Spotify"_s) + "/spotify/refresh"_L1);
    qCDebug(gmSpotifyServer()) << url;

    url.setQuery(QUrlQuery({{"refresh_token", refreshToken}}));

    auto *reply = m_networkManager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, updateAuthentication]() {
        // Network error, maybe server is unreachable?
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmSpotifyServer()) << "Could not refresh access token:" << reply->error() << reply->errorString();
            emit statusChanged(Status::Type::Error, reply->errorString());
            reply->deleteLater();
            return;
        }

        auto params = QJsonDocument::fromJson(reply->readAll()).object();
        reply->deleteLater();

        saveAccessToken(params["access_token"_L1].toString());
        updateTokenExpireTime(std::chrono::seconds(params["expires_in"_L1].toInt()));

        if (handleRefreshErrors(params)) return;

        if (getAccessToken().isEmpty())
        {
            qCWarning(gmSpotifyServer()) << "Something went wrong, access token is empty.";
            emit statusChanged(Status::Type::Error, u"Unexpected error, access token is empty."_s);
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
    if ((params["error"_L1].toString() == "invalid_grant"_L1) || (params["error"_L1].toString() == "invalid_client"_L1))
    {
        qCWarning(gmSpotifyServer()) << "Refresh token was not valid, trying to authenticate again ...";
        saveRefreshToken(u""_s);
        authenticate();
        return true;
    }

    if (!params["error"_L1].toString().isEmpty())
    {
        qCWarning(gmSpotifyServer()) << "Could not refresh access token, an unexpected error occurred:"
                                     << params["error"_L1].toString();
        emit statusChanged(Status::Type::Error, params["error"_L1].toString());
        return true;
    }

    return false;
}

auto SpotifyConnectorServer::addAuthHeader(QNetworkRequest request) -> QNetworkRequest
{
    request.setRawHeader("Authorization", QByteArray("Bearer " + getAccessToken().toUtf8()));
    return request;
}

void SpotifyConnectorServer::onServerError(const QString &error)
{
    emit statusChanged(Status::Type::Error, error);
}

void SpotifyConnectorServer::onServerClosed(bool hasParameters)
{
    if (!hasParameters) return;

    const auto parameters = m_callbackServer.getParameters();

    if (!parameters.contains("code"_L1)) return;

    requestAccessToken(parameters["code"_L1]);
}
