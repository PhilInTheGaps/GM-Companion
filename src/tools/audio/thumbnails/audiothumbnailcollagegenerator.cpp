#include "audiothumbnailcollagegenerator.h"
#include "audiothumbnail.h"
#include "audiothumbnailgenerator.h"
#include "loaders/spotifyimageloader.h"
#include "loaders/tagimageloader.h"
#include <QPainter>

auto AudioThumbnailCollageGenerator::makeCollageAsync(AudioElement *element) -> QFuture<QPixmap>
{
    // Check if collage can be generated
    if (!canMakeCollage(element))
    {
        return QtFuture::makeReadyFuture(AudioThumbnailGenerator::getPlaceholderImage(element));
    }

    // Get pixmaps to use in the collage
    auto future = findPixmapsForCollageAsync(element, 0, 0, 0);

    return future.then(element, [element]() { return generateCollageImage(element); });
}

auto AudioThumbnailCollageGenerator::findPixmapsForCollageAsync(AudioElement *element, int index, int fileCount,
                                                                int failCount) -> QFuture<void>
{
    auto *audioFile = element->files().at(index);
    if (!audioFile) return QtFuture::makeReadyFuture();

    const auto callback = [element, audioFile, index, fileCount, failCount](const QPixmap &pixmap) {
        const auto audioFileCount = element->files().length();

        if (pixmap.isNull())
        {
            auto newIndex = index + 1;

            // Skip a tenth of the list if no new icon was found three files in a row
            if (failCount >= FAILS_UNTIL_SKIP)
            {
                newIndex = index + audioFileCount * SKIP_FACTOR;
            }

            if (newIndex < audioFileCount && newIndex > index)
            {
                return findPixmapsForCollageAsync(element, newIndex, fileCount, failCount + 1);
            }
        }
        else
        {
            // Store pixmap as a collage icon and find next one (if less than 4 have been found)
            auto pair = std::pair<QString, QPixmap>(audioFile->url(), pixmap);
            element->thumbnail()->addCollageImage(pair);

            if (fileCount + 1 < 4 && index + 1 < audioFileCount)
            {
                return findPixmapsForCollageAsync(element, index + 1, fileCount + 1, 0);
            }
        }

        return QtFuture::makeReadyFuture();
    };

    auto future = getCoverArtAsync(element, audioFile);
    return future.then(element, callback).unwrap();
}

/**
 * @brief Check if a collage can be generated.
 * False if element is nullptr or of type radio or does not contain any files.
 */
auto AudioThumbnailCollageGenerator::canMakeCollage(AudioElement *element) -> bool
{
    return element && element->type() != AudioElement::Type::Radio && element->files().length() > 0;
}

/**
 * @brief Get the cover art of an audio file.
 */
auto AudioThumbnailCollageGenerator::getCoverArtAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>
{
    switch (audioFile->source())
    {
    case AudioFile::Source::File:
        return TagImageLoader::loadImageAsync(element, audioFile);
    case AudioFile::Source::Spotify:
        return SpotifyImageLoader::loadImageAsync(audioFile);
    case AudioFile::Source::Youtube:
        // TODO: implement
    default:
        return QtFuture::makeReadyFuture(QPixmap());
    }
}

auto AudioThumbnailCollageGenerator::generateCollageImage(AudioElement *element) -> QPixmap
{
    auto thumbnail = element->thumbnail();

    if (!thumbnail || thumbnail->collageImages().isEmpty()) return {};

    // Create collage
    QPixmap image(thumbnailSize());
    QPainter painter;
    painter.begin(&image);
    painter.setBackgroundMode(Qt::OpaqueMode);

    auto images = getUniquePixmaps(thumbnail);

    for (int i = 0; i < images.count(); i++)
    {
        if (i > 3) break;

        QRectF target = getTargetRect(thumbnailSize(), images.count(), i);
        QRectF source = getSourceRect(images[i].rect(), images.count(), i);
        painter.drawPixmap(target, images[i], source);
    }
    painter.end();

    return image;
}

auto AudioThumbnailCollageGenerator::getUniquePixmaps(AudioThumbnail *thumbnail) -> QList<QPixmap>
{
    QList<QPixmap> images;

    foreach (const auto &imagePair, thumbnail->collageImages())
    {
        bool doesExist = false;

        foreach (const auto &pixmap, images)
        {
            if (imagePair.second.toImage() == pixmap.toImage())
            {
                doesExist = true;
                break;
            }
        }

        if (!doesExist) images.append(imagePair.second);
    }

    return images;
}

auto AudioThumbnailCollageGenerator::getTargetRect(QSize imageSize, int imageCount, int index) -> QRectF
{
    if (imageCount > 3) imageCount = 4;

    const qreal width = imageCount == 4 ? imageSize.width() / 2 : imageSize.width() / imageCount;
    const qreal height = imageCount == 4 ? imageSize.height() / 2 : imageSize.height();
    const qreal left = imageCount == 4 ? width * ((index + 1) % 2) : width * index;
    const qreal top = imageCount == 4 ? (index > 1 ? height : 0) : 0;

    return {left, top, width, height};
}

auto AudioThumbnailCollageGenerator::getSourceRect(QRect imageRect, int imageCount, int index) -> QRectF
{
    if (imageCount > 3) return imageRect;

    const qreal width = imageRect.width() / imageCount;
    const qreal height = imageRect.height();
    const qreal left = width * index;
    const qreal top = 0;

    return {left, top, width, height};
}
