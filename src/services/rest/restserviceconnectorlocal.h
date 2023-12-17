#pragma once

#include "o0settingsstore.h"
#include "o2.h"
#include "o2requestor.h"
#include "restrequest.h"
#include "restserviceconnector.h"
#include "settings/abstractsettingsmanager.h"
#include <QHash>
#include <QLoggingCategory>
#include <QPromise>
#include <QQueue>
#include <memory>

namespace Services
{

struct RESTServiceLocalConfig
{
    QString scope;
    int port;
    int maxConcurrentRequests;
    QString authHeaderFormat;
    Common::Settings::Request<QString> idRequest;
    Common::Settings::Request<QString> secretRequest;
};

class RESTServiceConnectorLocal : public RESTServiceConnector
{
    Q_OBJECT
public:
    RESTServiceConnectorLocal(QNetworkAccessManager *networkManager, O2 *o2, const QLoggingCategory &loggingCategory,
                              QObject *parent);

    void grantAccess() override;
    void disconnectService() override;
    [[nodiscard]] auto isAccessGranted() const -> bool override;

    auto get(const QNetworkRequest &request, Options options) -> QFuture<RestReply> override;
    auto put(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply> override;
    auto post(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply> override;
    auto customRequest(const QNetworkRequest &req, const QByteArray &verb, const QByteArray &data, Options options)
        -> QFuture<RestReply> override;

protected:
    O2 *m_o2 = nullptr;
    O0SettingsStore *m_settingsStore = nullptr;
    RESTServiceLocalConfig m_config;

    void setConfig(RESTServiceLocalConfig config);

protected slots:
    void onLinkingSucceeded();
    static void onOpenBrowser(const QUrl &url);

    void onRefreshFinished(QNetworkReply::NetworkError error);

private:
    auto makeRequestor() -> O2Requestor *;
    void sendRequest(RestRequest &&container, QPromise<RestReply> &&promise) override;

    [[nodiscard]] auto getAccessToken() -> QString override;
    void refreshAccessToken(bool /*updateAuthentication*/ = false) override;
};

} // namespace Services
