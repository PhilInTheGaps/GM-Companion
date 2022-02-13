#include "spotifyalbuminfo.h"
#include "spotifyimage.h"
#include "spotifyartist.h"

#include <QJsonDocument>

auto SpotifyAlbumInfo::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbumInfo>
{
    auto *info = new SpotifyAlbumInfo {
        {
            json[QStringLiteral("href")].toString(),
            json[QStringLiteral("uri")].toString(),
            json[QStringLiteral("id")].toString(),
            json[QStringLiteral("name")].toString()
        },
        json[QStringLiteral("release_date")].toString(),
        SpotifyImage::fromJson(json[QStringLiteral("images")].toArray()),
        SpotifyArtist::fromJson(json[QStringLiteral("artists")].toArray())
    };

    return QSharedPointer<SpotifyAlbumInfo>(info);
}

auto SpotifyAlbumInfo::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbumInfo>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyAlbumInfo::fromJson(json);
}
