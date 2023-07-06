﻿#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <gsl/gsl>

#include "clients/librespotcontroller.h"
#include "service.h"
#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include "spotifynetworkerror.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

#include "api/albumapi.h"
#include "api/playerapi.h"
#include "api/playlistsapi.h"
#include "api/tracksapi.h"

class Spotify : public Service
{
    Q_OBJECT
    Q_PROPERTY(ServiceStatus *clientStatus READ clientStatus NOTIFY clientStatusChanged)

public:
    static auto instance() -> Spotify *;

    void grant();
    [[nodiscard]] auto isGranted() const -> bool;

    auto get(const QNetworkRequest &request) -> QFuture<gsl::owner<RestNetworkReply *>>;
    auto get(const QUrl &url) -> QFuture<gsl::owner<RestNetworkReply *>>;
    auto put(const QNetworkRequest &request, const QByteArray &data = "") -> QFuture<gsl::owner<RestNetworkReply *>>;
    auto put(const QUrl &url, const QByteArray &data = "") -> QFuture<gsl::owner<RestNetworkReply *>>;
    auto post(const QNetworkRequest &request, const QByteArray &data = "") -> QFuture<gsl::owner<RestNetworkReply *>>;

    [[nodiscard]] auto clientStatus() const -> ServiceStatus *;

    const AlbumAPI *albums;
    const PlayerAPI *player;
    const PlaylistsAPI *playlists;
    const TracksAPI *tracks;

    AUTO_PROPERTY(QString, username);

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    explicit Spotify(QObject *parent);
    static inline Spotify *m_instance = nullptr;

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
