#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include "spotifydevice.h"
#include "spotify/spotifyutils.h"

struct SpotifyContext
{
    static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyContext>;

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
    static auto fromJson(const QJsonDocument &json) -> QSharedPointer<SpotifyPlaybackState>;
    static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyPlaybackState>;

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

    QSharedPointer<SpotifyContext> context;

    /// The device that is currently active.
    QSharedPointer<SpotifyDevice> device;

    /// Timestamp when data was fetched
    QDateTime timestamp;

    /// Progress into the currently playing track or episode
    int progressMs;

    /// Is true if something is currently playing
    bool isPlaying;

private:
    static auto repeatStateFromString(const QString &str) -> RepeatState;
    static auto shuffleStateFromString(const QString &str) -> ShuffleState;

};
