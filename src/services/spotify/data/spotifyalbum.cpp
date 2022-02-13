#include "spotifyalbum.h"
#include "spotifyimage.h"
#include "spotifyartist.h"
#include "spotifytrack.h"

#include <QJsonDocument>

auto SpotifyAlbum::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbum>
{
    auto *album = new SpotifyAlbum
    {
        {
            json[QStringLiteral("href")].toString(),
            json[QStringLiteral("uri")].toString(),
            json[QStringLiteral("id")].toString(),
            json[QStringLiteral("name")].toString()
        },
        json[QStringLiteral("release_date")].toString(),
        SpotifyImage::fromJson(json[QStringLiteral("images")].toArray()),
        SpotifyArtist::fromJson(json[QStringLiteral("artists")].toArray()),
        SpotifyTrackList::fromJson(json[QStringLiteral("tracks")].toObject())
    };

    return QSharedPointer<SpotifyAlbum>(album);
}

auto SpotifyAlbum::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbum>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyAlbum::fromJson(json);
}
