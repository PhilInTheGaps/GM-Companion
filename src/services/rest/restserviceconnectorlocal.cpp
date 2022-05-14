#include "restserviceconnectorlocal.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "o0globals.h"
#include "thirdparty/http-status-codes/HttpStatusCodes_Qt.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <utility>

/**
 * @brief Constructor
 */
RESTServiceConnectorLocal::RESTServiceConnectorLocal
    (QNetworkAccessManager *networkManager, O2 *o2,
     const QLoggingCategory& loggingCategory, QObject *parent = nullptr) :
    RESTServiceConnector(networkManager, loggingCategory, parent), m_o2(o2)
{
    m_settingsStore = new O0SettingsStore("gm-companion", this);
    m_o2->setParent(this);
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
    setStatus(ServiceStatus::Type::Info, tr("Connecting..."));

    if (!m_wasConfigured)
    {
        setStatus(ServiceStatus::Type::Error, tr("Internal Error: Connector was not configured."));
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
        if (id.isEmpty())
        {
            setStatus(ServiceStatus::Type::Error, tr("Error: No Client ID has been set."));
        }
        else
        {
            setStatus(ServiceStatus::Type::Error, tr("Error: No Client Secret has been set."));
        }
    }
}

void RESTServiceConnectorLocal::disconnectService()
{
    qCDebug(m_loggingCategory) << "Disconnecting ...";
    m_o2->unlink();
}

void RESTServiceConnectorLocal::sendRequest(RequestContainer *container, AsyncFuture::Deferred<RestNetworkReply *> deferred)
{
    // If amount of concurrent requests is too high, enqueue request
    if (checkAndEnqueueRequest(container, deferred)) return;

    auto *requestor = makeRequestor();
    auto request = container->request();
    int id = -1;

    switch (container->requestType())
    {
    case GET:
        qCDebug(m_loggingCategory) << "Sending GET Request to URL" << container->request().url();

        // Workaround for google drive requests
        if (!m_config.authHeaderFormat.isEmpty())
        {
            requestor->setAddAccessTokenInQuery(false);
            requestor->setAccessTokenInAuthenticationHTTPHeaderFormat(m_config.authHeaderFormat);
        }

        id = requestor->get(request);
        break;

    case PUT:
        qCDebug(m_loggingCategory) << "Sending PUT Request to URL" << request.url() << "with data" << container->data();
        //request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);
        id = requestor->put(request, container->data());
        break;

    case POST:
        qCDebug(m_loggingCategory) << "Sending POST Request to URL" << request.url() << "with data" << container->data();
        //request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
        id = requestor->post(request, container->data());
        qCDebug(m_loggingCategory) << "Sent POST request:" << id;
        break;

    case CUSTOM:
        qCDebug(m_loggingCategory) << "Sending HTTP request:" << container->verb(), request.url();
        id = requestor->customRequest(request, container->verb(), container->data());
        break;
    }

    if (id == -1)
    {
        qCWarning(m_loggingCategory) << "Error: could not start requestor!";
        deferred.cancel();
        container->deleteLater();
    }
    else
    {
        m_activeRequests[id] = { deferred, container };
    }
}

/**
 * @brief Send a GET request to the REST API
 */
auto RESTServiceConnectorLocal::get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>
{
    auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
    auto *container = new RequestContainer(request, GET, "", this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto RESTServiceConnectorLocal::put(QNetworkRequest request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
    auto *container = new RequestContainer(request, PUT, data, this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto RESTServiceConnectorLocal::post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
    auto *container = new RequestContainer(request, POST, data, this);

    sendRequest(container, deferred);
    return deferred.future();
}

auto RESTServiceConnectorLocal::customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data) -> QFuture<RestNetworkReply *>
{
    auto deferred = AsyncFuture::deferred<RestNetworkReply*>();
    auto *container = new RequestContainer(request, CUSTOM, data, verb, this);

    sendRequest(container, deferred);
    return deferred.future();
}

/**
 * @brief Check if request can be sent or if requests are blocked.
 * If they are blocked, put request in queue.
 * @return Returns true if the request has been enqueued
 */
auto RESTServiceConnectorLocal::checkAndEnqueueRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply*> &deferred) -> bool
{
    if (m_isOnCooldown)
    {
        qCDebug(m_loggingCategory).noquote() << "Connector is on cooldown, putting request in queue ...";
    }
    else if (!isAccessGranted())
    {
        qCDebug(m_loggingCategory) << "Access has not been granted, putting request in queue ...";
    }
    else if (activeRequestCount() >= m_config.maxConcurrentRequests)
    {
        qCDebug(m_loggingCategory).noquote() << "Current request count (" << activeRequestCount() << ") too high ("
                                             << m_config.maxConcurrentRequests << "are allowed ), putting request in queue ...";
    }
    else
    {
        return false;
    }

    container->id(getQueueId());
    m_requestQueue.enqueue(QPair(deferred, container));

    return true;
}

auto RESTServiceConnectorLocal::makeRequestor() -> O2Requestor*
{
    auto *requestor = new O2Requestor(m_networkManager, m_o2, this);

    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QString, QByteArray, QList<QNetworkReply::RawHeaderPair> >::of(&O2Requestor::finished),
            this, &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished),
            requestor, &O2Requestor::deleteLater);

    return requestor;
}

void RESTServiceConnectorLocal::onReplyReceived(int id, QNetworkReply::NetworkError error, const QString &errorText,
                                                const QByteArray& data, const QList<QNetworkReply::RawHeaderPair>&headers)
{
    qCDebug(m_loggingCategory) << "Received reply with internal id" << id;

    // Check if rate limit was exceeded
    if (error == QNetworkReply::UnknownContentError)
    {
        const auto status = QJsonDocument::fromJson(data).object()["error"].toObject()["status"].toInt();

        if (status == HttpStatus::TooManyRequests)
        {
            handleRateLimit(m_activeRequests[id]);
            return;
        }
    }
    // Sometimes services (like google drive) hide rate limit errors in 403s
    else if (error == QNetworkReply::ContentAccessDenied)
    {
        const auto error = QJsonDocument::fromJson(data).object()["error"].toObject();

        if (error.contains("errors"))
        {
            const auto errors = error["errors"].toArray();

            for (const auto &entry : errors)
            {
                if (entry["reason"].toString() == "userRateLimitExceeded")
                {
                    handleRateLimit(m_activeRequests[id]);
                    return;
                }
            }
        }
    }

    if (error != QNetworkReply::NoError)
    {
        qCWarning(m_loggingCategory) << error << errorText << data << headers;
    }

    // Finish
    auto *result = new RestNetworkReply(error, errorText, data, headers, this);
    const auto pair = m_activeRequests.take(id);
    auto *container = pair.second;
    auto deferred = pair.first;
    deferred.complete(result);

    // If there are requests in queue, send next
    dequeueRequests();

    // Delete O2Requestor and Container
    container->deleteLater();
    sender()->deleteLater();
}

void RESTServiceConnectorLocal::onRefreshFinished(const QNetworkReply::NetworkError& error)
{
    if (error != QNetworkReply::NoError)
    {
        setStatus(ServiceStatus::Type::Error, tr("Error: Could not refresh token."));
    }
}

void RESTServiceConnectorLocal::dequeueRequests()
{
    qCDebug(m_loggingCategory) << "Dequeueing requests ..." << m_requestQueue.count();

    QQueue<QPair<AsyncFuture::Deferred<RestNetworkReply*>, RequestContainer*>> tempQueue;

    while (tempQueue.length() < m_config.maxConcurrentRequests - activeRequestCount() && !m_requestQueue.empty())
    {
        tempQueue.enqueue(m_requestQueue.dequeue());
    }

    while (!tempQueue.isEmpty())
    {
        auto pair = tempQueue.dequeue();

        sendRequest(pair.second, std::move(pair.first));
    }
}

auto RESTServiceConnectorLocal::getQueueId() -> int
{
    if (m_nextQueueId == 0)
    {
        m_nextQueueId++;
    }

    return m_nextQueueId++;
}

void RESTServiceConnectorLocal::handleRateLimit(const QPair<AsyncFuture::Deferred<RestNetworkReply *>, RequestContainer *> &pair)
{
    qCDebug(m_loggingCategory) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";
    startCooldown(2);
    m_requestQueue.enqueue(pair);
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
    if (!m_o2->linked())
    {
        emit isConnectedChanged(false);
        return;
    }

    emit isConnectedChanged(true);
    emit accessGranted();

    dequeueRequests();
}

void RESTServiceConnectorLocal::onOpenBrowser(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}
