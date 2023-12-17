#include "albumapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyAlbums, "gm.services.spotify.api.albums")

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto MAX_ALBUM_TRACK_COUNT = 50;

AlbumAPI::AlbumAPI(Spotify *parent) : m_spotify(parent)
{
}

auto AlbumAPI::getAlbum(const QString &id, Options options) -> QFuture<SpotifyAlbum>
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

    const auto callback = [this, options](const RestReply &reply) {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyAlbums()) << "getAlbum():" << reply.errorText();
            return QFuture<SpotifyAlbum>();
        }

        auto album = SpotifyAlbum::fromJson(reply.data());

        if (!album.tracks.next.isEmpty())
        {
            return getAlbumTracks(std::move(album), options);
        }

        return QtFuture::makeReadyFuture(album);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Services::Option::Authenticated | options)
        .then(callback)
        .unwrap();
}

auto AlbumAPI::getAlbumTracks(const QString &id, Options options) -> QFuture<SpotifyTrackList>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyAlbums()) << "getAlbumTracks(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/albums/%1/tracks"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"limit"_s, QString::number(MAX_ALBUM_TRACK_COUNT));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    auto callback = [this, options](const RestReply &reply) -> QFuture<SpotifyTrackList> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply.errorText();
            return {};
        }

        auto tracklist = SpotifyTrackList::fromJson(reply.data());
        if (!tracklist.next.isEmpty())
        {
            return getAlbumTracks(std::move(tracklist), options);
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Services::Option::Authenticated | options)
        .then(callback)
        .unwrap();
}

auto AlbumAPI::getAlbumTracks(SpotifyAlbum &&album, Options options) -> QFuture<SpotifyAlbum>
{
    if (album.tracks.next.isEmpty()) return QtFuture::makeReadyFuture(album);

    const QUrl url(album.tracks.next);

    const auto callback = [this, options, album = std::move(album)](const RestReply &reply) mutable {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply.errorText();
            return QtFuture::makeReadyFuture(album);
        }

        album.tracks.append(SpotifyTrackList::fromJson(reply.data()));
        if (!album.tracks.next.isEmpty())
        {
            return getAlbumTracks(std::move(album), options);
        }
        return QtFuture::makeReadyFuture(album);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Services::Option::Authenticated | options)
        .then(callback)
        .unwrap();
}

auto AlbumAPI::getAlbumTracks(SpotifyTrackList &&tracklist, Options options) -> QFuture<SpotifyTrackList>
{
    const QUrl url(tracklist.next);

    const auto callback = [this, tracklist = std::move(tracklist), options](const RestReply &reply) mutable {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyAlbums()) << reply.errorText();
            return QtFuture::makeReadyFuture(tracklist);
        }

        tracklist.append(SpotifyTrackList::fromJson(reply.data()));
        if (!tracklist.next.isEmpty())
        {
            return getAlbumTracks(std::move(tracklist), options);
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Services::Option::Authenticated | options)
        .then(callback)
        .unwrap();
}
