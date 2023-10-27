#pragma once

#include "spotifytrack.h"
#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <vector>

namespace Services
{

struct SpotifyTrackList
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyTrackList;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyTrackList;

    void append(const SpotifyTrackList &other);

    std::vector<SpotifyTrack> tracks;
    QString next;
};

} // namespace Services
