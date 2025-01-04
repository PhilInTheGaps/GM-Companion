#pragma once

#include "playlistid.h"
#include "video.h"
#include <QString>
#include <vector>

namespace Services
{

struct YouTubePlaylist
{
    PlaylistId id;
    QString title;
    QString uploader;
    QString thumbnailUrl;
    std::vector<YouTubeVideo> streams;
};

} // namespace Services
