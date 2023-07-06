#pragma once

#include "googledriveconnectorlocal.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>
#include <QObject>

class GoogleDrive : public Service
{
    Q_OBJECT
    AUTO_PROPERTY(QString, clientId)

public:
    GoogleDrive(QNetworkAccessManager &networkManager, QObject *parent = nullptr);
    GoogleDrive(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent = nullptr);

    void grant()
    {
        m_connector->grantAccess();
    }

    auto isGranted() const -> bool
    {
        return m_connector->isAccessGranted();
    }

    QFuture<RestNetworkReply *> get(const QUrl &url);
    QFuture<RestNetworkReply *> get(const QNetworkRequest &request);
    QFuture<RestNetworkReply *> put(const QUrl &url, const QByteArray &data = "");
    QFuture<RestNetworkReply *> put(const QNetworkRequest &request, const QByteArray &data);
    QFuture<RestNetworkReply *> post(const QUrl &url, const QByteArray &data);
    QFuture<RestNetworkReply *> post(const QNetworkRequest &request, const QByteArray &data);
    QFuture<RestNetworkReply *> customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                              const QByteArray &data);

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    QNetworkAccessManager &m_networkManager;
    RESTServiceConnector *m_connector = nullptr;

    void updateConnector();

private slots:
    void onAccessGranted();

signals:
    void authorized();
};
