#include "spotifyconnectorserver.h"
#include "config.h"
#include "exceptions/notimplementedexception.h"
#include "thirdparty/http-status-codes/HttpStatusCodes_Qt.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QUrlQuery>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyServer, "gm.service.spotify.server")

SpotifyConnectorServer::SpotifyConnectorServer
    (QNetworkAccessManager *networkManager, QObject *parent) :
    RESTServiceConnector(networkManager, gmSpotifyServer(), parent)
{
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    m_settingsStore = new O0SettingsStore("gm-companion", this);

    connect(&m_server, &QTcpServer::newConnection, this, &SpotifyConnectorServer::onIncomingConnection);
}

SpotifyConnectorServer::~SpotifyConnectorServer()
{
    qCDebug(gmSpotifyServer()) << "Closing spotify connector ...";
    disconnect();

    if (m_server.isListening()) m_server.close();
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
    saveAccessToken("");
    saveRefreshToken("");
}

void SpotifyConnectorServer::sendRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply*>& deferred)
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
        reply = m_networkManager->get(request);
        break;
    case PUT:
        qCDebug(gmSpotifyServer) << "Sending PUT Request to URL" << request.url() << "Data:" << container->data();
        reply = m_networkManager->put(request, container->data());
        break;
    case POST:
        qCDebug(gmSpotifyServer) << "Sending POST Request to URL" << request.url() << "Data:" << container->data();
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        reply = m_networkManager->post(request, container->data());
        break;
    default:
        throw NotImplementedException();
    }

    connect(reply, &QNetworkReply::finished, [this, reply, container, deferred]() mutable {
        onReceivedReply(reply, container, deferred);
    });
}

auto SpotifyConnectorServer::get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>
{
    const auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
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
    const auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
    auto *container = new RequestContainer(request, PUT, data, this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto SpotifyConnectorServer::post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    const auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
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

    if (!m_server.isListening())
    {
        m_server.listen(QHostAddress::LocalHost, SERVER_PORT);
    }

    auto url = QUrl(SettingsManager::getServerUrl("Spotify") + "/spotify/login");

    QUrlQuery query;
    query.addQueryItem("redirect_uri", QUrl::toPercentEncoding("http://localhost:" + QString::number(SERVER_PORT)));
    url.setQuery(query);

    qCDebug(gmSpotifyServer()) << "Started server, opening browser ...";
    QDesktopServices::openUrl(url);
}

void SpotifyConnectorServer::requestAccessToken(const QString& code)
{
    qCDebug(gmSpotifyServer()) << "Requesting access token from server ...";

    QUrl url(SettingsManager::getServerUrl("Spotify") + "/spotify/token");

    QUrlQuery query;
    query.addQueryItem("code", code);
    url.setQuery(query);

    auto *reply = m_networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [ = ]() {
        qCDebug(gmSpotifyServer()) << "Received access token and refresh token ...";
        auto params = QJsonDocument::fromJson(reply->readAll()).object();

        saveAccessToken(params["access_token"].toString());
        saveRefreshToken(params["refresh_token"].toString());
        updateExpireTime(params["expires_in"].toInt());

        m_isWaitingForToken = false;
        m_isAccessGranted   = true;
        emit accessGranted();

        reply->deleteLater();
    });
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
    }
    else
    {
        m_isWaitingForToken = true;
        QUrl url(SettingsManager::getServerUrl("Spotify") + "/spotify/refresh");

        qCDebug(gmSpotifyServer()) << url;

        QUrlQuery query;
        query.addQueryItem("refresh_token", refreshToken);
        url.setQuery(query);

        auto *reply = m_networkManager->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::finished, [ = ]() {
            auto params = QJsonDocument::fromJson(reply->readAll()).object();
            reply->deleteLater();

            saveAccessToken(params["access_token"].toString());
            updateExpireTime(params["expires_in"].toInt());

            // If user revoked permission, the refresh token is no longer valid
            if ((params["error"].toString() == "invalid_grant") || (params["error"].toString() == "invalid_client"))
            {
                qCWarning(gmSpotifyServer()) << "Refresh token was not valid, trying to authenticate again ...";
                saveRefreshToken("");
                authenticate();
                return;
            }

            // Network error, maybe server is unreachable?
            if (reply->error() != QNetworkReply::NoError)
            {
                qCWarning(gmSpotifyServer()) << "Could not refresh access token:" << reply->error() << reply->errorString();
                emit statusChanged(ServiceStatus::Type::Error, reply->errorString());
                return;
            }

            if (!params["error"].toString().isEmpty())
            {
                qCWarning(gmSpotifyServer()) << "Could not refresh access token, an unexpected error occurred:" << params["error"].toString();
                emit statusChanged(ServiceStatus::Type::Error, params["error"].toString());
                return;
            }

            if (getAccessToken().isEmpty())
            {
                qCWarning(gmSpotifyServer()) << "Something went wrong, access token is empty.";
                emit statusChanged(ServiceStatus::Type::Error, "Unexpected error, access token is empty.");
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
}

void SpotifyConnectorServer::updateExpireTime(int expiresIn)
{
    m_expireTime = QDateTime::currentDateTime().addSecs(expiresIn);
}

auto SpotifyConnectorServer::addAuthHeader(QNetworkRequest request) -> QNetworkRequest
{
    request.setRawHeader("Authorization", QByteArray("Bearer " + getAccessToken().toUtf8()));
    return request;
}

void SpotifyConnectorServer::handleRateLimit(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply*> &deferred,
                                             const QList<QPair<QByteArray, QByteArray>>& headers)
{
    using namespace std;

    qCDebug(gmSpotifyServer) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";

    for (const auto& header : headers)
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

void SpotifyConnectorServer::enqueueRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply*> &deferred)
{
    m_requestQueue.enqueue(QPair(container, deferred));
}

void SpotifyConnectorServer::dequeueRequests()
{
    while (!m_requestQueue.isEmpty() && m_currentRequestCount < MAX_REQUESTS)
    {
        const auto pair = m_requestQueue.dequeue();
        auto *container = pair.first;
        const auto deferred = pair.second;

        sendRequest(container, deferred);
    }
}

void SpotifyConnectorServer::onReceivedReply(QNetworkReply *reply, RequestContainer *container,
                                             AsyncFuture::Deferred<RestNetworkReply*> deferred)
{
    m_currentRequestCount--;

    if (!reply)
    {
        qCCritical(gmSpotifyServer()) << "Error: QNetworkReply is null!";
        return;
    }

    auto *result = new RestNetworkReply(
                reply->error(),
                reply->errorString(),
                reply->readAll(),
                reply->rawHeaderPairs(),
                this);

    // Check if rate limit was exceeded
    if (result->error() == QNetworkReply::UnknownContentError)
    {
        const auto status = QJsonDocument::fromJson(result->data())
                .object()["error"].toObject()["status"].toInt();

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

void SpotifyConnectorServer::onIncomingConnection()
{
    QTcpSocket *socket = m_server.nextPendingConnection();

    connect(socket, SIGNAL(readyRead()),    this,   SLOT(onBytesReady()), Qt::UniqueConnection);
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void SpotifyConnectorServer::onBytesReady()
{
    qCDebug(gmSpotifyServer()) << "onBytesReady: Processing request";

    // NOTE: on first call, the timeout timer is started
    auto *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket) {
        qCWarning(gmSpotifyServer()) << "onBytesReady: No socket available";
        emit statusChanged(ServiceStatus::Type::Error, "Error: No socket available.");
        return;
    }

    QFile f(":/services/auth-success.html");
    QByteArray replyContent;

    if (f.open(QIODevice::ReadOnly))
    {
        replyContent = f.readAll();
        f.close();
    }

    QByteArray reply;
    reply.append("HTTP/1.0 200 OK \r\n");
    reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    reply.append(QString("Content-Length: %1\r\n\r\n").arg(replyContent.size()).toLatin1());
    reply.append(replyContent);
    socket->write(reply);
    qCDebug(gmSpotifyServer()) << "onBytesReady: Sent reply";

    QByteArray data                    = socket->readAll();
    QMap<QString, QString> queryParams = parseQueryParams(&data);

    if (queryParams.isEmpty()) {
        if (m_tries < 3) {
            qCDebug(gmSpotifyServer()) << "onBytesReady: No query params found, waiting for more callbacks";
            ++m_tries;
            return;
        }
        m_tries = 0;
        qCWarning(gmSpotifyServer()) << "onBytesReady: No query params found, maximum callbacks received";
        closeServer(socket, false);
        return;
    }

    qCDebug(gmSpotifyServer()) << "onBytesReady: Query params found, closing server";
    closeServer(socket, true);

    requestAccessToken(queryParams["code"]);
}

void SpotifyConnectorServer::closeServer(QTcpSocket *socket, bool  /*hasparameters*/)
{
    qCDebug(gmSpotifyServer()) << "closeServer: Initiating";
    int port = m_server.serverPort();

    if (!socket && sender()) {
        auto *timer = qobject_cast<QTimer *>(sender());

        if (timer) {
            qCWarning(gmSpotifyServer()) << "closeServer: Closing due to timeout";
            emit statusChanged(ServiceStatus::Type::Error, "Error: Connection timed out.");
            timer->stop();
            socket = qobject_cast<QTcpSocket *>(timer->parent());
            timer->deleteLater();
        }
    }

    if (socket) {
        auto *timer = socket->findChild<QTimer *>("timeoutTimer");

        if (timer) {
            qCDebug(gmSpotifyServer()) << "closeServer: Stopping socket's timeout timer";
            timer->stop();
        }
        socket->disconnectFromHost();
    }
    m_server.close();
    qCDebug(gmSpotifyServer()) << "closeServer: Closed, no longer listening on port" << port;
}

auto SpotifyConnectorServer::parseQueryParams(QByteArray *data)->QMap<QString, QString>
{
    qCDebug(gmSpotifyServer()) << "parseQueryParams";

    QString splitGetLine = QString(*data).split("\r\n").first();
    splitGetLine.remove("GET ");
    splitGetLine.remove("HTTP/1.1");
    splitGetLine.remove("\r\n");
    splitGetLine.prepend("http://localhost");
    QUrl getTokenUrl(splitGetLine);

    QList<QPair<QString, QString> > tokens;
#if QT_VERSION < 0x050000
    tokens = getTokenUrl.queryItems();
#else // if QT_VERSION < 0x050000
    QUrlQuery query(getTokenUrl);
    tokens = query.queryItems();
#endif // if QT_VERSION < 0x050000
    QMap<QString, QString>  queryParams;
    QPair<QString, QString> tokenPair;
    foreach(tokenPair, tokens) {
        // FIXME: We are decoding key and value again. This helps with Google
        // OAuth, but is it mandated by the standard?
        QString key   = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.first.trimmed().toLatin1()));
        QString value = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.second.trimmed().toLatin1()));

        queryParams.insert(key, value);
    }
    return queryParams;
}
