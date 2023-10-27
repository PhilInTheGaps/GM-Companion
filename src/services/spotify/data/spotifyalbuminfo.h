#pragma once

#include "basespotifyelement.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include <QString>
#include <vector>

namespace Services
{

struct SpotifyAlbumInfo : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyAlbumInfo;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyAlbumInfo;

    QString releaseDate;

    std::vector<SpotifyImage> images;
    std::vector<SpotifyArtist> artists;
};

} // namespace Services
