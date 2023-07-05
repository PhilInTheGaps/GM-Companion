#include "spotifytrack.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyTrack, "gm.services.spotify.data.track")

auto SpotifyTrack::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrack>
{
    // Tracks in playlists might be located inside a container with playlist-specific meta infos
    if (json.contains(QStringLiteral("added_at")))
    {
        return SpotifyTrack::fromJson(json[QStringLiteral("track")].toObject());
    }

    const auto isLinked = json.contains(QStringLiteral("linked_from"));

    auto *track = new SpotifyTrack{
        {isLinked ? json[QStringLiteral("linked_from")][QStringLiteral("href")].toString()
                  : json[QStringLiteral("href")].toString(),
         isLinked ? json[QStringLiteral("linked_from")][QStringLiteral("uri")].toString()
                  : json[QStringLiteral("uri")].toString(),
         isLinked ? json[QStringLiteral("linked_from")][QStringLiteral("id")].toString()
                  : json[QStringLiteral("id")].toString(),
         json[QStringLiteral("name")].toString()},
        json[QStringLiteral("duration_ms")].toInt(),
        getIsPlayable(json),
        SpotifyAlbumInfo::fromJson(json[QStringLiteral("album")].toObject()),
        SpotifyArtist::fromJson(json[QStringLiteral("artists")].toArray()),
    };

    return QSharedPointer<SpotifyTrack>(track);
}

auto SpotifyTrack::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyTrack>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyTrack::fromJson(json);
}

auto SpotifyTrack::fromJsonArray(const QJsonArray &json) -> std::vector<QSharedPointer<SpotifyTrack>>
{
    std::vector<QSharedPointer<SpotifyTrack>> tracks;
    tracks.reserve(json.count());

    foreach (const auto &item, json)
    {
        tracks.push_back(SpotifyTrack::fromJson(item.toObject()));
    }

    return tracks;
}

auto SpotifyTrack::fromJsonArray(const QByteArray &data) -> std::vector<QSharedPointer<SpotifyTrack>>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyTrack::fromJsonArray(json[QStringLiteral("tracks")].toArray());
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
    if (album->images.isEmpty())
    {
        // FIXME
    }

    return album->images.constFirst();
}

auto SpotifyTrack::getIsPlayable(const QJsonObject &json) -> bool
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
