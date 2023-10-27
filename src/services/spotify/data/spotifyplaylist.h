#pragma once

#include "basespotifyelement.h"
#include "spotifyimage.h"
#include "spotifytracklist.h"
#include <QJsonObject>
#include <QString>
#include <vector>

namespace Services
{

struct SpotifyPlaylist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> SpotifyPlaylist;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> SpotifyPlaylist;

    QString description;
    QString snapshotId;

    bool isCollaborative;
    bool isPublic;

    std::vector<SpotifyImage> images;
    SpotifyTrackList tracks;
};

} // namespace Services
