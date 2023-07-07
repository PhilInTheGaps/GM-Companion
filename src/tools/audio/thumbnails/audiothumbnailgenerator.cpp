#include "audiothumbnailgenerator.h"
#include "audiothumbnail.h"
#include "audiothumbnailcache.h"
#include "audiothumbnailcollagegenerator.h"
#include "loaders/fileimageloader.h"
#include "loaders/spotifyimageloader.h"
#include "loaders/webimageloader.h"
#include "settings/settingsmanager.h"
#include <QLoggingCategory>
#include <QtConcurrent/QtConcurrentRun>

Q_LOGGING_CATEGORY(gmAudioThumbnailGenerator, "gm.audio.thumbnails.generator")

using namespace Qt::Literals::StringLiterals;

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
        AudioThumbnailCollageGenerator::makeCollageAsync(element).then(
            [element](const QPixmap &pixmap) { receivedImage(element, pixmap, false); });
        return;
    }

    AudioThumbnailCache::instance()->insertImage(element->thumbnail()->imageId(), pixmap);
    element->thumbnail()->update();
}

void AudioThumbnailGenerator::generateThumbnail(AudioElement *element)
{
    // Paranoid check
    if (!element || !element->thumbnail()) return;

    qCDebug(gmAudioThumbnailGenerator()) << "Generating thumbnail for element" << QString(*element) << "...";

    const auto iconPath = element->thumbnail()->absoluteUrl();
    const auto callbackWithFallback = [element](const QPixmap &pixmap) { receivedImage(element, pixmap, true); };

    // Is web url
    if (iconPath.startsWith("http://"_L1) || iconPath.startsWith("https://"_L1))
    {
        WebImageLoader::loadImageAsync(iconPath, networkManager).then(element, callbackWithFallback);
        return;
    }

    // Is a local file
    if (!iconPath.isEmpty())
    {
        FileImageLoader::loadImageAsync(iconPath).then(element, callbackWithFallback);
        return;
    }

    // If no explicit thumbnail has been specified, generate collage
    const auto callbackWithoutFallback = [element](const QPixmap &pixmap) { receivedImage(element, pixmap, false); };

    AudioThumbnailCollageGenerator::makeCollageAsync(element).then(element, callbackWithoutFallback);
}

auto AudioThumbnailGenerator::getPlaceholderImage(AudioElement *element) -> QPixmap
{
    if (!element) return emptyPixmap();
    return getPlaceholderImage(element->type());
}

auto AudioThumbnailGenerator::getPlaceholderImage(const QString &type) -> QPixmap
{
    if (type == "Sound"_L1) return getPlaceholderImage(AudioElement::Type::Sound);

    if (type == "Radio"_L1) return getPlaceholderImage(AudioElement::Type::Radio);

    if (type == "Music"_L1) return getPlaceholderImage(AudioElement::Type::Music);

    return emptyPixmap();
}

auto AudioThumbnailGenerator::getPlaceholderImage(AudioElement::Type type) -> QPixmap
{
    switch (type)
    {
    case AudioElement::Type::Music:
        return QPixmap(u":/icons/media/music_image.png"_s);

    case AudioElement::Type::Sound:
        return QPixmap(u":/icons/media/sound_image.png"_s);

    case AudioElement::Type::Radio:
        return QPixmap(u":/icons/media/radio_image.png"_s);

    default:
        return emptyPixmap();
    }
}

auto AudioThumbnailGenerator::emptyPixmap() -> QPixmap
{
    QPixmap img;
    img.fill(Qt::black);
    return img;
}
