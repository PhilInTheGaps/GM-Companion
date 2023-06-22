#include "webimageloader.h"
#include "../audiothumbnailcache.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

Q_LOGGING_CATEGORY(gmAudioWebImageLoader, "gm.audio.thumbnails.loaders.web")

using namespace AsyncFuture;

auto WebImageLoader::loadImageAsync(const QString &url, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    qCDebug(gmAudioWebImageLoader()) << "Loading image from web:" << url << "...";

    // Try to load from cache
    QPixmap pixmap;
    if (AudioThumbnailCache::tryGet(url, &pixmap)) return completed(pixmap);

    // Load from url
    const auto request = QNetworkRequest(QUrl(url));
    auto *reply = networkManager->get(request);

    return loadImageAsync(reply, url);
}

auto WebImageLoader::loadImageAsync(QNetworkReply *reply, const QString &url) -> QFuture<QPixmap>
{
    auto future = observe(reply, &QNetworkReply::finished).future();

    const auto callback = [url, reply]() {
        QPixmap image;

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmAudioWebImageLoader())
                << reply->error() << reply->errorString() << "Could not load image from" << url;
            reply->deleteLater();
            return image;
        }

        if (image.loadFromData(reply->readAll()))
        {
            AudioThumbnailCache::instance()->insertImage(url, image);
        }
        else
        {
            qCWarning(gmAudioWebImageLoader()) << "Failed to load image data for" << url;
        }

        reply->deleteLater();
        return image;
    };

    return observe(future).subscribe(callback).future();
}
