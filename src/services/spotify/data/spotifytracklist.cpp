#include "spotifytracklist.h"
#include "spotifytrack.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyTrackList, "gm.services.spotify.data.tracklist")

auto SpotifyTrackList::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrackList>
{
    const auto items = json[QStringLiteral("items")].toArray();

    QList<QSharedPointer<SpotifyTrack>> tracks;
    tracks.reserve(items.count());

    for (const auto &item : items)
    {
        tracks << SpotifyTrack::fromJson(item.toObject());
    }

    const auto next = json[QStringLiteral("next")].toString();

    return QSharedPointer<SpotifyTrackList>(new SpotifyTrackList{tracks, next});
}

auto SpotifyTrackList::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyTrackList>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyTrackList::fromJson(json);
}

void SpotifyTrackList::append(const SpotifyTrackList &other)
{
    tracks << other.tracks;
    next = other.next;
}
