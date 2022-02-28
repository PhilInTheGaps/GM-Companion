#pragma once

#include <QObject>
#include <QFuture>
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

    [[nodiscard]] auto getTracks(const QStringList &ids) const -> QFuture<QVector<QSharedPointer<SpotifyTrack>>>;

private:
    explicit TracksAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    static constexpr auto MAX_TRACK_COUNT = 50;

    [[nodiscard]] auto getTracks(const QStringList &ids, QVector<QSharedPointer<SpotifyTrack>> &&previous) const -> QFuture<QVector<QSharedPointer<SpotifyTrack>>>;
    [[nodiscard]] static auto getNextBatch(const QStringList &ids, const QVector<QSharedPointer<SpotifyTrack>> &previous) -> QStringList;
};

