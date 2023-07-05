#pragma once

#include "basespotifyelement.h"
#include "spotifyalbuminfo.h"
#include "spotifyartist.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

struct SpotifyTrack : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyTrack>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyTrack>;
    [[nodiscard]] static auto fromJsonArray(const QJsonArray &json) -> std::vector<QSharedPointer<SpotifyTrack>>;
    [[nodiscard]] static auto fromJsonArray(const QByteArray &data) -> std::vector<QSharedPointer<SpotifyTrack>>;

    int durationMs;
    bool isPlayable;

    QSharedPointer<SpotifyAlbumInfo> album;
    QList<QSharedPointer<SpotifyArtist>> artists;

    [[nodiscard]] auto artistString() const -> QString;
    [[nodiscard]] auto image() const -> QSharedPointer<SpotifyImage>;

private:
    [[nodiscard]] static auto getIsPlayable(const QJsonObject &json) -> bool;
};
