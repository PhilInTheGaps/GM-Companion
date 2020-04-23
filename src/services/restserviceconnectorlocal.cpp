#include "restserviceconnectorlocal.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "o0globals.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>

/**
 * @brief Constructor
 */
RESTServiceConnectorLocal::RESTServiceConnectorLocal
    (QNetworkAccessManager *networkManager, O2 *o2, const QLoggingCategory& loggingCategory, QObject *parent = nullptr) :
    RESTServiceConnector(networkManager, loggingCategory, parent), m_o2(o2)
{
    m_settingsStore = new O0SettingsStore("gm-companion");
    m_o2->setStore(m_settingsStore);

    // Connect signals
    connect(m_o2, &O2::linkingSucceeded, this, &RESTServiceConnectorLocal::onLinkingSucceeded);
    connect(m_o2, &O2::openBrowser,      this, &RESTServiceConnectorLocal::onOpenBrowser);
    connect(m_o2, &O2::refreshFinished,  this, &RESTServiceConnectorLocal::onRefreshFinished);
}

/**
 * @brief Destructor
 */
RESTServiceConnectorLocal::~RESTServiceConnectorLocal()
{
    disconnect();
    m_o2->deleteLater();
    m_settingsStore->deleteLater();
}

/**
 * @brief Configure parameters like scope, port etc.
 */
void RESTServiceConnectorLocal::setConfig(const RESTServiceLocalConfig& config)
{
    qCDebug(m_loggingCategory) << "Configuring rest service connector ...";
    m_config = config;
    m_o2->setScope(config.scope);
    m_o2->setLocalPort(config.port);
    m_wasConfigured = true;
}

/**
 * @brief Start the linking process
 */
void RESTServiceConnectorLocal::grantAccess()
{
    qCDebug(m_loggingCategory) << "Granting access ...";
    emit statusChanged(tr("Connecting..."));

    if (!m_wasConfigured)
    {
        qCWarning(m_loggingCategory) << "Could not grant access, connector was not configured.";
        emit statusChanged(tr("Internal Error: Connector was not configured."));
        return;
    }

    auto id     = SettingsManager::getSetting(m_config.idRequest);
    auto secret = SettingsManager::getSetting(m_config.secretRequest);

    m_o2->setClientId(id);
    m_o2->setClientSecret(secret);

    if (!id.isEmpty() && !secret.isEmpty())
    {
        qCDebug(m_loggingCategory) << "Found client id and secret. Trying to link now ...";
        m_o2->link();
    }
    else
    {
        qCWarning(m_loggingCategory) << "Client id and/or client secret not found!";

        if (id.isEmpty())
        {
            emit statusChanged(tr("Error: No Client ID has been set."));
        }
        else
        {
            emit statusChanged(tr("Error: No Client Secret has been set."));
        }
    }
}

/**
 * @brief Send a GET request to the REST API
 * @return Returns a unique request id that can be used to identify the reply
 * later
 */
auto RESTServiceConnectorLocal::get(QNetworkRequest request)->int
{
    auto requestId = getUniqueRequestId();

    get(request, requestId);
    return requestId;
}

/**
 * @brief Send a GET request to the REST API but with a specified request id
 */
void RESTServiceConnectorLocal::get(QNetworkRequest request, int requestId)
{
    // If amount of concurrent requests is too high, enqueue request
    auto wasEnqueued = checkAndEnqueueRequest(request, requestId, GET, "");

    if (wasEnqueued > -1) return;

    qCDebug(m_loggingCategory) << "Sending GET Request (" << requestId << ") to URL" << request.url();

    auto requestor = new O2Requestor(m_networkManager, m_o2, this);

    // Workaround for google drive requests
    if (!m_config.authHeaderFormat.isEmpty())
    {
        requestor->setAddAccessTokenInQuery(false);
        requestor->setAccessTokenInAuthenticationHTTPHeaderFormat(m_config.authHeaderFormat);
    }

    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair> >::of(&O2Requestor::finished), this,
            &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            requestor, &O2Requestor::deleteLater);

    auto internalId = requestor->get(request);

    if (internalId == -1) qCWarning(m_loggingCategory) << "Error: could not start requestor";

    m_requestMap[internalId] = RequestContainer(requestId, request, GET, "");
    m_currentRequestCount++;
}

/**
 * @brief Check if request can be sent or if requests are blocked. If they are
 * blocked, put request in queue.
 * @param request The network request
 * @param requestId Request id, if left at -1 will generate a new id
 * @return Returns request id for network request or -1 if request can be sent
 * now
 */
auto RESTServiceConnectorLocal::checkAndEnqueueRequest(const QNetworkRequest& request, int requestId, RequestType type, QByteArray data, QByteArray verb)->int
{
    if (m_isOnCooldown)
    {
        qCDebug(m_loggingCategory).noquote() << "Connector is on cooldown, putting request (" << requestId << ") in queue ...";
    }
    else if (!isAccessGranted())
    {
        qCDebug(m_loggingCategory) << "Access has not been granted, putting request (" << requestId << ") in queue ...";
    }
    else if (m_currentRequestCount >= m_config.maxConcurrentRequests)
    {
        qCDebug(m_loggingCategory).noquote() << "Current request count (" << m_currentRequestCount << ") too high ("
                                             << m_config.maxConcurrentRequests << "are allowed ), putting request ("
                                             << requestId << ") in queue ...";
    }
    else
    {
        return -1;
    }

    if (requestId < 0) requestId = getUniqueRequestId();
    m_requestQueue.enqueue(RequestContainer(requestId, request, type, std::move(data), std::move(verb)));
    return requestId;
}

auto RESTServiceConnectorLocal::put(QNetworkRequest request, QByteArray data)->int
{
    auto requestId = getUniqueRequestId();

    put(request, data, requestId);
    return requestId;
}

void RESTServiceConnectorLocal::put(QNetworkRequest request, QByteArray data, int requestId)
{
    // If amount of concurrent requests is too high, enqueue request
    auto wasEnqueued = checkAndEnqueueRequest(request, requestId, PUT, data);

    if (wasEnqueued > -1) return;

    qCDebug(m_loggingCategory) << "Sending PUT Request (" << requestId << ") to URL" << request.url() << "and data:" << data;

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);

    auto requestor = new O2Requestor(m_networkManager, m_o2, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair> >::of(&O2Requestor::finished), this,
            &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            requestor, &O2Requestor::deleteLater);

    auto internalId = requestor->put(request, data);

    if (internalId == -1) qCWarning(m_loggingCategory) << "Error: could not start requestor";

    m_requestMap[internalId] = RequestContainer(requestId, request, PUT, data);
    m_currentRequestCount++;
}

auto RESTServiceConnectorLocal::post(QNetworkRequest request, QByteArray data)->int
{
    auto requestId = getUniqueRequestId();

    post(request, data, requestId);
    return requestId;
}

void RESTServiceConnectorLocal::post(QNetworkRequest request, QByteArray data, int requestId)
{
    // If amount of concurrent requests is too high, enqueue request
    auto wasEnqueued = checkAndEnqueueRequest(request, requestId, POST, data);

    if (wasEnqueued > -1) return;

    qCDebug(m_loggingCategory) << "Sending POST Request (" << requestId << ") to URL" << request.url() << "with data" << data;

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    auto requestor = new O2Requestor(m_networkManager, m_o2, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair> >::of(&O2Requestor::finished), this,
            &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            requestor, &O2Requestor::deleteLater);
    auto internalId = requestor->post(request, data);

    if (internalId == -1) qCWarning(m_loggingCategory) << "Error: could not start requestor";

    m_requestMap[internalId] = RequestContainer(requestId, request, POST, data);
    m_currentRequestCount++;
}

void RESTServiceConnectorLocal::customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data, int requestId)
{
    // If amount of concurrent requests is too high, enqueue request
    auto wasEnqueued = checkAndEnqueueRequest(request, requestId, CUSTOM, data, verb);

    if (wasEnqueued > -1) return;

    auto requestor = new O2Requestor(m_networkManager, m_o2, this);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair> >::of(&O2Requestor::finished), this,
            &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            requestor, &O2Requestor::deleteLater);

    auto internalId = requestor->customRequest(request, verb, data);

    if (internalId == -1) qCWarning(m_loggingCategory) << "Error: could not start requestor";

    m_requestMap[internalId] = RequestContainer(requestId, request, CUSTOM, data);
    m_currentRequestCount++;
}

void RESTServiceConnectorLocal::onReplyReceived(int internalId, QNetworkReply::NetworkError error, const QByteArray& data, QList<QNetworkReply::RawHeaderPair>headers)
{
    qCDebug(m_loggingCategory) << "Received reply with internal id" << internalId;
    m_currentRequestCount--;

    // Check if rate limit was exceeded
    if (error == QNetworkReply::UnknownContentError) {
        auto status = QJsonDocument::fromJson(data).object()["error"].toObject()["status"].toInt();

        // HTTP 429 == Rate limit exceeded
        if (status == 429) {
            handleRateLimit(m_requestMap[internalId]);
            return;
        }
    }

    // Emit signal with non-internal request id
    auto externalId = m_requestMap[internalId].requestId;
    emit receivedReply(externalId, error, data, std::move(headers));

    // If there are requests in queue, send next
    dequeueRequests();

    m_requestMap.remove(internalId);
    sender()->deleteLater();
}

void RESTServiceConnectorLocal::onRefreshFinished(const QNetworkReply::NetworkError& error)
{
    if (error != QNetworkReply::NoError)
    {
        emit statusChanged(tr("Error: Could not refresh token."));
    }
}

void RESTServiceConnectorLocal::dequeueRequests()
{
    qCDebug(m_loggingCategory) << "Dequeueing requests ..." << m_requestQueue.count();

    QQueue<RequestContainer> tempQueue;

    while (tempQueue.length() < m_config.maxConcurrentRequests - m_currentRequestCount && !m_requestQueue.empty())
    {
        tempQueue.enqueue(m_requestQueue.dequeue());
    }

    while (!tempQueue.isEmpty())
    {
        auto container = tempQueue.dequeue();

        switch (container.requestType)
        {
        case GET: get(container.request, container.requestId);
            break;

        case PUT: put(container.request, container.data, container.requestId);
            break;

        case POST: post(container.request, container.data, container.requestId);
            break;

        case CUSTOM: customRequest(container.request, container.verb, container.data, container.requestId);
            break;
        }
    }
}

void RESTServiceConnectorLocal::handleRateLimit(const RequestContainer& container)
{
    qCDebug(m_loggingCategory) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";
    startCooldown(2);
    m_requestQueue.enqueue(container);
}

void RESTServiceConnectorLocal::startCooldown(int seconds)
{
    m_isOnCooldown = true;
    QTimer::singleShot(seconds * 1000, this, &RESTServiceConnectorLocal::onCooldownFinished);
}

void RESTServiceConnectorLocal::onCooldownFinished()
{
    qCDebug(m_loggingCategory) << "Cooldown finished.";
    m_isOnCooldown = false;
    dequeueRequests();
}

void RESTServiceConnectorLocal::onLinkingSucceeded()
{
    if (!m_o2->linked()) return;

    emit statusChanged(tr("Connected."));
    emit accessGranted();

    dequeueRequests();
}

void RESTServiceConnectorLocal::onOpenBrowser(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}
