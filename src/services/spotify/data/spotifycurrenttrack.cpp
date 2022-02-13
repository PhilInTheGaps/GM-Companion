#include "spotifycurrenttrack.h"

#include <QJsonDocument>

auto SpotifyCurrentTrack::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyCurrentTrack>
{
    auto *track = new SpotifyCurrentTrack {
        json[QStringLiteral("progress_ms")].toInt(),
        SpotifyTrack::fromJson(json[QStringLiteral("item")].toObject())
    };

    return QSharedPointer<SpotifyCurrentTrack>(track);
}

auto SpotifyCurrentTrack::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyCurrentTrack>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyCurrentTrack::fromJson(json);
}
