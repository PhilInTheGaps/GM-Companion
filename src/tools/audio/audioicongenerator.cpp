#include "audioicongenerator.h"
#include "logging.h"
#include "services/services.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"
#include "common/utils/utils.h"
#include "services/spotify/spotifyutils.h"
#include "project/audioicon.h"

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>

#include <QBuffer>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTemporaryFile>

using namespace TagLib;

QMap<QUrl, QPixmap> AudioIconGenerator::iconCache;
bool AudioIconGenerator::instanceFlag          = false;
AudioIconGenerator *AudioIconGenerator::single = nullptr;
QReadWriteLock AudioIconGenerator::cacheLock;

IconWorker::IconWorker(QString resourcesPath, QString musicPath, QString soundsPath)
    : m_musicPath(musicPath), m_soundsPath(soundsPath)
{
    connect(Spotify::getInstance(), &Spotify::authorized,           this,                       &IconWorker::onSpotifyAuthorized);
    connect(Spotify::getInstance(), &Spotify::receivedReply,        this,                       &IconWorker::onReceivedSpotifyReply);
    connect(this,                   &IconWorker::getSpotifyRequest, Spotify::getInstance(),     qOverload<QNetworkRequest, int>(&Spotify::get));
    connect(this,                   &IconWorker::getFile,           FileManager::getInstance(), &FileManager::getFile);
}

IconWorker::~IconWorker()
{
    if (m_networkManager) m_networkManager->deleteLater();
}

void IconWorker::generateThumbnails(AudioScenario *scenario)
{
    if (!scenario) return;

    if (!m_networkManager) m_networkManager = new QNetworkAccessManager;
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    m_scenarios.append(scenario);

    for (auto element : scenario->elements(true))
    {
        if (!element) continue;

        generateThumbnail(element);
    }

    getImagesFromSpotify();
}

auto IconWorker::getPlaceholderImage(AudioElement *element)->QPixmap
{
    if (element)
    {
        return AudioIconGenerator::getPlaceholderImage(element->type());
    }

    QPixmap img;
    img.fill(Qt::black);
    return img;
}

QPixmap AudioIconGenerator::getPlaceholderImage(AudioElement::Type type)
{
    switch (type)
    {
    case AudioElement::Type::Music: return QPixmap(":/icons/media/music_image.png");

    case AudioElement::Type::Sound: return QPixmap(":/icons/media/sound_image.png");

    default: break;
    }

    QPixmap img;
    img.fill(Qt::black);
    return img;
}

void IconWorker::generateThumbnail(AudioElement *element)
{
    // Paranoid check
    if (!element || !element->icon()) return;

    qCDebug(gmAudioIconGenerator()) << "Generating thumbnail for element" << element->name();

    auto iconPath = element->icon()->absoluteUrl();

    // Is web url?
    if (iconPath.startsWith("http://") ||
        iconPath.startsWith("https://"))
    {
        loadImageFromWeb(element, iconPath);
        return;
    }

    // Is empty?
    if (!iconPath.isEmpty())
    {
        loadImageFromPath(element, iconPath);
        return;
    }

    // Can not make collage
    if (element->type() == AudioElement::Type::Radio) return;

    // If no icon was specified, generate collage
    makeCollage(element);
}

/**
 * @brief Make a collage image from audio thumbnails
 */
void IconWorker::makeCollage(AudioElement *element)
{
    // Paranoid check
    if (!element) return;

    // Select files for collage
    auto fileCount = 0, failCount = 0;

    for (auto i = 0; i < element->files().length(); i++)
    {
        const auto file = element->files()[i];

        // Skip all files that are not local (0) or from spotify (2)
        // TODO 1: from url
        // TODO 3: from youtube
        if ((file->source() == 0) || (file->source() == 2))
        {
            if (element->icon()->addCollageIcon(QPair<QString, QPixmap>(file->url(), getImageFromAudioFile((element), file))))
            {
                failCount = 0;

                if (++fileCount >= 4) break;
            }

            // Skip a tenth of the list if no new icon was found three files in
            // a row
            else if (++failCount > 2)
            {
                i += element->files().length() / 10;
            }
        }
    }

    generateCollageImage(element);
}

void IconWorker::loadImageFromWeb(AudioElement *element, const QString& url)
{
    qCDebug(gmAudioIconGenerator()) << "Loading image from web:" << url;

    // Try to load from cache
    if (AudioIconGenerator::tryLoadFromCache(url, element)) return;

    // Load from url
    auto request = QNetworkRequest(QUrl(url));
    auto reply   = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [ = ]() {
        if (reply->error() != QNetworkReply::NoError) {
            qCCritical(gmAudioIconGenerator()) << reply->error() << reply->errorString() << "Could not load image from" << url;
            reply->deleteLater();
        }


        QPixmap image;

        if (image.loadFromData(reply->readAll()))
        {
            AudioIconGenerator::writeToCache(url,                        image);
            AudioIconGenerator::writeToCache(element->icon()->imageId(), image);
            element->icon()->update();
        }
        else
        {
            qCCritical(gmAudioIconGenerator()) << "Failed to load image data for" << url;
        }

        reply->deleteLater();
    });
}

void IconWorker::loadImageFromPath(AudioElement *element, const QString& filePath)
{
    // Try to load from cache
    if (AudioIconGenerator::tryLoadFromCache(filePath, element)) return;

    auto requestId = FileManager::getUniqueRequestId();

    auto *context = new QObject;
    connect(FileManager::getInstance(), &FileManager::receivedFile, context, [ = ](int id, QByteArray data) {
        if (requestId != id) return;

        QPixmap image;
        image.loadFromData(data);
        AudioIconGenerator::writeToCache(filePath,                   image);
        AudioIconGenerator::writeToCache(element->icon()->imageId(), image);
        element->icon()->update();
        delete context;
    });

    emit getFile(requestId, filePath);
}

auto IconWorker::getImageFromAudioFile(AudioElement *element, AudioFile *audioFile)->QPixmap
{
    // Paranoid pointer check
    if (!element || !audioFile) return getPlaceholderImage(element);

    // Return placeholder image for spotify
    // Also add url to list for batch-retrieval later
    if (audioFile->source() == 2)
    {
        if (AudioIconGenerator::cacheContains(SpotifyUtils::getIdFromUri(audioFile->url()))) {
            return AudioIconGenerator::readFromCache(SpotifyUtils::getIdFromUri(audioFile->url()));
        }
        m_spotifyIconList.append(audioFile->url());
        return getPlaceholderImage(element);
    }

    // Return empty placeholder image for non-local files
    if (audioFile->source() != 0) return getPlaceholderImage(element);

    // Resolve relative path
    QString path = element->type() == AudioElement::Type::Music ? m_musicPath + audioFile->url() : m_soundsPath + audioFile->url();

    // Check if icon cache contains image already
    if (AudioIconGenerator::cacheContains(QUrl(path)))
    {
        return AudioIconGenerator::readFromCache(path);
    }

    // Get id3v2 meta data tag
    FileRef f(path.toUtf8().data());

    if (auto *file = dynamic_cast<TagLib::MPEG::File *>(f.file()))
    {
        if (file->ID3v2Tag() && !file->ID3v2Tag()->isEmpty())
        {
            auto tag = file->ID3v2Tag();

            if (!tag)
            {
                qCWarning(gmAudioIconGenerator) << "Could not read meta data tags from file" << path;
                return getPlaceholderImage(element);
            }

            // Get frames from tag for image
            auto frames = tag->frameList("APIC");

            if (frames.isEmpty())
            {
                qCWarning(gmAudioIconGenerator) << "Meta data tags do not contain images" << path;
                return getPlaceholderImage(element);
            }

            // Convert image to QImage
            auto frame = dynamic_cast<ID3v2::AttachedPictureFrame *>(frames.front());
            QPixmap coverImage;
            coverImage.loadFromData(QByteArray(frame->picture().data(), static_cast<int>(frame->picture().size())));

            qCDebug(gmAudioIconGenerator) << "Successfully loaded image from audio file" << path;
            return coverImage;
        }

        qCWarning(gmAudioIconGenerator) << "File does not contain id3v2 tags" << path;
        return getPlaceholderImage(element);
    }

    qCCritical(gmAudioIconGenerator) << "Could not cast taglib fileref.file() to taglib mpeg file" << path;
    return getPlaceholderImage(element);
}

void IconWorker::getImagesFromSpotify()
{
    m_spotifyIconList.removeDuplicates();

    while (!m_spotifyIconList.isEmpty())
    {
        QString firstUri = m_spotifyIconList.takeFirst();
        QString firstId  = SpotifyUtils::getIdFromUri(firstUri);
        int     type     = SpotifyUtils::getUriType(firstUri);

        QStringList idBatch;
        idBatch.append(firstId);
        int index = 0;

        while (type != 1 && index < m_spotifyIconList.length() && SPOTIFY_BATCH_SIZE > idBatch.length())
        {
            if (SpotifyUtils::getUriType(m_spotifyIconList[index]) == type)
            {
                idBatch.append(SpotifyUtils::getIdFromUri(m_spotifyIconList.takeAt(index)));
            }
            else
            {
                index++;
            }
        }

        QUrl url;

        switch (type)
        {
        case 0:
            url = "https://api.spotify.com/v1/albums";
            break;

        case 1: url = "https://api.spotify.com/v1/playlists/" + firstId;
            break;

        case 2: url = "https://api.spotify.com/v1/tracks";
            break;

        default:
            qCCritical(gmAudioIconGenerator) << "Invalid type" << type;
            return;
        }

        if (type != 1)
        {
            QUrlQuery query;
            query.addQueryItem("ids", idBatch.join(","));
            url.setQuery(query);
        }

        auto requestId = Spotify::getInstance()->getUniqueRequestId();
        m_spotifyRequestList.append(requestId);
        emit getSpotifyRequest(QNetworkRequest(url), requestId);
    }
}

void IconWorker::insertImage(const QPixmap& image, const QString& uri)
{
    for (auto scenario : m_scenarios)
    {
        if (!scenario) continue;

        for (auto element : scenario->elements(true))
        {
            if (!element || !element->icon()) continue;

            for (int i = 0; i < element->icon()->collageIcons().length(); i++)
            {
                if (element->icon()->collageIcons()[i].first.contains(uri))
                {
                    element->icon()->setCollageIcon(QPair<QString, QPixmap>(uri, image), i);
                    generateCollageImage(element);
                    break;
                }
            }
        }
    }
}

void IconWorker::insertImageFromUrl(const QString& imageUrl, QString uri)
{
    qCDebug(gmAudioIconGenerator) << "Inserting image" << imageUrl << uri;

    if (uri.isEmpty()) uri = imageUrl;

    if (AudioIconGenerator::cacheContains(imageUrl))
    {
        insertImage(AudioIconGenerator::readFromCache(imageUrl), uri);
        return;
    }

    QNetworkRequest request(imageUrl);
    auto reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [ = ]()
    {
        auto data = reply->readAll();
        reply->deleteLater();

        QPixmap image;

        if (!image.loadFromData(data))
        {
            qCDebug(gmAudioIconGenerator) << "Could not read image from data:" << imageUrl;
            return;
        }

        AudioIconGenerator::writeToCache(SpotifyUtils::getIdFromUri(uri), image);
        AudioIconGenerator::writeToCache(imageUrl,                   image);
        insertImage(image, uri);
    });
}

void IconWorker::insertImageFromSpotifyPlaylist(QJsonObject playlist)
{
    QString imageUrl = playlist["images"].toArray()[0].toObject()["url"].toString();
    QString id       = playlist["id"].toString();

    qCDebug(gmAudioIconGenerator) << "Inserting image from spotify playlist:" << id << imageUrl;
    insertImageFromUrl(imageUrl, id);
}

void IconWorker::insertImageFromSpotifyAlbum(QJsonObject album)
{
    QString imageUrl = album["images"].toArray()[0].toObject()["url"].toString();
    QString id       = album["id"].toString();

    qCDebug(gmAudioIconGenerator) << "Inserting image from spotify album:" << id << imageUrl;
    insertImageFromUrl(imageUrl, id);
}

void IconWorker::insertImageFromSpotifyTrack(QJsonObject track)
{
    QString imageUrl = track["album"].toObject()["images"].toArray()[0].toObject()["url"].toString();
    QString id       = track["id"].toString();

    qCDebug(gmAudioIconGenerator) << "Inserting image from spotify track:" << id << imageUrl;
    insertImageFromUrl(imageUrl, id);
}

void IconWorker::onReceivedSpotifyReply(int id, QNetworkReply::NetworkError error, const QByteArray& data)
{
    if (!m_spotifyRequestList.contains(id)) return;

    qCDebug(gmAudioIconGenerator) << "Received reply from spotify:" << id;

    if (error != QNetworkReply::NoError) {
        qCCritical(gmAudioIconGenerator) << "Error reply" << id << error << data;
    }

    m_spotifyRequestList.removeOne(id);

    const auto doc = QJsonDocument::fromJson(data);

    if (doc.object().contains("followers"))
    {
        insertImageFromSpotifyPlaylist(doc.object());
    }
    else if (doc.object().contains("albums"))
    {
        for (auto album : doc.object()["albums"].toArray())
        {
            insertImageFromSpotifyAlbum(album.toObject());
        }
    }
    else if (doc.object().contains("tracks"))
    {
        for (auto track : doc.object()["tracks"].toArray())
        {
            insertImageFromSpotifyTrack(track.toObject());
        }
    }
}

void IconWorker::generateCollageImage(AudioElement *element)
{
    auto icon = element->icon();

    if (!icon || icon->collageIcons().isEmpty()) return;

    // Create collage
    QPixmap  image(512, 512);
    QPainter painter;
    painter.begin(&image);
    painter.setBackgroundMode(Qt::OpaqueMode);

    QList<QPixmap> images;

    for (auto image : icon->collageIcons())
    {
        bool doesExist = false;

        for (auto image2 : images)
        {
            if (image.second.toImage() == image2.toImage())
            {
                doesExist = true;
                break;
            }
        }

        if (!doesExist) images.append(image.second);
    }

    for (int i = 0; i < images.count(); i++)
    {
        if (i > 3) break;

        QRectF target = getTargetRect(512, 512, images.count(), i);
        QRectF source = getSourceRect(images[i].rect(), images.count(), i);
        painter.drawPixmap(target, images[i], source);
    }
    painter.end();

    // Save image
    AudioIconGenerator::writeToCache(icon->imageId(), image);
    icon->update();
    qCDebug(gmAudioIconGenerator()) << "Generated collage.";
}

auto IconWorker::getTargetRect(int imageWidth, int imageHeight, int imageCount, int index)->QRectF
{
    if (imageCount > 3) imageCount = 4;

    qreal width  = imageCount == 4 ? imageWidth / 2 : imageWidth / imageCount;
    qreal height = imageCount == 4 ? imageHeight / 2 : imageHeight;
    qreal left   = imageCount == 4 ? width * ((index + 1) % 2) : width * index;
    qreal top    = imageCount == 4 ? (index > 1 ? height : 0) : 0;

    return { left, top, width, height };
}

auto IconWorker::getSourceRect(QRect imageRect, int imageCount, int index)->QRectF
{
    if (imageCount > 3) return imageRect;

    qreal width  = imageRect.width() / imageCount;
    qreal height = imageRect.height();
    qreal left   = width * index;
    qreal top    = 0;

    return { left, top, width, height };
}

void IconWorker::onSpotifyAuthorized()
{
    getImagesFromSpotify();
}

AudioIconGenerator::AudioIconGenerator()
{
    auto *worker = new IconWorker(
        SettingsManager::getPath("resources"),
        SettingsManager::getPath("music"),
        SettingsManager::getPath("sounds")
        );

    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished,                      worker, &QObject::deleteLater);
    connect(this,          &AudioIconGenerator::startGeneratingAll, worker, &IconWorker::generateThumbnails);
    connect(this,          &AudioIconGenerator::startGeneratingOne, worker, &IconWorker::generateThumbnail);

    workerThread.start();
}

auto AudioIconGenerator::getInstance()->AudioIconGenerator *
{
    if (!instanceFlag)
    {
        single       = new AudioIconGenerator;
        instanceFlag = true;
    }
    return single;
}

AudioIconGenerator::~AudioIconGenerator()
{
    workerThread.quit();
    workerThread.wait();
    instanceFlag = false;
}

void AudioIconGenerator::generateIcons(AudioScenario *scenario)
{
    getInstance()->_generateIcons(scenario);
}

void AudioIconGenerator::_generateIcons(AudioScenario *scenario)
{
    emit startGeneratingAll(scenario);
}

void AudioIconGenerator::generateIcon(AudioElement *element)
{
    getInstance()->_generateIcon(element);
}

void AudioIconGenerator::writeToCache(QUrl url, QPixmap pixmap)
{
    if (cacheContains(url))
    {
        cacheLock.lockForWrite();
        iconCache[url] = pixmap;
        cacheLock.unlock();
    }
    else
    {
        cacheLock.lockForWrite();
        iconCache.insert(url, pixmap);
        cacheLock.unlock();
    }
}

QPixmap AudioIconGenerator::readFromCache(QUrl url)
{
    cacheLock.lockForRead();
    auto pixmap = iconCache[url];
    cacheLock.unlock();
    return pixmap;
}

bool AudioIconGenerator::cacheContains(QUrl url)
{
    cacheLock.lockForRead();
    auto contains = iconCache.contains(url);
    cacheLock.unlock();
    return contains;
}

bool AudioIconGenerator::tryLoadFromCache(QUrl url, AudioElement *element)
{
    if (cacheContains(url))
    {
        writeToCache(element->icon()->imageId(), AudioIconGenerator::readFromCache(url));
        element->icon()->update();
        return true;
    }

    return false;
}

void AudioIconGenerator::_generateIcon(AudioElement *element)
{
    emit startGeneratingOne(element);
}
