#include "spotifyalbuminfo.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;

auto SpotifyAlbumInfo::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbumInfo>
{
    auto *info = new SpotifyAlbumInfo{
        {json["href"_L1].toString(), json["uri"_L1].toString(), json["id"_L1].toString(), json["name"_L1].toString()},
        json["release_date"_L1].toString(),
        SpotifyImage::fromJson(json["images"_L1].toArray()),
        SpotifyArtist::fromJson(json["artists"_L1].toArray())};

    return QSharedPointer<SpotifyAlbumInfo>(info);
}

auto SpotifyAlbumInfo::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbumInfo>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyAlbumInfo::fromJson(json);
}
