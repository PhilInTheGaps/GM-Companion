#include "spotifyalbum.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include "spotifytracklist.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyAlbum::fromJson(const QJsonObject &json) -> SpotifyAlbum
{
    return {
        {json["href"_L1].toString(), json["uri"_L1].toString(), json["id"_L1].toString(), json["name"_L1].toString()},
        json["release_date"_L1].toString(),
        SpotifyImage::fromJson(json["images"_L1].toArray()),
        SpotifyArtist::fromJson(json["artists"_L1].toArray()),
        SpotifyTrackList::fromJson(json["tracks"_L1].toObject())};
}

auto SpotifyAlbum::fromJson(const QByteArray &data) -> SpotifyAlbum
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyAlbum::fromJson(json);
}
