#pragma once

#include <QStringList>
#include <QJsonObject>
#include "spotifytrack.h"

struct SpotifyTrackList
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrackList>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyTrackList>;

    void append(const SpotifyTrackList &other);

    QVector<QSharedPointer<SpotifyTrack>> tracks;
    QString next;
};

