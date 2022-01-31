#include "fileimageloader.h"
#include "../audiothumbnailcache.h"
#include "file.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>

using namespace AsyncFuture;
using namespace Files;

auto FileImageLoader::loadImageAsync(const QString &path) -> QFuture<QPixmap>
{
    // Try to load from cache
    QPixmap pixmap;
    if (AudioThumbnailCache::tryGet(path, &pixmap)) return completed(pixmap);

    const auto future = observe(File::getDataAsync(path)).future();

    const auto callback = [path](FileDataResult *result) {
        return QtConcurrent::run(loadFromFileResult, path, result);
    };

    return observe(future).subscribe(callback).future();
}

auto FileImageLoader::loadFromFileResult(const QString &path, FileDataResult *result) -> QPixmap
{
    QPixmap image;
    image.loadFromData(result->data());

    AudioThumbnailCache::instance()->insertImage(path, image);

    result->deleteLater();
    return image;
}
