#pragma once

#include <QFuture>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>

#include "../data/spotifytrack.h"

class Spotify;

class TracksAPI : public QObject
{
    Q_OBJECT
    friend Spotify;

public:
    [[nodiscard]] auto getTrack(const QString &id) -> QFuture<QSharedPointer<SpotifyTrack>>;

    [[nodiscard]] auto getTracks(const QStringList &ids) const -> QFuture<std::vector<QSharedPointer<SpotifyTrack>>>;

private:
    explicit TracksAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    [[nodiscard]] auto getTracks(const QStringList &ids, std::vector<QSharedPointer<SpotifyTrack>> &&previous) const
        -> QFuture<std::vector<QSharedPointer<SpotifyTrack>>>;
    [[nodiscard]] static auto getNextBatch(const QStringList &ids,
                                           const std::vector<QSharedPointer<SpotifyTrack>> &previous) -> QStringList;
};
