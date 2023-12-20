#include "audiothumbnailcollagegenerator.h"
#include "audiothumbnail.h"
#include "audiothumbnailgenerator.h"
#include "loaders/spotifyimageloader.h"
#include "loaders/tagimageloader.h"
#include "loaders/youtubeimageloader.h"
#include <QPainter>

auto AudioThumbnailCollageGenerator::makeCollageAsync(QPointer<AudioElement> element,
                                                      QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    // Check if collage can be generated
    if (!canMakeCollage(element))
    {
        return QtFuture::makeReadyFuture(AudioThumbnailGenerator::getPlaceholderImage(element));
    }

    // Get pixmaps to use in the collage
    auto future = findPixmapsForCollageAsync(element, 0, 0, 0, networkManager);

    return future.then([element]() { return generateCollageImage(element); });
}

auto AudioThumbnailCollageGenerator::findPixmapsForCollageAsync(QPointer<AudioElement> element, qsizetype index,
                                                                int fileCount, int failCount,
                                                                QNetworkAccessManager *networkManager) -> QFuture<void>
{
    if (!element) return QtFuture::makeReadyFuture();

    const QPointer<AudioFile> audioFile = element->files().at(index);
    if (!audioFile) return QtFuture::makeReadyFuture();

    const auto callback = [element, audioFile, index, fileCount, failCount, networkManager](const QPixmap &pixmap) {
        if (!element) return QtFuture::makeReadyFuture(); // element has possibly been deleted by now

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
                return findPixmapsForCollageAsync(element, newIndex, fileCount, failCount + 1, networkManager);
            }
        }
        else
        {
            // Store pixmap as a collage icon and find next one (if less than 4 have been found)
            auto pair = std::pair<QString, QPixmap>(audioFile->url(), pixmap);
            element->thumbnail()->addCollageImage(pair);

            if (fileCount + 1 < 4 && index + 1 < audioFileCount)
            {
                return findPixmapsForCollageAsync(element, index + 1, fileCount + 1, 0, networkManager);
            }
        }

        return QtFuture::makeReadyFuture();
    };

    return getCoverArtAsync(element, audioFile, networkManager).then(callback).unwrap();
}

/**
 * @brief Check if a collage can be generated.
 * False if element is nullptr or of type radio or does not contain any files.
 */
auto AudioThumbnailCollageGenerator::canMakeCollage(QPointer<AudioElement> element) -> bool
{
    return element && element->type() != AudioElement::Type::Radio && !element->files().isEmpty();
}

/**
 * @brief Get the cover art of an audio file.
 */
auto AudioThumbnailCollageGenerator::getCoverArtAsync(QPointer<AudioElement> element, QPointer<AudioFile> audioFile,
                                                      QNetworkAccessManager *networkManager) -> QFuture<QPixmap>
{
    if (!element || !audioFile) return QtFuture::makeReadyFuture(QPixmap());

    switch (audioFile->source())
    {
    case AudioFile::Source::File:
        return TagImageLoader::loadImageAsync(element, audioFile);
    case AudioFile::Source::Spotify:
        return SpotifyImageLoader::loadImageAsync(audioFile);
    case AudioFile::Source::Youtube: {
        if (const auto id = Services::VideoId(audioFile->url()); id.isValid())
            return YouTubeImageLoader::loadImageAsync(id, networkManager);

        if (const auto id = Services::PlaylistId(audioFile->url()); id.isValid())
            return YouTubeImageLoader::loadImageAsync(id, networkManager);

        return QtFuture::makeReadyFuture(QPixmap());
    }
    default:
        return QtFuture::makeReadyFuture(QPixmap());
    }
}

auto AudioThumbnailCollageGenerator::generateCollageImage(QPointer<AudioElement> element) -> QPixmap
{
    if (!element) return {};

    auto *thumbnail = element->thumbnail();

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

        const QRectF target = getTargetRect(thumbnailSize(), static_cast<int>(images.count()), i);
        const QRectF source = getSourceRect(images.at(i).rect(), static_cast<int>(images.count()), i);
        painter.drawPixmap(target, images.at(i), source);
    }
    painter.end();

    return image;
}

auto AudioThumbnailCollageGenerator::getUniquePixmaps(QPointer<AudioThumbnail> thumbnail) -> QList<QPixmap>
{
    if (!thumbnail) return {};

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
    const qreal top = imageCount == 4 && index > 1 ? height : 0;

    return {left, top, width, height};
}

/**
 * Calculate which part of the image should be used for the collage.
 * For non-square images a square is cut out from the center.
 * If there are more than one and less than 4 images in the collage,
 * only a vertical bar is taken from the image.
 */
auto AudioThumbnailCollageGenerator::getSourceRect(QRect imageRect, int imageCount, int index) -> QRectF
{
    const auto widthHeightDiff = imageRect.width() - imageRect.height();
    const qreal squareSideLength = imageRect.width() > imageRect.height() ? imageRect.height() : imageRect.width();

    const qreal left = widthHeightDiff > 0 ? widthHeightDiff / 2 : 0;
    const qreal top = widthHeightDiff > 0 ? 0 : -widthHeightDiff / 2;

    if (imageCount == 1 || imageCount > 3) return {left, top, squareSideLength, squareSideLength};

    const qreal width = squareSideLength / imageCount;

    return {left + width * index, top, width, squareSideLength};
}
