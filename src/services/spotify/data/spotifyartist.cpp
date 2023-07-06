#include "spotifyartist.h"

using namespace Qt::Literals::StringLiterals;

auto SpotifyArtist::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyArtist>
{
    auto *artist = new SpotifyArtist{
        {json["href"_L1].toString(), json["uri"_L1].toString(), json["id"_L1].toString(), json["name"_L1].toString()},
        SpotifyImage::fromJson(json["images"_L1].toArray())};

    return QSharedPointer<SpotifyArtist>(artist);
}

auto SpotifyArtist::fromJson(const QJsonArray &json) -> QList<QSharedPointer<SpotifyArtist>>
{
    QList<QSharedPointer<SpotifyArtist>> artists;
    artists.reserve(json.count());

    for (const auto &item : json)
    {
        artists << SpotifyArtist::fromJson(item.toObject());
    }

    return artists;
}
