#include "spotifyconnectorserver.h"
#include "logging.h"
#include "../services.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QUrlQuery>

SpotifyConnectorServer::SpotifyConnectorServer
    (QNetworkAccessManager *networkManager, QObject *parent) :
    RESTServiceConnector(networkManager, gmSpotifyServer(), parent)
{
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    m_settingsStore = new O0SettingsStore("gm-companion");

    connect(&m_server, SIGNAL(newConnection()),                        this, SLOT(onIncomingConnection()));
    connect(this,      &SpotifyConnectorServer::receivedReplyInternal, this, &SpotifyConnectorServer::onReceivedReply);
}

SpotifyConnectorServer::~SpotifyConnectorServer()
{
    qCDebug(gmSpotifyServer()) << "Closing spotify connector ...";
    disconnect();

    if (m_server.isListening()) m_server.close();
    m_settingsStore->deleteLater();
}

void SpotifyConnectorServer::grantAccess()
{
    if (getRefreshToken().isEmpty())
    {
        qCInfo(gmSpotifyServer()) << "No refresh token found, initializing auth procedure ...";
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

auto SpotifyConnectorServer::get(QNetworkRequest request)->int
{
    int requestId = getUniqueRequestId();

    get(request, requestId);
    return requestId;
}

void SpotifyConnectorServer::get(QNetworkRequest request, int requestId)
{
    if (!canSendRequest())
    {
        enqueueRequest(RequestContainer(requestId, request, GET, ""));
        return;
    }

    qCDebug(gmSpotifyServer) << "Sending GET Request to URL" << request.url();

    m_currentRequestCount++;
    request = addAuthHeader(request);
    auto reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto data    = reply->readAll();
        auto error   = reply->error();
        auto headers = reply->rawHeaderPairs();
        emit receivedReplyInternal(RequestContainer(requestId, request, GET, ""), data, headers, error);
        reply->deleteLater();
    });
}

auto SpotifyConnectorServer::put(QNetworkRequest request, QByteArray data)->int
{
    int requestId = getUniqueRequestId();

    put(request, data, requestId);
    return requestId;
}

void SpotifyConnectorServer::put(QNetworkRequest request, QByteArray data, int requestId)
{
    if (!canSendRequest())
    {
        enqueueRequest(RequestContainer(requestId, request, PUT, data));
        return;
    }

    qCDebug(gmSpotifyServer) << "Sending PUT Request to URL" << request.url() << "Data:" << data;

    m_currentRequestCount++;
    request = addAuthHeader(request);
    auto reply = m_networkManager->put(request, data);

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto replyData = reply->readAll();
        auto error     = reply->error();
        auto headers   = reply->rawHeaderPairs();
        emit receivedReplyInternal(RequestContainer(requestId, request, PUT, data), replyData, headers, error);
        reply->deleteLater();
    });
}

auto SpotifyConnectorServer::post(QNetworkRequest request, QByteArray data)->int
{
    int requestId = getUniqueRequestId();

    post(request, data, requestId);
    return requestId;
}

void SpotifyConnectorServer::post(QNetworkRequest request, QByteArray data, int requestId)
{
    if (!canSendRequest())
    {
        enqueueRequest(RequestContainer(requestId, request, POST, data));
        return;
    }

    qCDebug(gmSpotifyServer) << "Sending POST Request to URL" << request.url();

    m_currentRequestCount++;
    request = addAuthHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto reply = m_networkManager->post(request, data);

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto replyData = reply->readAll();
        auto error     = reply->error();
        auto headers   = reply->rawHeaderPairs();
        emit receivedReplyInternal(RequestContainer(requestId, request, POST, data), replyData, headers, error);
        reply->deleteLater();
    });
}

void SpotifyConnectorServer::authenticate()
{
    qCDebug(gmSpotifyServer()) << "Authenticating ...";
    emit statusChanged(ServiceStatus::Type::Info, tr("Connecting ..."));

    if (!m_server.isListening())
    {
        m_server.listen(QHostAddress::LocalHost, SPOTIFY_SERVER_PORT);
    }

    auto url = QUrl(SettingsManager::getServerUrl("Spotify") + "/spotify/login");

    QUrlQuery query;
    query.addQueryItem("redirect_uri", QUrl::toPercentEncoding("http://localhost:" + QString::number(SPOTIFY_SERVER_PORT)));
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

    auto reply = m_networkManager->get(QNetworkRequest(url));

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
        qCInfo(gmSpotifyServer()) << "No refresh token found, not connected to spotify.";
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

        auto reply = m_networkManager->get(QNetworkRequest(url));
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
            else if (!params["error"].toString().isEmpty())
            {
                qCWarning(gmSpotifyServer()) << "Could not refresh access token, an unexpected error occurred:" << params["error"].toString();
                emit statusChanged(ServiceStatus::Type::Error, params["error"].toString());
                return;
            }
            else if (getAccessToken().isEmpty())
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

void SpotifyConnectorServer::dequeueRequests()
{
    while (!m_requestQueue.isEmpty() && m_currentRequestCount < SPOTIFY_MAX_REQUESTS)
    {
        auto element = m_requestQueue.dequeue();

        switch (element.requestType)
        {
        case 0:
            get(element.request, element.requestId);
            break;

        case 1:
            put(element.request, element.data, element.requestId);
            break;

        case 2:
            post(element.request, element.data, element.requestId);
            break;

        default:
            qCWarning(gmSpotifyServer()) << "Unknown request type" << element.requestType;
        }
    }
}

auto SpotifyConnectorServer::addAuthHeader(QNetworkRequest request)->QNetworkRequest
{
    request.setRawHeader("Authorization", QByteArray("Bearer " + getAccessToken().toUtf8()));
    return request;
}

void SpotifyConnectorServer::handleRateLimit(const RequestContainer& container, const QList<QPair<QByteArray, QByteArray> >& headers)
{
    qCDebug(gmSpotifyServer) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";

    for (const auto& header : headers)
    {
        if (header.first == "Retry-After")
        {
            qCDebug(gmSpotifyServer()) << header;
            int seconds = header.second.toInt();
            startCooldown(seconds);
            enqueueRequest(container);
            return;
        }
    }

    startCooldown(2);
}

void SpotifyConnectorServer::startCooldown(int seconds)
{
    m_isOnCooldown = true;
    QTimer::singleShot(seconds * 1000, this, &SpotifyConnectorServer::onCooldownFinished);
}

auto SpotifyConnectorServer::canSendRequest()->bool
{
    if (!m_isAccessGranted) return false;

    if (isTokenExpired() || getAccessToken().isEmpty())
    {
        refreshAccessToken();
        return false;
    }

    return !m_isWaitingForToken && !m_isOnCooldown && m_currentRequestCount < SPOTIFY_MAX_REQUESTS;
}

void SpotifyConnectorServer::onReceivedReply(const RequestContainer& container, const QByteArray& data, const QList<QPair<QByteArray, QByteArray> >& headers, QNetworkReply::NetworkError error)
{
    m_currentRequestCount--;

    // Check if rate limit was exceeded
    if (error == QNetworkReply::UnknownContentError)
    {
        auto status = QJsonDocument::fromJson(data).object().value("error").toObject().value("status").toInt();

        // HTTP 429 == Rate limit exceeded
        if (status == 429)
        {
            handleRateLimit(container, headers);
            return;
        }
    }

    if (error != QNetworkReply::NoError)
    {
        qCWarning(gmSpotifyServer()) << "Error:" << error << data;
    }

    emit receivedReply(container.requestId, error, data, headers);

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
