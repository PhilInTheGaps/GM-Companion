#include "spotifyartist.h"


auto SpotifyArtist::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyArtist>
{
    auto *artist = new SpotifyArtist {
        {
            json[QStringLiteral("href")].toString(),
            json[QStringLiteral("uri")].toString(),
            json[QStringLiteral("id")].toString(),
            json[QStringLiteral("name")].toString()
        },
        SpotifyImage::fromJson(json[QStringLiteral("images")].toArray())
    };

    return QSharedPointer<SpotifyArtist>(artist);
}

auto SpotifyArtist::fromJson(const QJsonArray &json) -> QVector<QSharedPointer<SpotifyArtist>>
{
    QVector<QSharedPointer<SpotifyArtist>> artists;
    artists.reserve(json.count());

    for (const auto &item : json)
    {
        artists << SpotifyArtist::fromJson(item.toObject());
    }

    return artists;
}
