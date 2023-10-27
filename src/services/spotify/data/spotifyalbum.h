#pragma once

#include "basespotifyelement.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include "spotifytracklist.h"
#include <QJsonObject>
#include <vector>

namespace Services
{

struct SpotifyAlbum : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyAlbum;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyAlbum;

    QString releaseDate;

    std::vector<SpotifyImage> images;
    std::vector<SpotifyArtist> artists;

    SpotifyTrackList tracks;
};

} // namespace Services
