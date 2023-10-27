#include "spotifyimage.h"

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyImage::fromJson(const QJsonObject &json) -> SpotifyImage
{
    SpotifyImage image;
    image.url = json["url"_L1].toString();
    image.width = json["width"_L1].toInt();
    image.height = json["height"_L1].toInt();

    return image;
}

auto SpotifyImage::fromJson(const QJsonArray &array) -> std::vector<SpotifyImage>
{
    std::vector<SpotifyImage> images;
    images.reserve(array.count());

    for (const auto &element : array)
    {
        images.push_back(SpotifyImage::fromJson(element.toObject()));
    }

    return images;
}

auto SpotifyImage::size() const -> QSize
{
    return {width, height};
}
