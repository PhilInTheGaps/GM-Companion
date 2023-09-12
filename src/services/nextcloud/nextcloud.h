#pragma once

#include "qmlsingletonfactory.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <gsl/gsl>

class NextCloud : public Service
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_ONLY_SINGLETON_FACTORY(NextCloud)

public:
    explicit NextCloud(QQmlEngine &engine, QObject *parent);
    explicit NextCloud(QNetworkAccessManager &networkManager, QObject *parent);
    explicit NextCloud(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent);

    static auto qmlInstance(QQmlEngine *engine) -> NextCloud *;

    auto sendDavRequest(const QByteArray &method, const QString &path, const QByteArray &data,
                        const QList<std::pair<QByteArray, QByteArray>> &headers = {}) -> QFuture<QNetworkReply *>;

    [[nodiscard]] QString getPathUrl(const QString &path) const;

    AUTO_PROPERTY(QString, loginName)
    AUTO_PROPERTY(QString, serverUrl)

public slots:
    void connectService() override;
    void disconnectService() override;

signals:
    void loggedIn();

private:
    inline static NextCloud *s_qmlInstance = nullptr;

    QNetworkAccessManager &m_networkManager;
    int m_authPolls = 0;
    bool m_loggingIn = false;

    QString m_appPassword = QLatin1String("");

    void startLoginFlow();
    void pollAuthPoint(const QUrl &url, const QString &token);
    void handleAuthPointReply(QNetworkReply *reply, const QUrl &url, const QString &token);
    void handleAuthPointNotFound(const QUrl &url, const QString &token);
    void handleAuthPointSuccess(QNetworkReply &reply);
};
