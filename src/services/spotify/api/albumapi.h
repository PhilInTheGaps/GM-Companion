#pragma once

#include "../../options.h"
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
    [[nodiscard]] auto getAlbum(const QString &id, Options options) -> QFuture<SpotifyAlbum>;

    [[nodiscard]] auto getAlbumTracks(const QString &id, Options options) -> QFuture<SpotifyTrackList>;
    [[nodiscard]] auto getAlbumTracks(SpotifyAlbum &&album, Options options) -> QFuture<SpotifyAlbum>;
    [[nodiscard]] auto getAlbumTracks(SpotifyTrackList &&tracklist, Options options) -> QFuture<SpotifyTrackList>;

private:
    explicit AlbumAPI(Spotify *parent);
    QPointer<Spotify> m_spotify = nullptr;
};

} // namespace Services
