#pragma once

#include <QObject>
#include <QNetworkAccessManager>

#include "service.h"
#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include "spotifynetworkerror.h"
#include "clients/librespotcontroller.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Spotify : public Service
{
    Q_OBJECT
    Q_PROPERTY(ServiceStatus* clientStatus READ clientStatus NOTIFY clientStatusChanged)

public:
    static auto getInstance() -> Spotify*;

    void grant();
    [[nodiscard]] auto isGranted() const -> bool;

    auto get(const QNetworkRequest &request) -> QFuture<RestNetworkReply*>;
    auto get(const QUrl &url) -> QFuture<RestNetworkReply*>;
    auto put(const QUrl &url, const QByteArray &data = "") -> QFuture<RestNetworkReply*>;
    auto post(const QNetworkRequest& request, const QByteArray &data = "") -> QFuture<RestNetworkReply*>;

    [[nodiscard]] auto clientStatus() const -> ServiceStatus*;

    AUTO_PROPERTY(QString, username);

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    explicit Spotify(QObject* parent = nullptr);
    inline static Spotify *single = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;
    LibrespotController m_librespotController;

    void handleNetworkError(const RestNetworkReply *reply);
    void handleAccessDenied(const SpotifyNetworkError &error);

    void updateConnector();
    auto startClient() -> QFuture<bool>;

signals:
    void authorized();
    void usernameChanged();
    void clientStatusChanged();

private slots:
    void onAccessGranted();
    void forwardClientStatus(const QString &message);
};
