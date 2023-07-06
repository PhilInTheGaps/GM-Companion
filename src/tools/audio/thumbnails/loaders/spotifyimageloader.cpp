#include "spotifyimageloader.h"
#include "../audiothumbnailcache.h"
#include "services/spotify/spotify.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QRandomGenerator>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmAudioSpotifyImageLoader, "gm.audio.thumbnails.loaders.spotify")

using namespace Qt::Literals::StringLiterals;
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

    if (type == SpotifyUtils::SpotifyType::Playlist)
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
    const auto callback = [id](QSharedPointer<SpotifyPlaylist> playlist) {
        if (playlist.isNull()) return QFuture<QPixmap>();

        const auto url = playlist->images.first()->url;
        const auto future = Spotify::instance()->get(url);

        const auto callback = [id, url](gsl::owner<RestNetworkReply *> reply) {
            QPixmap image;

            if (image.loadFromData(reply->data()))
            {
                AudioThumbnailCache::instance()->insertImage(url, image);
                AudioThumbnailCache::instance()->insertImage(id, image);
            }

            reply->deleteLater();
            return image;
        };

        return observe(future).subscribe(callback).future();
    };

    const auto future = Spotify::instance()->playlists->getPlaylist(id);
    return observe(future).subscribe(callback).future();
}

void SpotifyImageLoader::startRequest(SpotifyUtils::SpotifyType type)
{
    auto url = QUrl(getEndpoint(type));
    const auto ids = getBatchIds(type);

    QUrlQuery query;
    query.addQueryItem(u"ids"_s, ids.join(','));
    url.setQuery(query);

    qCDebug(gmAudioSpotifyImageLoader()) << "Sending batch request:" << url;

    const auto future = Spotify::instance()->get(url);
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
        const auto name = entry["name"_L1].toString();
        auto images = entry["images"_L1].toArray();

        qCDebug(gmAudioSpotifyImageLoader()) << "Received image url for id:" << id << name;

        if (images.isEmpty())
        {
            images = entry["album"_L1]["images"_L1].toArray();
        }

        const auto url = images.first()["url"_L1].toString();

        QPixmap image;
        if (AudioThumbnailCache::tryGet(url, &image))
        {
            m_pendingFutures.take(id).complete(image);
            continue;
        }

        const auto request = QNetworkRequest(url);
        const auto future = Spotify::instance()->get(request);

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
        auto *rng = QRandomGenerator::system();
        timer->start(DEFAULT_TIMEOUT_MS + rng->bounded(RANDOM_TIMEOUT_MS));
    }
}

void SpotifyImageLoader::initTimers()
{
    m_haveTimersBeenInitialized = true;

    initTimer(SpotifyUtils::SpotifyType::Album);
    initTimer(SpotifyUtils::SpotifyType::Track);
    initTimer(SpotifyUtils::SpotifyType::Artist);
    initTimer(SpotifyUtils::SpotifyType::Episode);
    initTimer(SpotifyUtils::SpotifyType::Show);
}

void SpotifyImageLoader::initTimer(SpotifyUtils::SpotifyType type)
{
    auto *timer = getTimer(type);
    if (!timer) return;

    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, [type]() { SpotifyImageLoader::startRequest(type); });
}

auto SpotifyImageLoader::getQueue(SpotifyUtils::SpotifyType type) -> QQueue<QString> *
{
    switch (type)
    {
    case SpotifyUtils::SpotifyType::Album:
        return &m_albumQueue;
    case SpotifyUtils::SpotifyType::Track:
        return &m_trackQueue;
    case SpotifyUtils::SpotifyType::Artist:
        return &m_artistQueue;
    case SpotifyUtils::SpotifyType::Episode:
        return &m_episodeQueue;
    case SpotifyUtils::SpotifyType::Show:
        return &m_showQueue;
    case SpotifyUtils::SpotifyType::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getQueue() should not have been called for a playlist image";
        break;
    default:
        qCWarning(gmAudioSpotifyImageLoader())
            << "The spotify type" << (int)type << "has not been implemented yet for batch image requests";
        break;
    }

    return nullptr;
}

auto SpotifyImageLoader::getCriticalQueueLength(SpotifyUtils::SpotifyType type) -> int
{
    switch (type)
    {
    case SpotifyUtils::SpotifyType::Album:
        return CRITICAL_ALBUM_QUEUE_LENGTH;
    case SpotifyUtils::SpotifyType::Track:
        return CRITICAL_TRACK_QUEUE_LENGTH;
    case SpotifyUtils::SpotifyType::Artist:
        return CRITICAL_ARTIST_QUEUE_LENGTH;
    case SpotifyUtils::SpotifyType::Episode:
        return CRITICAL_EPISODE_QUEUE_LENGTH;
    case SpotifyUtils::SpotifyType::Show:
        return CRITICAL_SHOW_QUEUE_LENGTH;
    case SpotifyUtils::SpotifyType::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getQueue() should not have been called for a playlist image";
        return 0;
    default:
        qCWarning(gmAudioSpotifyImageLoader())
            << "The spotify type" << (int)type << "has not been implemented yet for batch image requests";
        return 0;
    }
}

auto SpotifyImageLoader::getTimer(SpotifyUtils::SpotifyType type) -> QTimer *
{
    switch (type)
    {
    case SpotifyUtils::SpotifyType::Album:
        return &m_albumTimer;
    case SpotifyUtils::SpotifyType::Track:
        return &m_trackTimer;
    case SpotifyUtils::SpotifyType::Artist:
        return &m_artistTimer;
    case SpotifyUtils::SpotifyType::Episode:
        return &m_episodeTimer;
    case SpotifyUtils::SpotifyType::Show:
        return &m_showTimer;
    case SpotifyUtils::SpotifyType::Playlist:
        qCCritical(gmAudioSpotifyImageLoader()) << "getTimer() should not have been called for a playlist image";
        break;
    default:
        qCWarning(gmAudioSpotifyImageLoader())
            << "The spotify type" << (int)type << "has not been implemented yet for batch image requests";
        break;
    }

    return nullptr;
}

auto SpotifyImageLoader::getEndpoint(SpotifyUtils::SpotifyType type) -> QString
{
    switch (type)
    {
    case SpotifyUtils::SpotifyType::Album:
        return u"https://api.spotify.com/v1/albums"_s;
    case SpotifyUtils::SpotifyType::Track:
        return u"https://api.spotify.com/v1/tracks"_s;
    case SpotifyUtils::SpotifyType::Artist:
        return u"https://api.spotify.com/v1/artists"_s;
    case SpotifyUtils::SpotifyType::Episode:
        return u"https://api.spotify.com/v1/episodes"_s;
    case SpotifyUtils::SpotifyType::Show:
        return u"https://api.spotify.com/v1/shows"_s;
    case SpotifyUtils::SpotifyType::Playlist:
        return u"https://api.spotify.com/v1/playlists/%1/images"_s;
    default:
        qCWarning(gmAudioSpotifyImageLoader())
            << "The spotify type" << (int)type << "has not been implemented yet for batch image requests";
        return u""_s;
    }
}

auto SpotifyImageLoader::getResultArrayName(SpotifyUtils::SpotifyType type) -> QString
{
    switch (type)
    {
    case SpotifyUtils::SpotifyType::Album:
        return u"albums"_s;
    case SpotifyUtils::SpotifyType::Track:
        return u"tracks"_s;
    case SpotifyUtils::SpotifyType::Artist:
        return u"artists"_s;
    case SpotifyUtils::SpotifyType::Episode:
        return u"episodes"_s;
    case SpotifyUtils::SpotifyType::Show:
        return u"shows"_s;
    case SpotifyUtils::SpotifyType::Playlist:
        return u"playlists"_s;
    default:
        qCWarning(gmAudioSpotifyImageLoader())
            << "The spotify type" << (int)type << "has not been implemented yet for batch image requests";
        return u""_s;
    }
}
