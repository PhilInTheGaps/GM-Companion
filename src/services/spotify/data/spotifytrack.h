#pragma once

#include "basespotifyelement.h"
#include "spotifyalbuminfo.h"
#include "spotifyartist.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <chrono>
#include <vector>

namespace Services
{

struct SpotifyTrack : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyTrack;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyTrack;
    [[nodiscard]] static auto fromJsonArray(const QJsonArray &json) -> std::vector<SpotifyTrack>;
    [[nodiscard]] static auto fromJsonArray(const QByteArray &data) -> std::vector<SpotifyTrack>;

    std::chrono::milliseconds duration;
    bool isPlayable;

    SpotifyAlbumInfo album;
    std::vector<SpotifyArtist> artists;

    [[nodiscard]] auto artistNames() const -> QStringList;
    [[nodiscard]] auto artistString() const -> QString;
    [[nodiscard]] auto image() const -> const SpotifyImage &;

private:
    [[nodiscard]] static auto getIsPlayable(const QJsonObject &json) -> bool;
};

} // namespace Services
