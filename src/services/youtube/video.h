#pragma once

#include "videoid.h"
#include <QString>

namespace Services
{

struct YouTubeVideo
{
    VideoId id;
    QString title;
    QString uploader;
    QString thumbnailUrl;
    QString audioStreamUrl;
};

} // namespace Services
