#include "webimageloader.h"
#include "../audiothumbnailcache.h"
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

Q_LOGGING_CATEGORY(gmAudioWebImageLoader, "gm.audio.thumbnails.loaders.web")

auto WebImageLoader::loadImageAsync(const QString &url, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    qCDebug(gmAudioWebImageLoader()) << "Loading image from web:" << url << "...";

    // Try to load from cache
    if (QPixmap pixmap; AudioThumbnailCache::tryGet(url, &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    // Load from url
    const auto request = QNetworkRequest(QUrl(url));
    auto *reply = networkManager->get(request);

    return loadImageAsync(reply, url);
}

auto WebImageLoader::loadImageAsync(QNetworkReply *reply, const QString &url) -> QFuture<QPixmap>
{
    auto future = QtFuture::connect(reply, &QNetworkReply::finished);

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

    return future.then(callback);
}
