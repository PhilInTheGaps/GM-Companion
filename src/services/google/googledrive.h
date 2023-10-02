#pragma once

#include "qmlsingletonfactory.h"
#include "rest/restserviceconnector.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class GoogleDrive : public Service
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_ONLY_SINGLETON_FACTORY(GoogleDrive)

    AUTO_PROPERTY(QString, clientId)

public:
    explicit GoogleDrive(QQmlEngine &engine, QObject *parent);
    explicit GoogleDrive(QNetworkAccessManager &networkManager, QObject *parent = nullptr);
    explicit GoogleDrive(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent = nullptr);

    static auto qmlInstance(QQmlEngine *engine) -> GoogleDrive *;

    void grant()
    {
        m_connector->grantAccess();
    }

    [[nodiscard]] auto isGranted() const -> bool
    {
        return m_connector->isAccessGranted();
    }

    auto get(const QUrl &url) -> QFuture<RestNetworkReply *>;
    auto get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>;
    auto put(const QUrl &url, const QByteArray &data = "") -> QFuture<RestNetworkReply *>;
    auto put(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestNetworkReply *>;
    auto post(const QUrl &url, const QByteArray &data) -> QFuture<RestNetworkReply *>;
    auto post(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestNetworkReply *>;
    auto customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
        -> QFuture<RestNetworkReply *>;

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    inline static GoogleDrive *s_qmlInstance = nullptr;

    QNetworkAccessManager &m_networkManager;
    RESTServiceConnector *m_connector = nullptr;

    void updateConnector();

private slots:
    void onAccessGranted();

signals:
    void authorized();
};
