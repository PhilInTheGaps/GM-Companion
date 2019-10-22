#include "audioicongenerator.h"

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

#define MAX_SPOTIFY_REQUESTS 3

using namespace TagLib;

QMap<QUrl, QImage> IconWorker::iconCache;

IconWorker::IconWorker(QList<AudioProject *>projects) : m_projects(projects)
{
    connect(Spotify::getInstance(), &Spotify::authorized, this, &IconWorker::onSpotifyAuthorized);
}

IconWorker::~IconWorker()
{
    if (m_networkManager) m_networkManager->deleteLater();
}

void IconWorker::generate()
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
                generateCollage(element);
            }
        }
    }
}

void IconWorker::generateCollage(AudioElement *element, int index)
{
    if (!element) return;

    if (element->type() < 2)
    {
        if (index >= element->files().length())
        {
            generateCollageImage(element);
            return;
        }

        auto file = element->files()[index];

        if (file.source() == 2)
        {
            fetchSpotifyIcon(element, index);
        }
        else if (file.source() != 0)
        {
            generateCollage(element, index + 1);
        }
        else
        {
            QString path = element->type() == 0 ? sManager.getSetting(Setting::musicPath) + file.url() : sManager.getSetting(Setting::soundPath) + file.url();

            // Get id3v2 meta data tag
            FileRef f(path.toLocal8Bit());

            if (TagLib::MPEG::File *file = dynamic_cast<TagLib::MPEG::File *>(f.file()))
            {
                if (file->ID3v2Tag() && !file->ID3v2Tag()->isEmpty())
                {
                    auto tag = file->ID3v2Tag();

                    if (!tag)
                    {
                        generateCollage(element, index + 1);
                        return;
                    }

                    // Get frames from tag for image
                    auto frames = tag->frameList("APIC");

                    if (frames.isEmpty())
                    {
                        generateCollage(element, index + 1);
                        return;
                    }

                    // Convert image to QImage
                    auto   frame = static_cast<ID3v2::AttachedPictureFrame *>(frames.front());
                    QImage coverImage;
                    coverImage.loadFromData(QByteArray(frame->picture().data(), static_cast<int>(frame->picture().size())));

                    // Generate collage
                    if (element->icon()->addCollageIcon(coverImage) < 4)
                    {
                        generateCollage(element, index + 1);
                    }
                    else
                    {
                        generateCollageImage(element);
                    }
                }
            }
        }
    }
}

void IconWorker::fetchSpotifyIcon(AudioElement *element, int index)
{
    if (!element) return;

    element->icon()->setLastFileIndex(index);

    if (!Spotify::getInstance()->isGranted() || Spotify::getInstance()->token().isEmpty() || (m_currentSpotifyRequests >= MAX_SPOTIFY_REQUESTS))
    {
        m_queue.enqueue(element);
        return;
    }

    m_currentSpotifyRequests++;

    QUrl url;
    auto file = element->files()[index];
    auto id   = file.url();

    // Element is album
    if (id.contains("album:"))
    {
        url = QUrl("https://api.spotify.com/v1/albums/" + id.replace("spotify:album:", ""));
    }

    // Element is playlist
    else if (id.contains("playlist:"))
    {
        QString playlist = id.right(id.length() - id.lastIndexOf(':') - 1);

        url = QUrl("https://api.spotify.com/v1/playlists/" + playlist + "/images");
    }

    // Single track
    else if (id.contains("track:"))
    {
        QString track = id.right(id.length() - id.lastIndexOf(':') - 1);

        url = QUrl("https://api.spotify.com/v1/tracks/" + track);
    }

    QUrlQuery query(url);
    query.addQueryItem(O2_OAUTH2_ACCESS_TOKEN, Spotify::getInstance()->token());
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    auto reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [ = ]() { fetchedSpotifyIcon(reply, element); });
}

void IconWorker::startNextSpotifyRequest()
{
    if (m_queue.isEmpty()) return;

    auto element = m_queue.dequeue();

    fetchSpotifyIcon(element, element->icon()->lastFileIndex());
}

void IconWorker::fetchedSpotifyIcon(QNetworkReply *reply, AudioElement *element)
{
    if (!element) return;

    m_currentSpotifyRequests--;

    if (reply->error() != QNetworkReply::NoError)
    {
        m_queue.enqueue(element);

        qDebug() << "Fetched Spotify icon:" << element->name();
        qWarning() << "   Error:" << reply->error();
        return;
    }
    else
    {
        startNextSpotifyRequest();
    }

    const auto doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    QJsonObject image;

    // Array of Images
    if (doc.isArray())
    {
        image = doc.array()[0].toObject();
    }

    // Object containing an Array of Images
    else
    {
        if (doc.object().keys().contains("album"))
        {
            image = doc.object().value("album").toObject().value("images").toArray()[0].toObject();
        }
        else
        {
            image = doc.object().value("images").toArray()[0].toObject();
        }
    }

    // Get image data
    QUrl url(image.value("url").toString());

    // If image is in cache
    if (iconCache.contains(url))
    {
        if (element->icon()->addCollageIcon(iconCache[url]) < 4)
        {
            generateCollage(element, element->icon()->lastFileIndex() + 1);
        }
        else
        {
            generateCollageImage(element);
        }
        return;
    }

    // Otherwise load it from url
    QNetworkRequest request(url);
    auto reply2 = m_networkManager->get(request);

    connect(reply2, &QNetworkReply::finished, [ = ]()
    {
        auto data = reply2->readAll();
        reply2->deleteLater();

        QImage image;

        if (!image.loadFromData(data, "JPG"))
        {
            if (!image.loadFromData(data, "PNG"))
            {
                qDebug() << "Could not read image from data:" << element->name();
                generateCollage(element, element->icon()->lastFileIndex() + 1);
                return;
            }
        }

        iconCache[url] = image;

        if (element->icon()->addCollageIcon(image) < 4)
        {
            generateCollage(element, element->icon()->lastFileIndex() + 1);
        }
        else
        {
            generateCollageImage(element);
        }
    });
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

int IconWorker::addCollageIcon(AudioElement *element, QImage image)
{
    // Read cover image from meta data
    QByteArray bArray;
    QBuffer    buffer(&bArray);

    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");
    return element->icon()->addCollageIcon(image);
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
    while (m_currentSpotifyRequests < MAX_SPOTIFY_REQUESTS && !m_queue.isEmpty())
    {
        auto element = m_queue.dequeue();
        fetchSpotifyIcon(element, element->icon()->lastFileIndex());
    }
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
    connect(this,          &AudioIconGenerator::startGenerating, worker, &IconWorker::generate);

    workerThread.start();
    emit startGenerating();
}
