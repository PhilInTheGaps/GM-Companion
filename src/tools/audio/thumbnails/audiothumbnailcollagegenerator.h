#pragma once

#include "../project/audioelement.h"
#include <QFuture>
#include <QNetworkAccessManager>
#include <QPixmap>

class AudioThumbnailCollageGenerator
{
public:
    static auto makeCollageAsync(QPointer<AudioElement> element, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>;

private:
    static auto canMakeCollage(QPointer<AudioElement> element) -> bool;
    static auto getCoverArtAsync(QPointer<AudioElement> element, QPointer<AudioFile> audioFile, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>;
    static auto generateCollageImage(QPointer<AudioElement> element) -> QPixmap;
    static auto getUniquePixmaps(QPointer<AudioThumbnail> thumbnail) -> QList<QPixmap>;

    static auto findPixmapsForCollageAsync(QPointer<AudioElement> element, qsizetype index, int fileCount,
                                           int failCount, QNetworkAccessManager *networkManager) -> QFuture<void>;

    static auto getTargetRect(QSize imageSize, int imageCount, int index) -> QRectF;
    static auto getSourceRect(QRect imageRect, int imageCount, int index) -> QRectF;

    /// The width and height of the generated
    static constexpr auto thumbnailSize() -> QSize
    {
        return {512, 512};
    }

    static constexpr auto FAILS_UNTIL_SKIP = 3;
    static constexpr auto SKIP_FACTOR = 1 / 10;
};
