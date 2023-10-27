#include "spotifycurrenttrack.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyCurrentTrack::fromJson(const QJsonObject &json) -> SpotifyCurrentTrack
{
    return {std::chrono::milliseconds(json["progress_ms"_L1].toInt()),
            SpotifyTrack::fromJson(json["item"_L1].toObject())};
}

auto SpotifyCurrentTrack::fromJson(const QByteArray &data) -> SpotifyCurrentTrack
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyCurrentTrack::fromJson(json);
}
