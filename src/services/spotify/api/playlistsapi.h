#pragma once

#include "../data/spotifyplaylist.h"
#include "../data/spotifytracklist.h"
#include "rest/restnetworkreply.h"
#include <QFuture>
#include <QObject>
#include <QPointer>
#include <gsl/gsl>

class Spotify;

struct PlaylistConfig
{
    QString id;
    QString name;
    QString description;
    bool isPublic;
    bool isCollaborative;
};

class PlaylistsAPI : public QObject
{
    Q_OBJECT
    friend Spotify;

public:
    [[nodiscard]] auto getPlaylist(const QString &id) -> QFuture<QSharedPointer<SpotifyPlaylist>>;

    [[nodiscard]] auto getPlaylistTracks(const QString &id) -> QFuture<QSharedPointer<SpotifyTrackList>>;

    [[nodiscard]] auto getPlaylistTracks(QSharedPointer<SpotifyTrackList> tracks)
        -> QFuture<QSharedPointer<SpotifyTrackList>>;

    [[nodiscard]] auto updatePlaylist(const PlaylistConfig &config) const -> QFuture<gsl::owner<RestNetworkReply *>>;

private:
    explicit PlaylistsAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    static constexpr auto MAX_TRACK_COUNT = 50;
};
