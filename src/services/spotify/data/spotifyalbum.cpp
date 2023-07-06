#include "spotifyalbum.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include "spotifytrack.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;

auto SpotifyAlbum::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbum>
{
    auto *album = new SpotifyAlbum{
        {json["href"_L1].toString(), json["uri"_L1].toString(), json["id"_L1].toString(), json["name"_L1].toString()},
        json["release_date"_L1].toString(),
        SpotifyImage::fromJson(json["images"_L1].toArray()),
        SpotifyArtist::fromJson(json["artists"_L1].toArray()),
        SpotifyTrackList::fromJson(json["tracks"_L1].toObject())};

    return QSharedPointer<SpotifyAlbum>(album);
}

auto SpotifyAlbum::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbum>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyAlbum::fromJson(json);
}
