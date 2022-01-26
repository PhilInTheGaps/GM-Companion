#pragma once

#include <QObject>
#include <QNetworkAccessManager>

#include "googledriveconnectorlocal.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class GoogleDrive : public Service
{
    Q_OBJECT
    AUTO_PROPERTY(QString, clientId)

public:
    static GoogleDrive *getInstance();

    void grant() { m_connector->grantAccess(); }
    bool isGranted() const { return m_connector->isAccessGranted(); }

    QFuture<RestNetworkReply*> get(const QUrl &url);
    QFuture<RestNetworkReply*> get(const QNetworkRequest &request);
    QFuture<RestNetworkReply*> put(const QUrl &url, const QByteArray &data = "");
    QFuture<RestNetworkReply*> put(const QNetworkRequest &request, const QByteArray &data);
    QFuture<RestNetworkReply*> post(const QUrl &url, const QByteArray &data);
    QFuture<RestNetworkReply*> post(const QNetworkRequest &request, const QByteArray &data);
    QFuture<RestNetworkReply*> customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data);

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    GoogleDrive(QObject *parent = nullptr);

    inline static GoogleDrive *single = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;

    void updateConnector();

private slots:
    void onAccessGranted();

signals:
    void authorized();
    void clientIdChanged();
};
