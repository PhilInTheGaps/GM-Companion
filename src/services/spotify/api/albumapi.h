#pragma once

#include <QFuture>
#include <QObject>
#include <QPointer>

#include "../data/spotifyalbum.h"

class Spotify;

class AlbumAPI : public QObject
{
    Q_OBJECT
    friend Spotify;

public:
    [[nodiscard]] auto getAlbum(const QString &id) -> QFuture<QSharedPointer<SpotifyAlbum>>;

    [[nodiscard]] auto getAlbumTracks(const QString &id) -> QFuture<QSharedPointer<SpotifyTrackList>>;
    [[nodiscard]] auto getAlbumTracks(QSharedPointer<SpotifyAlbum> album) -> QFuture<QSharedPointer<SpotifyAlbum>>;
    [[nodiscard]] auto getAlbumTracks(QSharedPointer<SpotifyTrackList> tracklist)
        -> QFuture<QSharedPointer<SpotifyTrackList>>;

private:
    explicit AlbumAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    static constexpr auto MAX_TRACK_COUNT = 50;
};
