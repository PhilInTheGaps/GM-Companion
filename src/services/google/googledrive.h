#pragma once

// GoogleDrive support has been disabled.
//
// The reason is that it seems like my account(s) can no longer complete the grant flow.
// They can complete the login process and accept the scopes
// but then granting access fails with a generic error message.
//
// It is possible that this will be fixed in the future but currently I can't maintain this feature.

#include "qmlsingletonfactory.h"
#include "rest/restserviceconnector.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

namespace Services
{

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

    auto get(const QUrl &url) -> QFuture<RestReply>;
    auto get(const QNetworkRequest &request) -> QFuture<RestReply>;
    auto put(const QUrl &url, const QByteArray &data = "") -> QFuture<RestReply>;
    auto put(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestReply>;
    auto post(const QUrl &url, const QByteArray &data) -> QFuture<RestReply>;
    auto post(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestReply>;
    auto customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
        -> QFuture<RestReply>;

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    inline static GoogleDrive *s_qmlInstance = nullptr;

    QNetworkAccessManager &m_networkManager;
    RESTServiceConnector *m_connector = nullptr;

    void updateConnector();
    void grant();
    [[nodiscard]] auto isGranted() const -> bool;

private slots:
    void onAccessGranted();

signals:
    void authorized();
};
} // namespace Services
