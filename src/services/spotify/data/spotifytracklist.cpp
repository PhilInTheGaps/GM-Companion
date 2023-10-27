#include "spotifytracklist.h"
#include "spotifytrack.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

Q_LOGGING_CATEGORY(gmSpotifyTrackList, "gm.services.spotify.data.tracklist")

auto SpotifyTrackList::fromJson(const QJsonObject &json) -> SpotifyTrackList
{
    const auto items = json["items"_L1].toArray();

    std::vector<SpotifyTrack> tracks;
    tracks.reserve(items.count());

    for (const auto &item : items)
    {
        tracks.push_back(SpotifyTrack::fromJson(item.toObject()));
    }

    const auto next = json["next"_L1].toString();

    return {tracks, next};
}

auto SpotifyTrackList::fromJson(const QByteArray &data) -> SpotifyTrackList
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyTrackList::fromJson(json);
}

void SpotifyTrackList::append(const SpotifyTrackList &other)
{
    tracks.insert(tracks.end(), other.tracks.begin(), other.tracks.end());
    next = other.next;
}
