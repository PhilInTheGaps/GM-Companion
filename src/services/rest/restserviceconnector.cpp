#include "restserviceconnector.h"
#include "thirdparty/http-status-codes/HttpStatusCodes_Qt.h"
#include <QDateTime>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto RATELIMIT_TIMEOUT = std::chrono::seconds(2);

RESTServiceConnector::RESTServiceConnector(QNetworkAccessManager *networkManager,
                                           const QLoggingCategory &loggingCategory, QStringList &&recoverableErrors,
                                           QObject *parent)
    : QObject(parent), m_loggingCategory(loggingCategory), m_networkManager(networkManager),
      m_recoverableErrors(std::move(recoverableErrors))
{
}

auto RESTServiceConnector::networkManager() const -> QNetworkAccessManager *
{
    return m_networkManager;
}

void RESTServiceConnector::setNetworkManager(QNetworkAccessManager *networkManager)
{
    m_networkManager = networkManager;
}

auto RESTServiceConnector::isOnCooldown() const -> bool
{
    return m_isOnCooldown;
}

void RESTServiceConnector::startCooldown(std::chrono::seconds duration)
{
    using namespace std::chrono;
    m_isOnCooldown = true;
    QTimer::singleShot(duration, this, &RESTServiceConnector::onCooldownFinished);
}

void RESTServiceConnector::onCooldownFinished()
{
    qCDebug(m_loggingCategory) << "Cooldown finished.";
    m_isOnCooldown = false;
    dequeueRequests();
}

void RESTServiceConnector::onReplyReceived(int id, QNetworkReply::NetworkError error, const QString &errorText,
                                           const QByteArray &data, const QList<QNetworkReply::RawHeaderPair> &headers)
{
    qCDebug(m_loggingCategory) << "Received reply with internal id" << id;

    auto pair = std::move(m_activeRequests.extract(id).mapped());

    if (wasRateLimitReached(error, data))
    {
        handleRateLimit(std::move(pair), headers);
        return;
    }

    if (error != QNetworkReply::NoError)
    {
        bool logAsError = true;

        // Some errors we can easily recover from, so we only log them as debug messages
        foreach (const auto &_error, m_recoverableErrors)
        {
            if (data.contains(_error.toUtf8()))
            {
                qCDebug(m_loggingCategory) << errorText << data;
                logAsError = false;
                break;
            }
        }

        if (logAsError)
        {
            qCWarning(m_loggingCategory) << error << errorText << data << headers;
        }
    }

    // Finish
    auto promise = std::move(pair.first);
    promise.addResult(RestReply(error, errorText, data, headers));
    promise.finish();

    // If there are requests in queue, send next
    dequeueRequests();
}

void RESTServiceConnector::setStatus(Status::Type type, const QString &message)
{
    switch (type)
    {
    case Status::Type::Warning:
        qCWarning(m_loggingCategory) << message;
        break;
    case Status::Type::Error:
        qCCritical(m_loggingCategory) << message;
        break;
    default:
        qCDebug(m_loggingCategory) << message;
    }

    emit statusChanged(type, message);
}

void RESTServiceConnector::dequeueRequests()
{
    qCDebug(m_loggingCategory) << "Dequeueing requests ..." << m_requestQueueHighPriority.size()
                               << m_requestQueueLowPriority.size();

    QString reason;

    while (canSendRequest(reason) && !m_requestQueueHighPriority.empty())
    {
        auto [reply, request] = std::move(m_requestQueueHighPriority.front());
        m_requestQueueHighPriority.pop();

        sendRequest(std::move(request), std::move(reply));
    }

    if (!reason.isEmpty()) qCDebug(m_loggingCategory) << "Could not dequeue high priority requests:" << reason;

    while (canSendRequest(reason) && !m_requestQueueLowPriority.empty())
    {
        auto [reply, request] = std::move(m_requestQueueLowPriority.front());
        m_requestQueueLowPriority.pop();

        sendRequest(std::move(request), std::move(reply));
    }

    if (!reason.isEmpty()) qCDebug(m_loggingCategory) << "Could not dequeue low priority requests:" << reason;
}

auto RESTServiceConnector::canSendRequest(QString &reason) -> bool
{
    if (isOnCooldown())
    {
        reason = u"Connector is on cooldown"_s;
        return false;
    }

    if (!isAccessGranted())
    {
        reason = u"Access has not been granted"_s;
        return false;
    }

    if (activeRequestCount() >= maxConcurrentRequests())
    {
        reason = u"Current request count (%1) too high (%2 are allowed)"_s.arg(activeRequestCount())
                     .arg(maxConcurrentRequests());
        return false;
    }

    if (isTokenExpired() || getAccessToken().isEmpty())
    {
        refreshAccessToken();
        return false;
    }

    return true;
}

auto RESTServiceConnector::enqueueRequest(RestRequest &&request, QPromise<RestReply> &&reply) -> QFuture<RestReply>
{
    auto future = reply.future();
    request.id(m_nextQueueId);
    m_nextQueueId++;

    if (request.options().testFlag(Option::LowPriority))
    {
        m_requestQueueLowPriority.emplace(std::move(reply), std::move(request));
    }
    else
    {
        m_requestQueueHighPriority.emplace(std::move(reply), std::move(request));
    }

    // try to dispatch request
    dequeueRequests();
    return future;
}

auto RESTServiceConnector::markRequestActive(RestRequest &&request, QPromise<RestReply> &&reply) -> QFuture<RestReply>
{
    auto id = request.id();
    auto future = reply.future();
    m_activeRequests.try_emplace(id, std::move(reply), std::move(request));
    return future;
}

auto RESTServiceConnector::maxConcurrentRequests() const -> size_t
{
    return m_maxConcurrentRequests;
}

void RESTServiceConnector::setMaxConcurrentRequests(int count)
{
    m_maxConcurrentRequests = count;
}

void RESTServiceConnector::updateTokenExpireTime(std::chrono::seconds expiresIn)
{
    m_tokenExpireTime = QDateTime::currentDateTime().addDuration(expiresIn);
}

auto RESTServiceConnector::isTokenExpired() const -> bool
{
    return !m_tokenExpireTime.isValid() || QDateTime::currentDateTime() > m_tokenExpireTime;
}

auto RESTServiceConnector::activeRequestCount() const -> size_t
{
    return m_activeRequests.size();
}

auto RESTServiceConnector::wasRateLimitReached(QNetworkReply::NetworkError error, const QByteArray &data) const -> bool
{
    if (error == QNetworkReply::UnknownContentError)
    {
        const auto status = QJsonDocument::fromJson(data).object()["error"_L1]["status"_L1].toInt();
        return status == HttpStatus::TooManyRequests;
    }

    // Sometimes services (like google drive) hide rate limit errors in 403s
    if (error == QNetworkReply::ContentAccessDenied)
    {
        const auto errorObject = QJsonDocument::fromJson(data).object()["error"_L1].toObject();

        if (errorObject.contains("errors"_L1))
        {
            foreach (const auto &entry, errorObject["errors"_L1].toArray())
            {
                return entry["reason"_L1].toString() == "userRateLimitExceeded"_L1;
            }
        }
    }

    return false;
}

void RESTServiceConnector::handleRateLimit(std::pair<QPromise<RestReply>, RestRequest> &&pair,
                                           const QList<QNetworkReply::RawHeaderPair> &headers)
{
    qCDebug(m_loggingCategory) << "Rate limit was exceeded, setting cooldown and rescheduling request ...";

    auto timeout = RATELIMIT_TIMEOUT;

    foreach (const auto &header, headers)
    {
        if (header.first == "Retry-After")
        {
            // try to interpret retry-after as seconds first
            bool ok = false;
            timeout = std::chrono::seconds(header.second.toInt(&ok));

            // if that did not work, it is probably a datetime string
            if (!ok)
            {
                auto datetime = QDateTime::fromString(header.second);
                timeout = std::chrono::seconds(QDateTime::currentDateTime().secsTo(datetime));
            }

            break;
        }
    }

    startCooldown(timeout);
    m_requestQueueHighPriority.push(std::move(pair));
}
