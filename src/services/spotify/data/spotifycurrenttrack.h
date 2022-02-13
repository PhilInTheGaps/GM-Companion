#pragma once

#include <QJsonObject>
#include "spotifytrack.h"

struct SpotifyCurrentTrack
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyCurrentTrack>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyCurrentTrack>;

    int progressMs = 0;
    QSharedPointer<SpotifyTrack> track;
};

