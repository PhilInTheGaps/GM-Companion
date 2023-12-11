#pragma once

#include "rest/restserviceconnector.h"
#include <QNetworkAccessManager>
#include <QString>
#include <vector>

namespace Services
{

struct PipedInstance
{
    QString name;
    QString apiUrl;
    QString locations;
    bool cdn;
};

class PipedConnector : public RESTServiceConnector
{
    Q_OBJECT
public:
    explicit PipedConnector(QObject *parent = nullptr);

    [[nodiscard]] auto instances() const -> std::vector<PipedInstance>;
    [[nodiscard]] auto currentInstance() const -> PipedInstance;
    auto fetchInstancesAsync() -> QFuture<bool>;
    void selectBestInstance();

    void grantAccess() override;
    void disconnectService() override;
    [[nodiscard]] auto isAccessGranted() const -> bool override;

    auto get(const QNetworkRequest &request, bool isAuthRequired) -> QFuture<RestReply> override;
    auto put(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply> override;
    auto post(QNetworkRequest request, const QByteArray &data) -> QFuture<RestReply> override;
    auto customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
        -> QFuture<RestReply> override;

protected:
    void sendRequest(RestRequest &&request, QPromise<RestReply> &&reply) override;
    void refreshAccessToken(bool updateAuthentication = false) override;
    [[nodiscard]] auto getAccessToken() -> QString override;

private:
    auto fetchInstancesFromApiAsync() -> QFuture<bool>;
    auto fetchInstancesFromWikiAsync() -> QFuture<bool>;

    std::vector<PipedInstance> m_instances;
    PipedInstance m_currentInstance;
};

} // namespace Services
