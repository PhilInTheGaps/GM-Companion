#pragma once

#include "../../options.h"
#include "../data/spotifyplaylist.h"
#include "../data/spotifytracklist.h"
#include "rest/restreply.h"
#include <QFuture>
#include <QObject>
#include <QPointer>

namespace Services
{

class Spotify;

struct PlaylistConfig
{
    QString id;
    QString name;
    QString description;
    bool isPublic;
    bool isCollaborative;
};

class PlaylistsAPI
{
    friend Spotify;

public:
    [[nodiscard]] auto getPlaylist(const QString &id, Options options) -> QFuture<SpotifyPlaylist>;

    [[nodiscard]] auto getPlaylistTracks(const QString &id, Options options) -> QFuture<SpotifyTrackList>;

    [[nodiscard]] auto getPlaylistTracks(SpotifyTrackList &&tracks, Options options) -> QFuture<SpotifyTrackList>;

    [[nodiscard]] auto updatePlaylist(const PlaylistConfig &config, Options options) const -> QFuture<RestReply>;

private:
    explicit PlaylistsAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;
};

} // namespace Services
