#include "playlistsapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyPlaylists, "gm.spotify.api.playlists")

using namespace Qt::Literals::StringLiterals;

PlaylistsAPI::PlaylistsAPI(Spotify *parent) : QObject(parent), m_spotify(parent)
{
}

auto PlaylistsAPI::getPlaylist(const QString &id) -> QFuture<QSharedPointer<SpotifyPlaylist>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyPlaylists()) << "getPlaylist(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/playlists/%1"_s.arg(id));
    QUrlQuery query;
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [](gsl::owner<RestNetworkReply *> reply) -> QSharedPointer<SpotifyPlaylist> {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply->errorText();
            reply->deleteLater();
            return {};
        }

        const auto data = reply->data();
        reply->deleteLater();

        return SpotifyPlaylist::fromJson(data);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback);
}

auto PlaylistsAPI::getPlaylistTracks(const QString &id) -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyPlaylists()) << "getPlaylistTracks(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/playlists/%1/tracks"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"limit"_s, QString::number(MAX_TRACK_COUNT));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [this](gsl::owner<RestNetworkReply *> reply) -> QFuture<QSharedPointer<SpotifyTrackList>> {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply->errorText();
            reply->deleteLater();
            return {};
        }

        const auto data = reply->data();
        reply->deleteLater();

        auto tracklist = SpotifyTrackList::fromJson(data);
        if (!tracklist->next.isEmpty())
        {
            return getPlaylistTracks(tracklist);
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}

auto PlaylistsAPI::getPlaylistTracks(QSharedPointer<SpotifyTrackList> tracklist)
    -> QFuture<QSharedPointer<SpotifyTrackList>>
{
    const QUrl url(tracklist->next);

    const auto callback = [this, tracklist](gsl::owner<RestNetworkReply *> reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply->errorText();
            reply->deleteLater();
            return QtFuture::makeReadyFuture(tracklist);
        }

        const auto data = reply->data();
        reply->deleteLater();

        tracklist->append(*SpotifyTrackList::fromJson(data));
        if (!tracklist->next.isEmpty())
        {
            return getPlaylistTracks(std::move(tracklist));
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(this, callback).unwrap();
}

auto PlaylistsAPI::updatePlaylist(const PlaylistConfig &config) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url(u"https://api.spotify.com/v1/playlists/%1"_s.arg(config.id));

    if (config.id.isEmpty())
    {
        qCCritical(gmSpotifyPlaylists()) << "getPlaylist(): id must not be empty!";
        return {};
    }

    QJsonObject body{{"name", config.name},
                     {"description", config.description},
                     {"public", config.isPublic},
                     {"collaborative", config.isCollaborative}};
    const QJsonDocument content(body);

    return m_spotify->put(NetworkUtils::makeJsonRequest(url), content.toJson(QJsonDocument::JsonFormat::Compact));
}
