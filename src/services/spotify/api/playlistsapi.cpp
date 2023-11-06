#include "playlistsapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyPlaylists, "gm.spotify.api.playlists")

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto MAX_PLAYLIST_TRACK_COUNT = 50;

PlaylistsAPI::PlaylistsAPI(Spotify *parent) : m_spotify(parent)
{
}

auto PlaylistsAPI::getPlaylist(const QString &id) -> QFuture<SpotifyPlaylist>
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

    const auto callback = [](const RestReply &reply) -> QFuture<SpotifyPlaylist> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply.errorText();
            return {};
        }

        return QtFuture::makeReadyFuture(SpotifyPlaylist::fromJson(reply.data()));
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(callback).unwrap();
}

auto PlaylistsAPI::getPlaylistTracks(const QString &id) -> QFuture<SpotifyTrackList>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyPlaylists()) << "getPlaylistTracks(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/playlists/%1/tracks"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"limit"_s, QString::number(MAX_PLAYLIST_TRACK_COUNT));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [this](const RestReply &reply) -> QFuture<SpotifyTrackList> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply.errorText();
            return {};
        }

        auto tracklist = SpotifyTrackList::fromJson(reply.data());
        if (!tracklist.next.isEmpty())
        {
            return getPlaylistTracks(std::move(tracklist));
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(callback).unwrap();
}

auto PlaylistsAPI::getPlaylistTracks(SpotifyTrackList &&tracklist) -> QFuture<SpotifyTrackList>
{
    const QUrl url(tracklist.next);

    const auto callback = [this, tracklist = std::move(tracklist)](const RestReply &reply) mutable {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyPlaylists()) << reply.errorText();
            return QtFuture::makeReadyFuture(tracklist);
        }

        tracklist.append(SpotifyTrackList::fromJson(reply.data()));
        if (!tracklist.next.isEmpty())
        {
            return getPlaylistTracks(std::move(tracklist));
        }
        return QtFuture::makeReadyFuture(tracklist);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url)).then(callback).unwrap();
}

auto PlaylistsAPI::updatePlaylist(const PlaylistConfig &config) const -> QFuture<RestReply>
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
