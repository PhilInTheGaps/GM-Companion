#include "audiothumbnailprovider.h"
#include "audiothumbnailcache.h"
#include "audiothumbnailgenerator.h"

#include <QRegularExpression>

AudioThumbnailProvider::AudioThumbnailProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

auto AudioThumbnailProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) -> QPixmap
{
    auto _id = id;
    auto type = QStringLiteral("Music");

    if (const auto match = regex.match(id); match.hasMatch())
    {
        _id = match.captured(1);
        type = match.captured(2);
    }

    QPixmap pixmap;
    AudioThumbnailCache::tryGet(_id, &pixmap);

    if (pixmap.isNull())
    {
        pixmap = AudioThumbnailGenerator::getPlaceholderImage(type);
    }

    if (size) *size = pixmap.size();

    if ((requestedSize.width() > 0) && (requestedSize.height() > 0))
    {
        pixmap = pixmap.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }

    return pixmap;
}
