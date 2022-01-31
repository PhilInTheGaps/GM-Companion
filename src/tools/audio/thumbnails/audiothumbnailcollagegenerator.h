#pragma once

#include <QFuture>
#include <QPixmap>
#include <QNetworkAccessManager>
#include "../project/audioelement.h"

class AudioThumbnailCollageGenerator
{
public:
    static auto makeCollageAsync(AudioElement *element) -> QFuture<QPixmap>;

private:
    static auto canMakeCollage(AudioElement *element) -> bool;
    static auto getCoverArtAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>;
    static auto generateCollageImage(AudioElement *element) -> QPixmap;
    static auto getUniquePixmaps(AudioThumbnail *thumbnail) -> QList<QPixmap>;

    static auto findPixmapsForCollageAsync(AudioElement *element, int index, int fileCount, int failCount) -> QFuture<void>;

    static auto getTargetRect(QSize imageSize, int imageCount, int index) -> QRectF;
    static auto getSourceRect(QRect imageRect, int imageCount, int index) -> QRectF;

    /// The width and height of the generated
    static constexpr auto thumbnailSize() -> QSize { return { 512, 512 }; }

    static constexpr auto FAILS_UNTIL_SKIP = 3;
    static constexpr auto SKIP_FACTOR = 1/10;
};
