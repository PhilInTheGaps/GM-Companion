#pragma once

#include "basespotifyelement.h"
#include "spotifyimage.h"
#include <QJsonArray>
#include <QJsonObject>
#include <vector>

namespace Services
{

struct SpotifyArtist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyArtist;
    [[nodiscard]] static auto fromJson(const QJsonArray &json) -> std::vector<SpotifyArtist>;

    std::vector<SpotifyImage> images;
};

} // namespace Services
