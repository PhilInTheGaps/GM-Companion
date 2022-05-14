#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFuture>
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class NextCloud : public Service
{
    Q_OBJECT
public:
    static NextCloud *getInstance();

    QFuture<QNetworkReply*> sendDavRequest(const QByteArray& method, const QString &path, const QByteArray& data,
                                  const QList<QPair<QByteArray, QByteArray> >& headers = {});

    QString getPathUrl(const QString &path);

    AUTO_PROPERTY(QString, loginName)
    AUTO_PROPERTY(QString, serverUrl)

public slots:
    void connectService() override;
    void disconnectService() override;

signals:
    void loggedIn();

private:
    explicit NextCloud(QObject *parent = nullptr);

    void startLoginFlow();
    void pollAuthPoint(const QUrl& url, const QString& token);

    inline static NextCloud *single = nullptr;

    static constexpr const char* AUTH_URL = "/index.php/login/v2";
    static constexpr const char* DAV_ENDPOINT = "/remote.php/dav/files";
    static constexpr int AUTH_POLL_DELAY = 3000;
    static constexpr int MAX_AUTH_POLLS = 20;

    QNetworkAccessManager *m_networkManager = nullptr;
    int m_authPolls = 0;
    bool m_loggingIn = false;

    QString m_appPassword = "";
};
