#include "spotifyimageloader.h"
#include "services/spotify/spotify.h"
#include "../audiothumbnailcache.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QRandomGenerator>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(gmAudioSpotifyImageLoader)
Q_LOGGING_CATEGORY(gmAudioSpotifyImageLoader, "gm.audio.thumbnails.loaders.spotify")

using namespace AsyncFuture;

auto SpotifyImageLoader::loadImageAsync(AudioFile *audioFile) -> QFuture<QPixmap>
{
    return loadImageAsync(audioFile->url());
}

auto SpotifyImageLoader::loadImageAsync(const QString &uri) -> QFuture<QPixmap>
{
    const auto type = SpotifyUtils::getUriType(uri);
    const auto id = SpotifyUtils::getIdFromUri(uri);

    // Try to get image from cache
    QPixmap image;
    if (AudioThumbnailCache::tryGet(id, &image))
    {
        return completed(image);
    }

    if (type == SpotifyUtils::Playlist)
    {
        return loadPlaylistImageAsync(id);
    }

    if (m_pendingFutures.contains(id))
    {
        return m_pendingFutures[id].future();
    }

    enqueueRequest(id, type);

    auto result = deferred<QPixmap>();
    m_pendingFutures[id] = result;

    startTimer(type);

    if (shouldTimerBeStopped(type))
    {
        stopTimerEarly(type);
    }

    return result.future();
}

void SpotifyImageLoader::enqueueRequest(const QString &id, SpotifyUtils::SpotifyType type)
{
    auto *queue = getQueue(type);

    if (queue)
    {
        queue->enqueue(id);
    }
}

auto SpotifyImageLoader::loadPlaylistImageAsync(const QString &id) -> QFuture<QPixmap>
{
    const auto url = getEndpoint(SpotifyUtils::Playlist).arg(id);

    const auto future = Spotify::getInstance()->get(url);

    const auto callback = [id](RestNetworkReply *reply) {
        if (reply->hasError())
        {
            qCWarning(gmAudioSpotifyImageLoader()) << reply->errorText();
            reply->deleteLater();
            return completed(QPixmap());
        }

        const auto json = QJsonDocument::fromJson(reply->data()).array();
        const auto image = json.first().toObject();
        const auto url = image["url"].toString();
        reply->deleteLater();

        const auto request = QNetworkRequest(url);
        const auto future = Spotify::getInstance()->get(request);

        const auto callback = [id, url](RestNetworkReply *reply) {
            QPixmap image;

            if (image.loadFromData(reply->data()))
            {
                AudioThumbnailCache::instance()->insertImage(url, image);
                AudioThumbnailCache::instance()->insertImage(id, image);
            }

            return image;
        };

        return observe(future).subscribe(callback).future();
    };

    return observe(future).subscribe(callback).future();
}

void SpotifyImageLoader::startRequest(SpotifyUtils::SpotifyType type)
{
    auto url = QUrl(getEndpoint(type));
    const auto ids = getBatchIds(type);

    QUrlQuery query;
    query.addQueryItem("ids", ids.join(','));
    url.setQuery(query);

    qCDebug(gmAudioSpotifyImageLoader()) << "Sending batch request:" << url;

    const auto future = Spotify::getInstance()->get(url);
    observe(future).subscribe([type](RestNetworkReply *reply) { receivedRequest(reply, type); });

    // Start timer again if there still are ids in the queue
    if (!getQueue(type)->isEmpty())
    {
        startTimer(type);
    }
}

void SpotifyImageLoader::receivedRequest(RestNetworkReply *reply, SpotifyUtils::SpotifyType type)
{
    if (reply->hasError())
    {
        qCWarning(gmAudioSpotifyImageLoader()) << reply->errorText();
        reply->deleteLater();
        return;
    }

    const auto json = QJsonDocument::fromJson(reply->data()).object();
    reply->deleteLater();

    const auto entries = json[getResultArrayName(type)].toArray();

    for (const auto &value : entries)
    {
        const auto entry = value.toObject();
        const auto id = entry["id"].toString();
        const auto name = entry["name"].toString();
        auto images = entry["images"].toArray();

        qCDebug(gmAudioSpotifyImageLoader()) << "Received image url for id:" << id << name;

        if (images.isEmpty())
        {
            images = entry["album"].toObject()["images"].toArray();
        }

        const auto url = images.first().toObject()["url"].toString();

        QPixmap image;
        if (AudioThumbnailCache::tryGet(url, &image))
        {
            m_pendingFutures.take(id).complete(image);
            continue;
        }

        const auto request = QNetworkRequest(url);
        const auto future = Spotify::getInstance()->get(request);

        const auto callback = [id, url](RestNetworkReply *reply) {
            QPixmap image;
            if (image.loadFromData(reply->data()))
            {
                AudioThumbnailCache::instance()->insertImage(url, image);
                AudioThumbnailCache::instance()->insertImage(id, image);
            }

            m_pendingFutures.take(id).complete(image);
        };

        observe(future).subscribe(callback);
    }
}

auto SpotifyImageLoader::getBatchIds(SpotifyUtils::SpotifyType type) -> QStringList
{
    QStringList ids;

    auto *queue = getQueue(type);
    const auto maxIdCount = getCriticalQueueLength(type);

    while (!queue->isEmpty() && ids.length() < maxIdCount)
    {
        ids.append(queue->dequeue());
    }

    ids.removeDuplicates();

    return ids;
}

auto SpotifyImageLoader::shouldTimerBeStopped(SpotifyUtils::SpotifyType type) -> bool
{
    const auto *queue = getQueue(type);

    if (queue)
    {
        return queue->length() >= getCriticalQueueLength(type);
    }

    return false;
}

void SpotifyImageLoader::stopTimerEarly(SpotifyUtils::SpotifyType type)
{
    auto *timer = getTimer(type);
    if (timer)
    {
        timer->stop();
        startRequest(type);
    }
}

void SpotifyImageLoader::startTimer(SpotifyUtils::SpotifyType type)
{
    if (!m_haveTimersBeenInitialized) initTimers();

    auto *timer = getTimer(type);

    if (timer && !timer->isActive())
    {
        auto rng = QRandomGenerator();
        timer->start(DEFAULT_TIMEOUT_MS + rng.bounded(RANDOM_TIMEOUT_MS));
    }
}

void SpotifyImageLoader::initTimers()
{
    m_haveTimersBeenInitialized = true;

    initTimer(SpotifyUtils::Album);
    initTimer(SpotifyUtils::Track);
    initTimer(SpotifyUtils::Artist);
    initTimer(SpotifyUtils::Episode);
    initTimer(SpotifyUtils::Show);
}

void SpotifyImageLoader::initTimer(SpotifyUtils::SpotifyType type)
{
    auto *timer = getTimer(type);
    if (!timer) return;

    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, [type](){ SpotifyImageLoader::startRequest(type); });
}

auto SpotifyImageLoader::getQueue(SpotifyUtils::SpotifyType type) -> QQueue<QString>*
{
    switch (type)
    {
    case SpotifyUtils::Album:
        return &m_albumQueue;
    case SpotifyUtils::Track:
        return &m_trackQueue;
    case SpotifyUtils::Artist:
        return &m_artistQueue;
    case SpotifyUtils::Episode:
        return &m_episodeQueue;
    case SpotifyUtils::Show:
        return &m_showQueue;
    case SpotifyUtils::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getQueue() should not have been called for a playlist image";
        break;
    default:
        qCWarning(gmAudioSpotifyImageLoader()) << "The spotify type" << type << "has not been implemented yet for batch image requests";
        break;
    }

    return nullptr;
}

auto SpotifyImageLoader::getCriticalQueueLength(SpotifyUtils::SpotifyType type) -> int
{
    switch (type)
    {
    case SpotifyUtils::Album:
        return CRITICAL_ALBUM_QUEUE_LENGTH;
    case SpotifyUtils::Track:
        return CRITICAL_TRACK_QUEUE_LENGTH;
    case SpotifyUtils::Artist:
        return CRITICAL_ARTIST_QUEUE_LENGTH;
    case SpotifyUtils::Episode:
        return CRITICAL_EPISODE_QUEUE_LENGTH;
    case SpotifyUtils::Show:
        return CRITICAL_SHOW_QUEUE_LENGTH;
    case SpotifyUtils::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getQueue() should not have been called for a playlist image";
        return 0;
    default:
        qCWarning(gmAudioSpotifyImageLoader()) << "The spotify type" << type << "has not been implemented yet for batch image requests";
        return 0;
    }
}

auto SpotifyImageLoader::getTimer(SpotifyUtils::SpotifyType type) -> QTimer*
{
    switch (type)
    {
    case SpotifyUtils::Album:
        return &m_albumTimer;
    case SpotifyUtils::Track:
        return &m_trackTimer;
    case SpotifyUtils::Artist:
        return &m_artistTimer;
    case SpotifyUtils::Episode:
        return &m_episodeTimer;
    case SpotifyUtils::Show:
        return &m_showTimer;
    case SpotifyUtils::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getTimer() should not have been called for a playlist image";
        break;
    default:
        qCWarning(gmAudioSpotifyImageLoader()) << "The spotify type" << type << "has not been implemented yet for batch image requests";
        break;
    }

    return nullptr;
}

auto SpotifyImageLoader::getEndpoint(SpotifyUtils::SpotifyType type) -> QString
{
    switch (type)
    {
    case SpotifyUtils::Album:
        return QStringLiteral("https://api.spotify.com/v1/albums");
    case SpotifyUtils::Track:
        return QStringLiteral("https://api.spotify.com/v1/tracks");
    case SpotifyUtils::Artist:
        return QStringLiteral("https://api.spotify.com/v1/artists");
    case SpotifyUtils::Episode:
        return QStringLiteral("https://api.spotify.com/v1/episodes");
    case SpotifyUtils::Show:
        return QStringLiteral("https://api.spotify.com/v1/shows");
    case SpotifyUtils::Playlist:
        return QStringLiteral("https://api.spotify.com/v1/playlists/%1/images");
    default:
        qCWarning(gmAudioSpotifyImageLoader()) << "The spotify type" << type << "has not been implemented yet for batch image requests";
        return QLatin1String();
    }
}

auto SpotifyImageLoader::getResultArrayName(SpotifyUtils::SpotifyType type) -> QString
{
    switch (type)
    {
    case SpotifyUtils::Album:
        return QStringLiteral("albums");
    case SpotifyUtils::Track:
        return QStringLiteral("tracks");
    case SpotifyUtils::Artist:
        return QStringLiteral("artists");
    case SpotifyUtils::Episode:
        return QStringLiteral("episodes");
    case SpotifyUtils::Show:
        return QStringLiteral("shows");
    case SpotifyUtils::Playlist:
        return QStringLiteral("playlists");
    default:
        qCWarning(gmAudioSpotifyImageLoader()) << "The spotify type" << type << "has not been implemented yet for batch image requests";
        return QLatin1String();
    }
}
