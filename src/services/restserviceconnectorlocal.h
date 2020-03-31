#ifndef RESTSERVICECONNECTORLOCAL_H
#define RESTSERVICECONNECTORLOCAL_H

#include <QQueue>

#include "restserviceconnector.h"
#include "o2.h"
#include "o2requestor.h"
#include "o0settingsstore.h"

struct RESTServiceLocalConfig {
    QString scope;
    int port;
    int maxConcurrentRequests;
    QString authHeaderFormat;
    SettingRequest idRequest;
    SettingRequest secretRequest;
};

class RESTServiceConnectorLocal : public RESTServiceConnector
{
    Q_OBJECT
public:
    RESTServiceConnectorLocal(QNetworkAccessManager* networkManager, O2 *o2, const QLoggingCategory &loggingCategory, QObject *parent);
    virtual ~RESTServiceConnectorLocal();

    void grantAccess();
    bool isAccessGranted() const { return m_o2->linked(); }

    int get(QNetworkRequest request);
    void get(QNetworkRequest request, int requestId);

    int put(QNetworkRequest request, QByteArray data = "");
    void put(QNetworkRequest request, QByteArray data, int requestId);

    int post(QNetworkRequest request, QByteArray data = "");
    void post(QNetworkRequest request, QByteArray data, int requestId);

    void customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data, int requestId);

    int getUniqueRequestId() { return ++m_requestCount; }

protected:
    O2 *m_o2 = nullptr;
    O0SettingsStore *m_settingsStore = nullptr;
    RESTServiceLocalConfig m_config;

    void setConfig(const RESTServiceLocalConfig& config);
    void handleRateLimit(const RequestContainer& container);
    void startCooldown(int seconds);
    void dequeueRequests();

    bool m_isOnCooldown = false;
    int m_requestCount = 0;
    int m_currentRequestCount = 0;
    QMap<int, RequestContainer> m_requestMap;
    QQueue<RequestContainer> m_requestQueue;

private:
    int checkAndEnqueueRequest(const QNetworkRequest& request, int requestId, RequestType type, QByteArray data, QByteArray verb = "");

protected slots:
    void onLinkingSucceeded();
    static void onOpenBrowser(const QUrl& url);

    void onCooldownFinished();
    void onReplyReceived(int internalId, QNetworkReply::NetworkError error, const QByteArray& data, QList<QNetworkReply::RawHeaderPair> headers);
};

#endif // RESTSERVICECONNECTORLOCAL_H
