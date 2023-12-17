#pragma once

#include "../options.h"
#include "nextcloudconnector.h"
#include "qmlsingletonfactory.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

namespace Services
{
class NextCloud : public Services::Service
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_ONLY_SINGLETON_FACTORY(Services::NextCloud)

public:
    explicit NextCloud(const QQmlEngine &engine, QObject *parent);
    explicit NextCloud(QNetworkAccessManager *networkManager, QObject *parent);
    explicit NextCloud(const QString &serviceName, QNetworkAccessManager *networkManager, QObject *parent);

    static auto qmlInstance(QQmlEngine *engine) -> NextCloud *;

    auto sendDavRequest(const QByteArray &method, const QString &path, const QByteArray &data,
                        const QList<std::pair<QByteArray, QByteArray>> &headers, Options options) -> QFuture<RestReply>;

    [[nodiscard]] auto getPathUrl(const QString &path) const -> QString;

    AUTO_PROPERTY(QString, loginName)
    AUTO_PROPERTY(QString, serverUrl)

public slots:
    void connectService() override;
    void disconnectService() override;

signals:
    void loggedIn();

private slots:
    void onConnectorStateChanged(NextCloudConnector::State state);

private:
    inline static NextCloud *s_qmlInstance = nullptr;

    NextCloudConnector m_connector;
};
} // namespace Services
