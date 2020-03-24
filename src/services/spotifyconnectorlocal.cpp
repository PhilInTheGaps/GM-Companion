#include "spotifyconnectorlocal.h"
#include "logging.h"
#include "services.h"
#include "o0globals.h"
#include "o0requestparameter.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>

SpotifyConnectorLocal::SpotifyConnectorLocal
    (QNetworkAccessManager *networkManager) :
    ISpotifyConnector(networkManager)
{
    m_o2spotify = new O2Spotify;

    // Settings Store for tokens
    m_settingsStore = new O0SettingsStore("gm-companion");
    m_o2spotify->setStore(m_settingsStore);

    // Set scope for authorization
    m_o2spotify->setScope(SPOTIFY_SCOPE);

    // Port for local webserver
    m_o2spotify->setLocalPort(SPOTIFY_LOCAL_PORT);

    // Connect signals
    connect(m_o2spotify, &O2Spotify::linkingSucceeded, this, &SpotifyConnectorLocal::onLinkingSucceeded);
    connect(m_o2spotify, &O2Spotify::openBrowser,      this, &SpotifyConnectorLocal::onOpenBrowser);
}

SpotifyConnectorLocal::~SpotifyConnectorLocal()
{
    qCDebug(gmSpotifyLocal()) << "Closing spotify connector ...";
    disconnect();
    m_o2spotify->deleteLater();
    m_settingsStore->deleteLater();
}

void SpotifyConnectorLocal::grantAccess()
{
    qCDebug(gmSpotifyLocal) << "Granting access ...";

    QString id     = m_sManager.getSetting(Setting::spotifyID);
    QString secret = m_sManager.getSetting(Setting::spotifySecret);

    m_o2spotify->setClientId(id);
    m_o2spotify->setClientSecret(secret);

    if (!id.isEmpty() && !secret.isEmpty())
    {
        qCDebug(gmSpotifyLocal) << "Found client id and secret. Trying to link now...";
        m_o2spotify->link();
    }
    else
    {
        qCCritical(gmSpotifyLocal) << "Client id and/or client secret not found!";
    }
}

int SpotifyConnectorLocal::get(QNetworkRequest request)
{
    // If amount of concurrent requests is too high, enqueue request
    if ((m_currentRequestCount >= SPOTIFY_MAX_REQUESTS) || m_onCooldown) {
        qCDebug(gmSpotifyLocal) << "Current request count too high, putting request in queue ...";

        auto requestId = getUniqueRequestId();
        m_requestQueue.enqueue(QPair<QNetworkRequest, int>(request, requestId));
        return requestId;
    }

    qCDebug(gmSpotifyLocal) << "Sending GET Request to URL" << request.url();

    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            this, &SpotifyConnectorLocal::onReplyReceived);

    auto requestId  = getUniqueRequestId();
    auto internalId = requestor->get(request);
    m_requestMap[internalId] = QPair<QNetworkRequest, int>(request, requestId);
    m_currentRequestCount++;

    return requestId;
}

void SpotifyConnectorLocal::get(QNetworkRequest request, int requestId)
{
    qCDebug(gmSpotifyLocal) << "Sending GET Request to URL" << request.url();

    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            this, &SpotifyConnectorLocal::onReplyReceived);

    auto internalId = requestor->get(request);
    m_requestMap[internalId] = QPair<QNetworkRequest, int>(request, requestId);
    m_currentRequestCount++;
}

int SpotifyConnectorLocal::put(QUrl url, QString params)
{
    qCDebug(gmSpotifyLocal) << "Sending PUT Request to URL" << url << "and parameters:" << params;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);

    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            this, &SpotifyConnectorLocal::onReplyReceived);

    auto requestId  = getUniqueRequestId();
    auto internalId = requestor->put(request, params.toLocal8Bit());
    m_requestMap[internalId] = QPair<QNetworkRequest, int>(request, requestId);
    m_currentRequestCount++;

    return requestId;
}

int SpotifyConnectorLocal::post(QNetworkRequest request, QByteArray data)
{
    qCDebug(gmSpotifyLocal) << "Sending POST Request to URL" << request.url() << "with data" << data;

    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            this, &SpotifyConnectorLocal::onReplyReceived);

    auto requestId  = getUniqueRequestId();
    auto internalId = requestor->post(request, data);
    m_requestMap[internalId] = QPair<QNetworkRequest, int>(request, requestId);
    m_currentRequestCount++;

    return requestId;
}

void SpotifyConnectorLocal::onReplyReceived(int id, QNetworkReply::NetworkError error, QByteArray data)
{
    m_currentRequestCount--;

    // Check if rate limit was exceeded
    if (error == QNetworkReply::UnknownContentError) {
        auto status = QJsonDocument::fromJson(data).object().value("error").toObject().value("status").toInt();

        // HTTP 429 == Rate limit exceeded
        if (status == 429) {
            handleRateLimit(m_requestMap[id]);
            return;
        }
    }

    // Emit signal with non-internal request id
    auto requestId = m_requestMap[id].second;
    emit receivedReply(requestId, error, data);

    // If there are requests in queue, send next
    dequeueRequests();

    m_requestMap.remove(id);
    sender()->deleteLater();
}

void SpotifyConnectorLocal::dequeueRequests()
{
    qDebug() << "Dequeueing requests ..." << m_requestQueue.count();

    while (m_currentRequestCount < SPOTIFY_MAX_REQUESTS && !m_requestQueue.empty()) {
        auto pair = m_requestQueue.dequeue();
        get(pair.first, pair.second);
    }
}

void SpotifyConnectorLocal::handleRateLimit(QPair<QNetworkRequest, int>requestPair)
{
    qCDebug(gmSpotifyLocal) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";
    startCooldown(2);
    m_requestQueue.enqueue(requestPair);
}

void SpotifyConnectorLocal::startCooldown(int seconds)
{
    m_onCooldown = true;
    QTimer::singleShot(seconds * 1000, this, &SpotifyConnectorLocal::onCooldownFinished);
}

void SpotifyConnectorLocal::onCooldownFinished()
{
    m_onCooldown = false;
    dequeueRequests();
}

void SpotifyConnectorLocal::onLinkingSucceeded()
{
    if (!m_o2spotify->linked()) return;

    emit accessGranted();
}

void SpotifyConnectorLocal::onOpenBrowser(QUrl url)
{
    QDesktopServices::openUrl(url);
}
