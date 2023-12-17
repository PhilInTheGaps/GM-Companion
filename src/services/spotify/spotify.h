#pragma once

#include "../options.h"
#include "api/albumapi.h"
#include "api/playerapi.h"
#include "api/playlistsapi.h"
#include "api/tracksapi.h"
#include "clients/librespotcontroller.h"
#include "qmlsingletonfactory.h"
#include "rest/restserviceconnector.h"
#include "service.h"
#include "spotifynetworkerror.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <QtQml/qqmlregistration.h>

namespace Services
{

class Spotify : public Services::Service
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_SINGLETON_FACTORY(Services::Spotify)

    Q_PROPERTY(Services::Status *clientStatus READ clientStatus NOTIFY clientStatusChanged)

public:
    Spotify() = delete;
    static auto instance() -> Spotify *;

    void grant();
    [[nodiscard]] auto isGranted() const -> bool;

    auto get(const QNetworkRequest &request, Options options) -> QFuture<RestReply>;
    auto get(const QUrl &url, Options options) -> QFuture<RestReply>;
    auto put(const QNetworkRequest &request, const QByteArray &data, Options options) -> QFuture<RestReply>;
    auto put(const QUrl &url, const QByteArray &data, Options options) -> QFuture<RestReply>;
    auto post(const QNetworkRequest &request, const QByteArray &data, Options options) -> QFuture<RestReply>;

    [[nodiscard]] auto clientStatus() const -> Status *;

    AlbumAPI albums = AlbumAPI(this);
    PlayerAPI player = PlayerAPI(this);
    PlaylistsAPI playlists = PlaylistsAPI(this);
    TracksAPI tracks = TracksAPI(this);

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

    void handleNetworkError(const RestReply &reply);
    void handleAccessDenied(const SpotifyNetworkError &error);

    void updateConnector();

signals:
    void authorized();
    void clientStatusChanged();

private slots:
    void onAccessGranted();
    void forwardClientStatus(const QString &message);
};

} // namespace Services
