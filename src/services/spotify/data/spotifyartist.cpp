#include "spotifyartist.h"

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyArtist::fromJson(const QJsonObject &json) -> SpotifyArtist
{
    return {
        {json["href"_L1].toString(), json["uri"_L1].toString(), json["id"_L1].toString(), json["name"_L1].toString()},
        SpotifyImage::fromJson(json["images"_L1].toArray())};
}

auto SpotifyArtist::fromJson(const QJsonArray &json) -> std::vector<SpotifyArtist>
{
    std::vector<SpotifyArtist> artists;
    artists.reserve(json.count());

    for (const auto &item : json)
    {
        artists.push_back(SpotifyArtist::fromJson(item.toObject()));
    }

    return artists;
}
