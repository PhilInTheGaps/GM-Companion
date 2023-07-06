#include "spotifyimage.h"

using namespace Qt::Literals::StringLiterals;

auto SpotifyImage::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyImage>
{
    auto *image = new SpotifyImage();

    image->url = json["url"_L1].toString();
    image->width = json["width"_L1].toInt();
    image->height = json["height"_L1].toInt();

    return QSharedPointer<SpotifyImage>(image);
}

auto SpotifyImage::fromJson(const QJsonArray &array) -> QList<QSharedPointer<SpotifyImage>>
{
    QList<QSharedPointer<SpotifyImage>> images;
    images.reserve(array.count());

    for (const auto &element : array)
    {
        images << SpotifyImage::fromJson(element.toObject());
    }

    return images;
}

auto SpotifyImage::size() const -> QSize
{
    return {width, height};
}
