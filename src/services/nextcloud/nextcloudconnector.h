#pragma once

#include "rest/restserviceconnector.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>
#include <QPointer>

namespace Services
{

class NextCloud;

class NextCloudConnector : public RESTServiceConnector
{
    Q_OBJECT

public:
    enum class State
    {
        Connected,
        Connecting,
        Disconnected
    };

    explicit NextCloudConnector(NextCloud *nc, QObject *parent = nullptr);

    void grantAccess() override;
    void disconnectService() override;
    [[nodiscard]] auto isAccessGranted() const -> bool override;

    auto get(const QNetworkRequest &request, Options options) -> QFuture<RestReply> override;
    auto put(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply> override;
    auto post(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply> override;
    auto customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data,
                       Options options) -> QFuture<RestReply> override;

    AUTO_PROPERTY_VAL2(NextCloudConnector::State, state, State::Disconnected)
protected:
    void sendRequest(RestRequest &&request, QPromise<RestReply> &&reply) override;
    void refreshAccessToken(bool updateAuthentication = false) override;
    [[nodiscard]] auto getAccessToken() -> QString override;

private slots:
    void onStateChanged(State state);

private:
    void startLoginFlow();
    void pollAuthPoint(const QUrl &url, const QString &token);
    void handleAuthPointReply(QNetworkReply *reply, const QUrl &url, const QString &token);
    void handleAuthPointNotFound(const QUrl &url, const QString &token);
    void handleAuthPointSuccess(QNetworkReply &reply);

    int m_authPolls = 0;
    QString m_appPassword = QLatin1String("");

    QPointer<NextCloud> m_nc = nullptr;
};

} // namespace Services
