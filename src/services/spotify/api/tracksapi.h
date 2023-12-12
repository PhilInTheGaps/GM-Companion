#pragma once

#include "../data/spotifytrack.h"
#include <QFuture>
#include <QObject>
#include <QPointer>

namespace Services
{

class Spotify;

class TracksAPI
{
    friend Spotify;

public:
    [[nodiscard]] auto getTrack(const QString &id, bool lowPriority) -> QFuture<SpotifyTrack>;

    [[nodiscard]] auto getTracks(const QStringList &ids, bool lowPriority) -> QFuture<std::vector<SpotifyTrack>>;

private:
    explicit TracksAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    [[nodiscard]] auto getTracks(const QStringList &ids, std::vector<SpotifyTrack> &&previous, bool lowPriority)
        -> QFuture<std::vector<SpotifyTrack>>;
    [[nodiscard]] static auto getNextBatch(const QStringList &ids, const std::vector<SpotifyTrack> &previous)
        -> QStringList;
};

} // namespace Services
