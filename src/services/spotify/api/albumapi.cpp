#include "albumapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyAlbums, "gm.services.spotify.api.albums")

using namespace Qt::Literals::StringLiterals;

AlbumAPI::AlbumAPI(Spotify *parent) : QObject(parent), m_spotify(parent)
{
}

auto AlbumAPI::getAlbum(const QString &id) -> QFuture<QSharedPointer<SpotifyAlbum>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyAlbums()) << "getAlbum(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/albums/%1"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"market"_s, u"from_token"_s);
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

        return QtFuture::makeReadyFuture(album);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}

auto AlbumAPI::getAlbumTracks(const QString &id) -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyAlbums()) << "getAlbumTracks(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/albums/%1/tracks"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"limit"_s, QString::number(MAX_TRACK_COUNT));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    auto callback = [this](gsl::owner<RestNetworkReply *> reply) -> QFuture<QSharedPointer<SpotifyTrackList>> {
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
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}

auto AlbumAPI::getAlbumTracks(QSharedPointer<SpotifyAlbum> album) -> QFuture<QSharedPointer<SpotifyAlbum>>
{
    if (album->tracks->next.isEmpty()) return QtFuture::makeReadyFuture(album);

    const QUrl url(album->tracks->next);

    const auto callback = [this, album](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply->errorText();
            reply->deleteLater();
            return QtFuture::makeReadyFuture(album);
        }

        const auto data = reply->data();
        reply->deleteLater();

        album->tracks->append(*SpotifyTrackList::fromJson(data));
        if (!album->tracks->next.isEmpty())
        {
            return getAlbumTracks(album);
        }
        return QtFuture::makeReadyFuture(album);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}

auto AlbumAPI::getAlbumTracks(QSharedPointer<SpotifyTrackList> tracklist) -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    const QUrl url(tracklist->next);

    const auto callback = [this, tracklist](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply->errorText();
            reply->deleteLater();
            return QtFuture::makeReadyFuture(tracklist);
        }

        const auto data = reply->data();
        reply->deleteLater();

        tracklist->append(*SpotifyTrackList::fromJson(data));
        if (!tracklist->next.isEmpty())
        {
            return getAlbumTracks(std::move(tracklist));
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}
