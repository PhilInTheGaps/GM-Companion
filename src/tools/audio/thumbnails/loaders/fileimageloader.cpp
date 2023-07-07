#include "fileimageloader.h"
#include "../audiothumbnailcache.h"
#include "file.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

using namespace Files;

auto FileImageLoader::loadImageAsync(const QString &path) -> QFuture<QPixmap>
{
    // Try to load from cache
    QPixmap pixmap;
    if (AudioThumbnailCache::tryGet(path, &pixmap)) return QtFuture::makeReadyFuture(pixmap);

    auto future = File::getDataAsync(path);

    const auto callback = [path](FileDataResult *result) {
        return QtConcurrent::run(loadFromFileResult, path, result);
    };

    return future.then(callback).unwrap();
}

auto FileImageLoader::loadFromFileResult(const QString &path, FileDataResult *result) -> QPixmap
{
    QPixmap image;
    image.loadFromData(result->data());

    AudioThumbnailCache::instance()->insertImage(path, image);

    result->deleteLater();
    return image;
}
