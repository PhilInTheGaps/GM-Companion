#include "albumapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QLoggingCategory>
#include <QJsonDocument>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyAlbums, "gm.services.spotify.api.albums")

using namespace AsyncFuture;

AlbumAPI::AlbumAPI(Spotify *parent) : QObject(parent), m_spotify(parent) {}

auto AlbumAPI::getAlbum(const QString &id) -> QFuture<QSharedPointer<SpotifyAlbum>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyAlbums()) << "getAlbum(): id must not be empty!";
        return {};
    }

    QUrl url(QStringLiteral("https://api.spotify.com/v1/albums/%1").arg(id));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("market"), QStringLiteral("from_token"));
    url.setQuery(query);

    const auto callback = [this](RestNetworkReply *reply) {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << "getAlbum():" << reply->errorText();
            reply->deleteLater();
            return QFuture<QSharedPointer<SpotifyAlbum>>();
        }

        const auto data = reply->data();
        reply->deleteLater();

        auto album = SpotifyAlbum::fromJson(data);

        if (!album->tracks->next.isEmpty())
        {
            return getAlbumTracks(album);
        }

        return completed(album);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto AlbumAPI::getAlbumTracks(const QString &id) const -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyAlbums()) << "getAlbumTracks(): id must not be empty!";
        return {};
    }

    QUrl url(QStringLiteral("https://api.spotify.com/v1/albums/%1/tracks").arg(id));

    QUrlQuery query;
    query.addQueryItem("limit", QString::number(MAX_TRACK_COUNT));
    query.addQueryItem(QStringLiteral("market"), QStringLiteral("from_token"));
    url.setQuery(query);

    const auto callback = [this](gsl::owner<RestNetworkReply*> reply) -> QFuture<QSharedPointer<SpotifyTrackList>> {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply->errorText();
            reply->deleteLater();
            return {};
        }

        const auto data = reply->data();
        reply->deleteLater();

        auto tracklist = SpotifyTrackList::fromJson(data);
        if (!tracklist->next.isEmpty())
        {
            return getAlbumTracks(tracklist);
        }
        return completed(tracklist);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url)))
            .subscribe(callback).future();
}

auto AlbumAPI::getAlbumTracks(QSharedPointer<SpotifyAlbum> album) const -> QFuture<QSharedPointer<SpotifyAlbum>>
{
    if (album->tracks->next.isEmpty()) return completed(album);

    const QUrl url(album->tracks->next);

    const auto callback = [this, album](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply->errorText();
            reply->deleteLater();
            return completed(album);
        }

        const auto data = reply->data();
        reply->deleteLater();

        album->tracks->append(*SpotifyTrackList::fromJson(data));
        if (!album->tracks->next.isEmpty())
        {
            return getAlbumTracks(album);
        }
        return completed(album);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url)))
            .subscribe(callback).future();
}

auto AlbumAPI::getAlbumTracks(QSharedPointer<SpotifyTrackList> tracklist) const -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    const QUrl url(tracklist->next);

    const auto callback = [this, tracklist](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply->errorText();
            reply->deleteLater();
            return completed(tracklist);
        }

        const auto data = reply->data();
        reply->deleteLater();

        tracklist->append(*SpotifyTrackList::fromJson(data));
        if (!tracklist->next.isEmpty())
        {
            return getAlbumTracks(std::move(tracklist));
        }
        return completed(tracklist);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url)))
            .subscribe(callback).future();
}
