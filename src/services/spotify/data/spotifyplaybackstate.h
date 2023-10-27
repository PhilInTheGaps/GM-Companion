#pragma once

#include "spotify/spotifyutils.h"
#include "spotifydevice.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <chrono>

namespace Services
{

struct SpotifyContext
{
    static auto fromJson(const QJsonObject &json) -> SpotifyContext;

    /// The object type, e.g. "artist", "playlist", "album", "show"
    SpotifyUtils::SpotifyType type;

    /// A link to the Web API endpoint providing full details of the track
    QString href;

    /// The Spotify URI for the context
    QString uri;

    /// Whether the context is null
    bool isNull = false;
};

struct SpotifyPlaybackState
{
    static auto fromJson(const QJsonDocument &json) -> SpotifyPlaybackState;
    static auto fromJson(const QJsonObject &json) -> SpotifyPlaybackState;

    enum class RepeatState
    {
        Off,
        Track,
        Context
    };

    enum class ShuffleState
    {
        On,
        Off
    };

    RepeatState repeatState = RepeatState::Off;
    ShuffleState shuffleState = ShuffleState::Off;

    SpotifyContext context;

    /// The device that is currently active.
    SpotifyDevice device;

    /// Timestamp when data was fetched
    QDateTime timestamp;

    /// Progress into the currently playing track or episode
    std::chrono::milliseconds progress;

    /// Is true if something is currently playing
    bool isPlaying;

private:
    static auto repeatStateFromString(const QString &str) -> RepeatState;
    static auto shuffleStateFromString(const QString &str) -> ShuffleState;
};

} // namespace Services
