#include "spotifyimage.h"

auto SpotifyImage::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyImage>
{
    auto *image = new SpotifyImage();

    image->url = json[QStringLiteral("url")].toString();
    image->width = json[QStringLiteral("width")].toInt();
    image->height = json[QStringLiteral("height")].toInt();

    return QSharedPointer<SpotifyImage>(image);
}

auto SpotifyImage::fromJson(const QJsonArray &array) -> QVector<QSharedPointer<SpotifyImage>>
{
    QVector<QSharedPointer<SpotifyImage>> images;
    images.reserve(array.count());

    for (const auto &element : array)
    {
        images << SpotifyImage::fromJson(element.toObject());
    }

    return images;
}

auto SpotifyImage::size() const -> QSize
{
    return { width, height };
}
