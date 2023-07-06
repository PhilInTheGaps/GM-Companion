#include "spotifycurrenttrack.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;

auto SpotifyCurrentTrack::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyCurrentTrack>
{
    auto *track =
        new SpotifyCurrentTrack{json["progress_ms"_L1].toInt(), SpotifyTrack::fromJson(json["item"_L1].toObject())};

    return QSharedPointer<SpotifyCurrentTrack>(track);
}

auto SpotifyCurrentTrack::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyCurrentTrack>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyCurrentTrack::fromJson(json);
}
