#pragma once

#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <gsl/gsl>

class NextCloud : public Service
{
    Q_OBJECT
public:
    explicit NextCloud(QNetworkAccessManager &networkManager, QObject *parent);
    explicit NextCloud(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent);

    auto sendDavRequest(const QByteArray &method, const QString &path, const QByteArray &data,
                        const QList<QPair<QByteArray, QByteArray>> &headers = {}) -> QFuture<QNetworkReply *>;

    [[nodiscard]] QString getPathUrl(const QString &path) const;

    AUTO_PROPERTY(QString, loginName)
    AUTO_PROPERTY(QString, serverUrl)

public slots:
    void connectService() override;
    void disconnectService() override;

signals:
    void loggedIn();

private:
    void startLoginFlow();
    void pollAuthPoint(const QUrl &url, const QString &token);
    void handleAuthPointReply(QNetworkReply *reply, const QUrl &url, const QString &token);
    void handleAuthPointNotFound(const QUrl &url, const QString &token);
    void handleAuthPointSuccess(QNetworkReply &reply);

    QNetworkAccessManager &m_networkManager;
    int m_authPolls = 0;
    bool m_loggingIn = false;

    QString m_appPassword = QLatin1String("");
};
