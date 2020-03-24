#include "audioicongenerator.h"
#include "logging.h"
#include "services/services.h"

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>
#include <QDebug>
#include <QBuffer>
#include <QPainter>
#include <o0globals.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

using namespace TagLib;

QMap<QUrl, QImage> IconWorker::iconCache;

IconWorker::IconWorker(QList<AudioProject *>projects) : m_projects(projects)
{
    connect(Spotify::getInstance(), &Spotify::authorized,           this,                   &IconWorker::onSpotifyAuthorized);
    connect(Spotify::getInstance(), &Spotify::receivedReply,        this,                   &IconWorker::onReceivedSpotifyReply);
    connect(this,                   &IconWorker::getSpotifyRequest, Spotify::getInstance(), qOverload<QNetworkRequest, int>(&Spotify::get));

    loadPlaceholderImages();
}

IconWorker::~IconWorker()
{
    if (m_networkManager) m_networkManager->deleteLater();
}

void IconWorker::generateThumbnails()
{
    if (!m_networkManager) m_networkManager = new QNetworkAccessManager;

    for (auto project : m_projects)
    {
        if (!project) continue;

        for (auto element : project->elements())
        {
            if (!element) continue;

            if (element->icon()->background().isEmpty())
            {
                makeThumbnail(element);
            }
        }
    }

    getImagesFromSpotify();
}

QImage IconWorker::getPlaceholderImage(AudioElement *element)
{
    if (element)
    {
        switch (element->type())
        {
        case 0: return m_musicPlaceholderImage;

        case 1: return m_soundPlaceholderImage;

        default: break;
        }
    }

    QImage img;
    img.fill(0);
    return img;
}

void IconWorker::loadPlaceholderImages()
{
    m_musicPlaceholderImage = QImage(":/icons/media/music_image.png");
    m_soundPlaceholderImage = QImage(":/icons/media/sound_image.png");
}

void IconWorker::makeThumbnail(AudioElement *element)
{
    // Is element null or radio?
    if (!element || (element->type() == 2)) return;

    // Select files for collage
    auto fileCount = 0;

    for (auto file : element->files())
    {
        // Skip all files that are not local (0) or from spotify (2)
        // TODO 1: from url
        // TODO 3: from youtube
        if ((file->source() == 0) || (file->source() == 2))
        {
            if (element->icon()->addCollageIcon(getImageFromAudioFile(element, file)))
            {
                element->icon()->addCollageIconSource(file->url());

                if (++fileCount >= 4) break;
            }
        }
    }

    generateCollageImage(element);
}

QImage IconWorker::getImageFromAudioFile(AudioElement *element, AudioFile *audioFile)
{
    // Paranoid pointer check
    if (!element || !audioFile) return getPlaceholderImage(element);

    // Return placeholder image for spotify
    // Also add url to list for batch-retrieval later
    if (audioFile->source() == 2)
    {
        if (iconCache.contains(Spotify::getId(audioFile->url()))) {
            return iconCache[Spotify::getId(audioFile->url())];
        }
        m_spotifyIconList.append(audioFile->url());
        return getPlaceholderImage(element);
    }

    // Return empty placeholder image for non-local files
    if (audioFile->source() != 0) return getPlaceholderImage(element);

    // Resolve relative path
    QString path = element->type() == 0 ? sManager.getSetting(Setting::musicPath) + audioFile->url() : sManager.getSetting(Setting::soundPath) + audioFile->url();

    // Check if icon cache contains image already
    if (iconCache.contains(QUrl(path)))
    {
        return iconCache[path];
    }

    // Get id3v2 meta data tag
    FileRef f(path.toUtf8().data());

    if (TagLib::MPEG::File *file = dynamic_cast<TagLib::MPEG::File *>(f.file()))
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
            auto   frame = static_cast<ID3v2::AttachedPictureFrame *>(frames.front());
            QImage coverImage;
            coverImage.loadFromData(QByteArray(frame->picture().data(), static_cast<int>(frame->picture().size())));

            qCDebug(gmAudioIconGenerator) << "Successfully loaded image from audio file.";
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
        QString firstId  = Spotify::getId(firstUri);
        int     type     = Spotify::getUriType(firstUri);

        QStringList idBatch;
        idBatch.append(firstId);
        int index = 0;

        while (type != 1 && index < m_spotifyIconList.length() && SPOTIFY_BATCH_SIZE > idBatch.length())
        {
            if (Spotify::getUriType(m_spotifyIconList[index]) == type)
            {
                idBatch.append(Spotify::getId(m_spotifyIconList.takeAt(index)));
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

void IconWorker::insertImage(QImage image, QString uri)
{
    for (auto project : m_projects)
    {
        if (!project) continue;

        for (auto element : project->elements())
        {
            if (!element || !element->icon()) continue;

            for (int i = 0; i < element->icon()->collageIconSources().length(); i++)
            {
                if (element->icon()->collageIconSources()[i].contains(uri))
                {
                    element->icon()->setCollageIcon(image, i);
                    generateCollageImage(element);
                    break;
                }
            }
        }
    }
}

void IconWorker::insertImageFromUrl(QString imageUrl, QString uri)
{
    qCDebug(gmAudioIconGenerator) << "Inserting image" << imageUrl << uri;

    if (uri.isEmpty()) uri = imageUrl;

    if (iconCache.contains(imageUrl))
    {
        insertImage(iconCache[imageUrl], uri);
        return;
    }

    QNetworkRequest request(imageUrl);
    auto reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [ = ]()
    {
        auto data = reply->readAll();
        reply->deleteLater();

        QImage image = m_soundPlaceholderImage;

        if (!image.loadFromData(data, "JPG"))
        {
            if (!image.loadFromData(data, "PNG"))
            {
                qCDebug(gmAudioIconGenerator) << "Could not read image from data:" <<
                imageUrl;
                return;
            }
        }

        iconCache[Spotify::getId(uri)] = image;
        iconCache[imageUrl]            = image;
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

void IconWorker::onReceivedSpotifyReply(int id, QNetworkReply::NetworkError error, QByteArray data)
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
    QImage   image(512, 512, QImage::Format_RGB32);
    QPainter painter;
    painter.begin(&image);
    painter.setBackgroundMode(Qt::OpaqueMode);

    for (int i = 0; i < icon->collageIcons().count(); i++)
    {
        if (i > 3) break;

        QRectF target = getTargetRect(512, 512, icon->collageIcons().count(), i);
        QRectF source = getSourceRect(icon->collageIcons()[i].rect(), icon->collageIcons().count(), i);
        painter.drawImage(target, icon->collageIcons()[i], source);
    }
    painter.end();

    // Save image
    QByteArray bArray;
    QBuffer    buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");

    // Store collage as base64
    QString imageString("data:image/jpg;base64,");
    imageString.append(QString::fromLatin1(bArray.toBase64().data()));
    icon->setBackground(imageString);
}

QRectF IconWorker::getTargetRect(int imageWidth, int imageHeight, int imageCount, int index)
{
    if (imageCount > 3) imageCount = 4;

    int width  = imageCount == 4 ? imageWidth / 2 : imageWidth / imageCount;
    int height = imageCount == 4 ? imageHeight / 2 : imageHeight;
    int left   = imageCount == 4 ? width * ((index + 1) % 2) : width * index;
    int top    = imageCount == 4 ? (index > 1 ? height : 0) : 0;

    return QRectF(left, top, width, height);
}

QRectF IconWorker::getSourceRect(QRect imageRect, int imageCount, int index)
{
    if (imageCount > 3) return imageRect;

    int width  = imageRect.width() / imageCount;
    int height = imageRect.height();
    int left   = width * index;
    int top    = 0;

    return QRectF(left, top, width, height);
}

void IconWorker::onSpotifyAuthorized()
{
    getImagesFromSpotify();
}

AudioIconGenerator::~AudioIconGenerator()
{
    workerThread.quit();
    workerThread.wait();
}

void AudioIconGenerator::generateIcons(QList<AudioProject *>projects)
{
    IconWorker *worker = new IconWorker(projects);

    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished,                   worker, &QObject::deleteLater);
    connect(this,          &AudioIconGenerator::startGenerating, worker, &IconWorker::generateThumbnails);

    workerThread.start();
    emit startGenerating();
}
