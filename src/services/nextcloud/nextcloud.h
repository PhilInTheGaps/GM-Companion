#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class NextCloud : public Service
{
    Q_OBJECT
public:
    static NextCloud *getInstance();

    QNetworkReply *sendDavRequest(const QByteArray& method, const QString &path, const QByteArray& data,
                                  const QList<QPair<QByteArray, QByteArray> >& headers = {});

    QString getPathUrl(const QString &path);

    AUTO_PROPERTY(QString, loginName)
    AUTO_PROPERTY(QString, serverUrl)

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    explicit NextCloud(QObject *parent = nullptr);

    inline static NextCloud *single = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
    int m_authPolls = 0;
    bool m_loggingIn = false;

    QString m_appPassword = "";

    void startLoginFlow();
    void pollAuthPoint(const QUrl& url, const QString& token);

};
