#pragma once

#include "services/youtube/playlistid.h"
#include "services/youtube/videoid.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QString>

namespace Services
{
struct YouTubeVideo;
struct YouTubePlaylist;
} // namespace Services

class YouTubeImageLoader
{
public:
    static auto loadImageAsync(const Services::VideoId &id, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>;
    static auto loadImageAsync(const Services::YouTubeVideo &video, QNetworkAccessManager *networkManager)
        -> QFuture<QPixmap>;

    static auto loadImageAsync(const Services::PlaylistId &id, QNetworkAccessManager *networkManager)
        -> QFuture<QPixmap>;
    static auto loadImageAsync(const Services::YouTubePlaylist &playlist, QNetworkAccessManager *networkManager)
        -> QFuture<QPixmap>;

private:
    static auto loadImageAsync(QNetworkReply *reply, const QString &id) -> QFuture<QPixmap>;
};
