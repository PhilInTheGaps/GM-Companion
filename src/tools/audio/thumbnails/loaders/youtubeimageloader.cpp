#include "youtubeimageloader.h"
#include "../audiothumbnailcache.h"
#include "services/youtube/youtube.h"
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

Q_LOGGING_CATEGORY(gmAudioYouTubeImageLoader, "gm.audio.thumbnails.loaders.youtube")

using namespace Services;

auto YouTubeImageLoader::loadImageAsync(const VideoId &id, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    if (!id.isValid()) return {};

    // Try to load from cache
    if (QPixmap pixmap; AudioThumbnailCache::tryGet(id.toString(), &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    // Get video info
    return YouTube::instance()
        ->getStreamInfoAsync(id, Services::Option::LowPriority)
        .then([networkManager](const YouTubeVideo &video) { return loadImageAsync(video, networkManager); })
        .unwrap();
}

auto YouTubeImageLoader::loadImageAsync(const YouTubeVideo &video, QNetworkAccessManager *networkManager)
    -> QFuture<QPixmap>
{
    if (!video.id.isValid()) return {};

    qCDebug(gmAudioYouTubeImageLoader()) << "Loading image from youtube video:" << video.id.toString() << "...";

    // Try to load from cache
    if (QPixmap pixmap; AudioThumbnailCache::tryGet(video.id.toString(), &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    if (video.thumbnailUrl.isEmpty()) return {};

    // Load from url
    const auto request = QNetworkRequest(QUrl(video.thumbnailUrl));
    auto *reply = networkManager->get(request);

    return loadImageAsync(reply, video.id.toString());
}

auto YouTubeImageLoader::loadImageAsync(const PlaylistId &id, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    if (!id.isValid()) return {};

    // Try to load from cache
    if (QPixmap pixmap; AudioThumbnailCache::tryGet(id.toString(), &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    // Get video info
    return YouTube::instance()
        ->getPlaylistInfoAsync(id, Services::Option::LowPriority)
        .then([networkManager](const YouTubePlaylist &playlist) { return loadImageAsync(playlist, networkManager); })
        .unwrap();

    return {};
}

auto YouTubeImageLoader::loadImageAsync(const YouTubePlaylist &playlist, QNetworkAccessManager *networkManager)
    -> QFuture<QPixmap>
{
    if (!playlist.id.isValid()) return {};

    qCDebug(gmAudioYouTubeImageLoader()) << "Loading image from youtube playlist:" << playlist.id.toString() << "...";

    // Try to load from cache
    if (QPixmap pixmap; AudioThumbnailCache::tryGet(playlist.id.toString(), &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    if (playlist.thumbnailUrl.isEmpty()) return {};

    // Load from url
    const auto request = QNetworkRequest(QUrl(playlist.thumbnailUrl));
    auto *reply = networkManager->get(request);

    return loadImageAsync(reply, playlist.id.toString());
}

auto YouTubeImageLoader::loadImageAsync(QNetworkReply *reply, const QString &id) -> QFuture<QPixmap>
{
    auto future = QtFuture::connect(reply, &QNetworkReply::finished);

    const auto callback = [id, reply]() {
        QPixmap image;

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmAudioYouTubeImageLoader())
                << reply->error() << reply->errorString() << "Could not load image from" << id;
            reply->deleteLater();
            return image;
        }

        if (image.loadFromData(reply->readAll()))
        {
            AudioThumbnailCache::instance()->insertImage(id, image);
        }
        else
        {
            qCWarning(gmAudioYouTubeImageLoader()) << "Failed to load image data for" << id;
        }

        reply->deleteLater();
        return image;
    };

    return future.then(callback);
}
