#include "spotifytrack.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmSpotifyTrack, "gm.services.spotify.data.track")

auto SpotifyTrack::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrack>
{
    // Tracks in playlists might be located inside a container with playlist-specific meta infos
    if (json.contains("added_at"_L1))
    {
        return SpotifyTrack::fromJson(json["track"_L1].toObject());
    }

    const auto isLinked = json.contains("linked_from"_L1);

    auto *track = new SpotifyTrack{
        {isLinked ? json["linked_from"_L1]["href"_L1].toString() : json["href"_L1].toString(),
         isLinked ? json["linked_from"_L1]["uri"_L1].toString() : json["uri"_L1].toString(),
         isLinked ? json["linked_from"_L1]["id"_L1].toString() : json["id"_L1].toString(), json["name"_L1].toString()},
        json["duration_ms"_L1].toInt(),
        getIsPlayable(json),
        SpotifyAlbumInfo::fromJson(json["album"_L1].toObject()),
        SpotifyArtist::fromJson(json["artists"_L1].toArray()),
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
    return SpotifyTrack::fromJsonArray(json["tracks"_L1].toArray());
}

auto SpotifyTrack::artistString() const -> QString
{
    QStringList names;
    names.reserve(artists.count());

    for (const auto &artist : artists)
    {
        names << artist->name;
    }

    return names.join(u", "_s);
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
    if (json.contains("is_playable"_L1))
    {
        return json["is_playable"_L1].toBool();
    }

    if (json.contains("available_markets"_L1))
    {
        return !json["available_markets"_L1].toArray().isEmpty();
    }

    return true;
}
