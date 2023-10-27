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
    [[nodiscard]] auto getAlbum(const QString &id) -> QFuture<SpotifyAlbum>;

    [[nodiscard]] auto getAlbumTracks(const QString &id) -> QFuture<SpotifyTrackList>;
    [[nodiscard]] auto getAlbumTracks(SpotifyAlbum &&album) -> QFuture<SpotifyAlbum>;
    [[nodiscard]] auto getAlbumTracks(SpotifyTrackList &&tracklist) -> QFuture<SpotifyTrackList>;

private:
    explicit AlbumAPI(Spotify *parent);
    QPointer<Spotify> m_spotify = nullptr;
};

} // namespace Services
