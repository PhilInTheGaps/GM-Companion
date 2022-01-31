#include "audiothumbnailgenerator.h"
#include "audiothumbnailcache.h"
#include "audiothumbnailcollagegenerator.h"
#include "audiothumbnail.h"
#include "loaders/webimageloader.h"
#include "loaders/fileimageloader.h"
#include "loaders/spotifyimageloader.h"
#include "settings/settingsmanager.h"
#include <QtConcurrent/QtConcurrentRun>
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(gmAudioThumbnailGenerator, "gm.audio.thumbnails.generator")

using namespace AsyncFuture;

QPointer<QNetworkAccessManager> AudioThumbnailGenerator::networkManager = nullptr;

void AudioThumbnailGenerator::configureNetworkManager()
{
    if (!networkManager)
    {
        networkManager = new QNetworkAccessManager;
        networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    }
}

void AudioThumbnailGenerator::generateThumbnails(AudioScenario *scenario)
{
    if (!scenario) return;

    configureNetworkManager();

    for (auto element : scenario->elements(true))
    {
        if (!element) continue;

        generateThumbnail(element);
    }
}

void AudioThumbnailGenerator::receivedImage(AudioElement *element, const QPixmap &pixmap, bool makeFallbackCollage)
{
    if (!element || !element->thumbnail())
    {
        qCCritical(gmAudioThumbnailGenerator()) << "Received image but related element or thumbnail object is null";
        return;
    }

    if (pixmap.isNull() && makeFallbackCollage)
    {
        qCDebug(gmAudioThumbnailGenerator()) << "Received pixmap is null, making collage as fallback ...";
        observe(AudioThumbnailCollageGenerator::makeCollageAsync(element))
                .subscribe([element](const QPixmap &pixmap) {
            receivedImage(element, pixmap, false);
        });
        return;
    }

    AudioThumbnailCache::instance()->insertImage(element->thumbnail()->imageId(), pixmap);
    element->thumbnail()->update();
}

void AudioThumbnailGenerator::generateThumbnail(AudioElement *element)
{
    // Paranoid check
    if (!element || !element->thumbnail()) return ;

    qCDebug(gmAudioThumbnailGenerator()) << "Generating thumbnail for element"
                                         << QString(*element) << "...";

    const auto iconPath = element->thumbnail()->absoluteUrl();
    const auto callbackWithFallback = [element](const QPixmap &pixmap) {
        receivedImage(element, pixmap, true);
    };

    // Is web url
    if (iconPath.startsWith("http://") ||
        iconPath.startsWith("https://"))
    {
        observe(WebImageLoader::loadImageAsync(iconPath, networkManager))
                .subscribe(callbackWithFallback);
        return;
    }

    // Is a local file
    if (!iconPath.isEmpty())
    {
        observe(FileImageLoader::loadImageAsync(iconPath))
                .subscribe(callbackWithFallback);
        return;
    }

    // If no explicit thumbnail has been specified, generate collage
    const auto callbackWithoutFallback = [element](const QPixmap &pixmap) {
        receivedImage(element, pixmap, false);
    };

    observe(AudioThumbnailCollageGenerator::makeCollageAsync(element))
            .subscribe(callbackWithoutFallback);
}

auto AudioThumbnailGenerator::getPlaceholderImage(AudioElement *element) -> QPixmap
{
    if (!element) return emptyPixmap();
    return getPlaceholderImage(element->type());
}

auto AudioThumbnailGenerator::getPlaceholderImage(const QString &type) -> QPixmap
{
    if (type == "Sound") return getPlaceholderImage(AudioElement::Sound);

    if (type == "Radio") return getPlaceholderImage(AudioElement::Radio);

    if (type == "Music") return getPlaceholderImage(AudioElement::Music);

    return emptyPixmap();
}

auto AudioThumbnailGenerator::getPlaceholderImage(AudioElement::Type type) -> QPixmap
{
    switch (type)
    {
    case AudioElement::Type::Music: return QPixmap(":/icons/media/music_image.png");

    case AudioElement::Type::Sound: return QPixmap(":/icons/media/sound_image.png");

    case AudioElement::Type::Radio: return QPixmap(":/icons/media/radio_image.png");

    default: return emptyPixmap();
    }
}

auto AudioThumbnailGenerator::emptyPixmap() -> QPixmap
{
    QPixmap img;
    img.fill(Qt::black);
    return img;
}
