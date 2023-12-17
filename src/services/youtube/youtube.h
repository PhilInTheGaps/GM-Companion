#pragma once

#include "pipedconnector.h"
#include "playlistid.h"
#include "qmlsingletonfactory.h"
#include "service.h"
#include "videoid.h"
#include <QNetworkAccessManager>
#include <QString>
#include <vector>

namespace Services
{

struct YouTubeVideo
{
    VideoId id;
    QString title;
    QString uploader;
    QString thumbnailUrl;
    QString audioStreamUrl;
};

struct YouTubePlaylist
{
    PlaylistId id;
    QString title;
    QString uploader;
    QString thumbnailUrl;
    std::vector<YouTubeVideo> streams;
};

class YouTube : public Services::Service
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_SINGLETON_FACTORY(Services::YouTube)

public:
    YouTube() = delete;
    static auto instance() -> YouTube *;

    void setNetworkManager(QNetworkAccessManager *networkManager);
    void selectNewPipedInstance();

    [[nodiscard]] auto getStreamInfoAsync(const VideoId &id, Options options) -> QFuture<YouTubeVideo>;
    [[nodiscard]] auto getPlaylistInfoAsync(const PlaylistId &id, Options options) -> QFuture<YouTubePlaylist>;

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    explicit YouTube(QObject *parent = nullptr);

    [[nodiscard]] static auto parseStreamResponse(const QByteArray &data, const VideoId &id, bool &ok) -> YouTubeVideo;
    [[nodiscard]] auto parsePlaylistResponse(const QByteArray &data, const PlaylistId &id, Options options, bool &ok)
        -> QFuture<YouTubePlaylist>;

    [[nodiscard]] auto continueLoadingOfPlaylist(YouTubePlaylist &&playlist, const QString &nextpage, Options options)
        -> QFuture<YouTubePlaylist>;

    static inline YouTube *m_instance = nullptr;

    PipedConnector m_connector;
};

} // namespace Services
