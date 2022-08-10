#pragma once

#include <QQueue>

#include "restserviceconnector.h"
#include "o2.h"
#include "o2requestor.h"
#include "o0settingsstore.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

struct RESTServiceLocalConfig
{
    QString scope;
    int port;
    int maxConcurrentRequests;
    QString authHeaderFormat;
    SettingRequest<QString> idRequest;
    SettingRequest<QString> secretRequest;
};

class RESTServiceConnectorLocal : public RESTServiceConnector
{
    Q_OBJECT
public:
    RESTServiceConnectorLocal(QNetworkAccessManager* networkManager, O2 *o2, const QLoggingCategory &loggingCategory, QObject *parent);
    virtual ~RESTServiceConnectorLocal() override;

    void grantAccess() override;
    void disconnectService() override;
    bool isAccessGranted() const override { return m_o2->linked(); }

    QFuture<RestNetworkReply*> get(const QNetworkRequest &request) override;
    QFuture<RestNetworkReply*> put(QNetworkRequest request, const QByteArray &data) override;
    QFuture<RestNetworkReply*> post(QNetworkRequest request, const QByteArray &data) override;
    QFuture<RestNetworkReply*> customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data) override;

protected:
    O2 *m_o2 = nullptr;
    O0SettingsStore *m_settingsStore = nullptr;
    RESTServiceLocalConfig m_config;

    void setConfig(const RESTServiceLocalConfig& config);
    void handleRateLimit(const QPair<AsyncFuture::Deferred<RestNetworkReply *>, RequestContainer*> &pair);
    void startCooldown(int seconds);
    void dequeueRequests();

    bool m_isOnCooldown = false;

    QMap<int, QPair<AsyncFuture::Deferred<RestNetworkReply*>, RequestContainer*>> m_activeRequests;
    QQueue<QPair<AsyncFuture::Deferred<RestNetworkReply*>, RequestContainer*>> m_requestQueue;

    int activeRequestCount() const { return m_activeRequests.count(); };
    int getQueueId();

private:
    bool checkAndEnqueueRequest(RequestContainer *container, const AsyncFuture::Deferred<RestNetworkReply *> &deferred);
    O2Requestor* makeRequestor();

    void sendRequest(RequestContainer *container, AsyncFuture::Deferred<RestNetworkReply *> deferred);

    int m_nextQueueId = 1;

protected slots:
    void onLinkingSucceeded();
    static void onOpenBrowser(const QUrl& url);

    void onCooldownFinished();
    void onReplyReceived(int id, QNetworkReply::NetworkError error, const QString &errorText,
                         const QByteArray& data, const QList<QNetworkReply::RawHeaderPair>& headers);
    void onRefreshFinished(const QNetworkReply::NetworkError& error);
};
