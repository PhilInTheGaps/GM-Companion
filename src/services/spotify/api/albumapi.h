#pragma once

#include "../data/spotifyalbum.h"
#include <QFuture>
#include <QPointer>

namespace Services
{

class Spotify;

class AlbumAPI
{
    friend Spotify;

public:
    [[nodiscard]] auto getAlbum(const QString &id, bool lowPriority) -> QFuture<SpotifyAlbum>;

    [[nodiscard]] auto getAlbumTracks(const QString &id, bool lowPriority) -> QFuture<SpotifyTrackList>;
    [[nodiscard]] auto getAlbumTracks(SpotifyAlbum &&album, bool lowPriority) -> QFuture<SpotifyAlbum>;
    [[nodiscard]] auto getAlbumTracks(SpotifyTrackList &&tracklist, bool lowPriority) -> QFuture<SpotifyTrackList>;

private:
    explicit AlbumAPI(Spotify *parent);
    QPointer<Spotify> m_spotify = nullptr;
};

} // namespace Services
