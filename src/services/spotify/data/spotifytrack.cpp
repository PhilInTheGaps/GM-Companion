#include "spotifytrack.h"

#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyTrack, "gm.services.spotify.data.track")

auto SpotifyTrack::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrack>
{
    // Tracks in playlists might be located inside a container with playlist-specific meta infos
    if (json.contains(QStringLiteral("added_at")))
    {
        return SpotifyTrack::fromJson(json[QStringLiteral("track")].toObject());
    }

    auto *track = new SpotifyTrack {
        {
            json[QStringLiteral("href")].toString(),
            json[QStringLiteral("uri")].toString(),
            json[QStringLiteral("id")].toString(),
            json[QStringLiteral("name")].toString()
        },
        json[QStringLiteral("duration_ms")].toInt(),
        getIsPlayable(json),
        SpotifyAlbumInfo::fromJson(json[QStringLiteral("album")].toObject()),
        SpotifyArtist::fromJson(json[QStringLiteral("artists")].toArray()),
    };

    return QSharedPointer<SpotifyTrack>(track);
}

auto SpotifyTrack::fromJson(const QJsonArray &json) -> QVector<QSharedPointer<SpotifyTrack>>
{
    QVector<QSharedPointer<SpotifyTrack>> tracks;
    tracks.reserve(json.count());

    for (const auto &item : json)
    {
        tracks << SpotifyTrack::fromJson(item.toObject());
    }

    return tracks;
}

auto SpotifyTrack::artistString() const -> QString
{
    QStringList names;
    names.reserve(artists.count());

    for (const auto &artist : artists)
    {
        names << artist->name;
    }

    return names.join(QStringLiteral(", "));
}

auto SpotifyTrack::image() const -> QSharedPointer<SpotifyImage>
{
    if (album->images.isEmpty()) {}

    return album->images.first();
}

auto SpotifyTrack::getIsPlayable(const QJsonObject &json) ->bool
{
    if (json.contains(QStringLiteral("is_playable")))
    {
        return json[QStringLiteral("is_playable")].toBool();
    }

    if (json.contains(QStringLiteral("available_markets")))
    {
        return !json[QStringLiteral("available_markets")].toArray().isEmpty();
    }

    return true;
}
