#pragma once

#include "../data/spotifycurrenttrack.h"
#include "../data/spotifyplaybackstate.h"
#include "rest/restnetworkreply.h"
#include <QDateTime>
#include <QFuture>
#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QStringList>
#include <gsl/gsl>

class Spotify;

class PlayerAPI : public QObject
{
    Q_OBJECT
    friend Spotify;

public:
    /// Resume playback
    [[nodiscard]] auto play() const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Start playback on specific device
    [[nodiscard]] auto play(const QString &deviceId, const QJsonObject &body) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Start playback of URI on current device
    [[nodiscard]] auto play(const QString &uri) const -> QFuture<gsl::owner<RestNetworkReply *>>;
    [[nodiscard]] auto play(const QJsonObject &body) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Pause playback on the user's account (current device)
    [[nodiscard]] auto pause() const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Pause playback on the user's account (specific device)
    [[nodiscard]] auto pause(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Skips to next track in the user’s queue (current device)
    [[nodiscard]] auto next() const -> QFuture<gsl::owner<RestNetworkReply *>>;
    [[nodiscard]] auto next(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Skips to previous track in the user’s queue (current device)
    [[nodiscard]] auto previous() const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Skips to previous track in the user’s queue (specific device)
    [[nodiscard]] auto previous(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Seeks to the given position in the user’s currently playing track (current device)
    [[nodiscard]] auto seek(int positionMs) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Seeks to the given position in the user’s currently playing track (specific device)
    [[nodiscard]] auto seek(int positionMs, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Get information about the user’s current playback state, including track or episode, progress, and active device
    [[nodiscard]] auto getState() -> QFuture<QSharedPointer<SpotifyPlaybackState>>;
    [[nodiscard]] auto getState(const QStringList &additionalTypes, const QString &market)
        -> QFuture<QSharedPointer<SpotifyPlaybackState>>;

    /// Get the object currently being played on the user's Spotify account
    [[nodiscard]] auto getCurrentlyPlaying() -> QFuture<QSharedPointer<SpotifyCurrentTrack>>;
    [[nodiscard]] auto getCurrentlyPlaying(const QStringList &additionalTypes, const QString &market)
        -> QFuture<QSharedPointer<SpotifyCurrentTrack>>;

    /// Transfer playback to a new device and keep current playback state
    [[nodiscard]] auto transfer(const QStringList &deviceIds) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Transfer playback to a new device and determine if it should start playing
    [[nodiscard]] auto transfer(const QStringList &deviceIds, bool play) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Get information about a user’s available devices
    [[nodiscard]] auto devices() -> QFuture<QSharedPointer<SpotifyDeviceList>>;

    enum SpotifyRepeatMode
    {
        /// Repeat current track
        Track,
        /// Repeat current context (album, etc)
        Context,
        /// Don't repeat
        Off
    };
    Q_ENUM(SpotifyRepeatMode)

    /// Set the repeat mode for the user's playback. Options are repeat-track, repeat-context, and off (current device)
    [[nodiscard]] auto repeat(SpotifyRepeatMode mode) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Set the repeat mode for the user's playback. Options are repeat-track, repeat-context, and off (specific device)
    [[nodiscard]] auto repeat(SpotifyRepeatMode mode, const QString &deviceId) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Set the repeat mode for the user's playback. Options are repeat-track, repeat-context, and off (specific device)
    [[nodiscard]] auto repeat(const QString &state, const QString &deviceId) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Set the volume for the user’s current playback device (current device)
    [[nodiscard]] auto volume(int volumePercent) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Set the volume for the user’s current playback device (specific device)
    [[nodiscard]] auto volume(int volumePercent, const QString &deviceId) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Toggle shuffle on or off for user’s playback (current device)
    [[nodiscard]] auto shuffle(bool state) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Toggle shuffle on or off for user’s playback (specific device)
    [[nodiscard]] auto shuffle(bool state, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Get tracks from the current user's recently played tracks.
    /// Note: "limit" will be clamped to between 0 and 50
    [[nodiscard]] auto getRecentlyPlayed(int limit) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Get tracks from the current user's recently played tracks
    /// Note: "limit" will be clamped to between 0 and 50
    [[nodiscard]] auto getRecentlyPlayed(const QDateTime &after, const QDateTime &before, int limit) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Add an item to the end of the user's current playback queue (current device)
    [[nodiscard]] auto addToQueue(const QString &uri) const -> QFuture<gsl::owner<RestNetworkReply *>>;

    /// Add an item to the end of the user's current playback queue (specific device)
    [[nodiscard]] auto addToQueue(const QString &uri, const QString &deviceId) const
        -> QFuture<gsl::owner<RestNetworkReply *>>;

private:
    explicit PlayerAPI(Spotify *parent);
    const QPointer<Spotify> m_spotify = nullptr;

    static auto makePlayBody(const QString &contextUri, const QStringList &uris, int position, int positionMs)
        -> QJsonObject;

    static constexpr auto MAX_RECENTLY_PLAYED = 50;
};
