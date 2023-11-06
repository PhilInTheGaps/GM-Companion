#pragma once

#include "restreply.h"
#include "restrequest.h"
#include "servicestatus.h"
#include <QDateTime>
#include <QFuture>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPromise>
#include <chrono>
#include <memory>
#include <queue>
#include <unordered_map>

namespace Services
{

class RESTServiceConnector : public QObject
{
    Q_OBJECT
public:
    RESTServiceConnector(QNetworkAccessManager &networkManager, const QLoggingCategory &loggingCategory,
                         QStringList &&recoverableErrors, QObject *parent = nullptr);

    virtual void grantAccess() = 0;
    virtual void disconnectService() = 0;
    [[nodiscard]] virtual auto isAccessGranted() const -> bool = 0;

    virtual auto get(const QNetworkRequest &request, bool isAuthRequired) -> QFuture<RestReply> = 0;
    virtual auto put(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply> = 0;
    virtual auto post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply> = 0;
    virtual auto customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data)
        -> QFuture<RestReply> = 0;

signals:
    void accessGranted();
    void statusChanged(const Status::Type &type, const QString &message);
    void isConnectedChanged(const bool &connected);

protected slots:
    void onCooldownFinished();
    void onReplyReceived(int id, QNetworkReply::NetworkError error, const QString &errorText, const QByteArray &data,
                         const QList<QNetworkReply::RawHeaderPair> &headers);

protected:
    [[nodiscard]] auto isOnCooldown() const -> bool;
    void startCooldown(std::chrono::seconds duration);

    void setStatus(Status::Type type, const QString &message);

    void dequeueRequests();
    [[nodiscard]] auto canSendRequest(QString &reason) -> bool;
    auto enqueueRequest(RestRequest &&request, QPromise<RestReply> &&reply) -> QFuture<RestReply>;
    auto markRequestActive(RestRequest &&request, QPromise<RestReply> &&reply) -> QFuture<RestReply>;
    virtual void sendRequest(RestRequest &&request, QPromise<RestReply> &&reply) = 0;

    [[nodiscard]] auto maxConcurrentRequests() const -> size_t;
    void setMaxConcurrentRequests(int count);

    [[nodiscard]] virtual auto getAccessToken() -> QString
    {
        return QLatin1String();
    };
    virtual void refreshAccessToken(bool updateAuthentication = false) = 0;
    void updateTokenExpireTime(std::chrono::seconds expiresIn);
    [[nodiscard]] auto isTokenExpired() const -> bool;

    QNetworkAccessManager &m_networkManager;
    const QLoggingCategory &m_loggingCategory;
    bool m_wasConfigured = false;

private:
    [[nodiscard]] auto activeRequestCount() const -> size_t;

    [[nodiscard]] auto wasRateLimitReached(QNetworkReply::NetworkError error, const QByteArray &data) const -> bool;
    void handleRateLimit(std::pair<QPromise<RestReply>, RestRequest> &&pair,
                         const QList<QNetworkReply::RawHeaderPair> &headers);

    bool m_isOnCooldown = false;
    size_t m_maxConcurrentRequests = 1;
    int m_nextQueueId = 1;
    QStringList m_recoverableErrors;
    QDateTime m_tokenExpireTime;

    std::unordered_map<int, std::pair<QPromise<RestReply>, RestRequest>> m_activeRequests;
    std::queue<std::pair<QPromise<RestReply>, RestRequest>> m_requestQueue;
};

} // namespace Services
