#pragma once

#include "spotifytrack.h"
#include <QJsonObject>
#include <QStringList>

struct SpotifyTrackList
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrackList>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyTrackList>;

    void append(const SpotifyTrackList &other);

    QList<QSharedPointer<SpotifyTrack>> tracks;
    QString next;
};
