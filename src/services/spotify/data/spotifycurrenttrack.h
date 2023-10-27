#pragma once

#include "spotifytrack.h"
#include <QByteArray>
#include <QJsonObject>
#include <chrono>

namespace Services
{

struct SpotifyCurrentTrack
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyCurrentTrack;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyCurrentTrack;

    std::chrono::milliseconds progress = std::chrono::milliseconds(0);
    SpotifyTrack track;
};

} // namespace Services
