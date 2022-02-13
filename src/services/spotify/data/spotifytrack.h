#pragma once

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include "basespotifyelement.h"
#include "spotifyartist.h"
#include "spotifyalbuminfo.h"

struct SpotifyTrack : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrack>;
    [[nodiscard]] static auto fromJson(const QJsonArray &json) -> QVector<QSharedPointer<SpotifyTrack>>;

    int durationMs;
    bool isPlayable;

    QSharedPointer<SpotifyAlbumInfo> album;
    QVector<QSharedPointer<SpotifyArtist>> artists;

    [[nodiscard]] auto artistString() const -> QString;
    [[nodiscard]] auto image() const -> QSharedPointer<SpotifyImage>;

private:
    [[nodiscard]] static auto getIsPlayable(const QJsonObject &json) -> bool;
};

