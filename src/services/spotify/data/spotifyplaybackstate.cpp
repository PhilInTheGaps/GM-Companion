#include "spotifyplaybackstate.h"

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyPlaybackState::fromJson(const QJsonDocument &json) -> SpotifyPlaybackState
{
    return fromJson(json.object());
}

auto SpotifyPlaybackState::fromJson(const QJsonObject &json) -> SpotifyPlaybackState
{
    SpotifyPlaybackState state;

    state.device = SpotifyDevice::fromJson(json["device"_L1].toObject());
    state.repeatState = repeatStateFromString(json["repeat_state"_L1].toString());
    state.shuffleState = shuffleStateFromString(json["shuffle_state"_L1].toString());
    state.context = SpotifyContext::fromJson(json["context"_L1].toObject());
    state.timestamp = QDateTime::fromMSecsSinceEpoch(json["timestamp_ms"_L1].toInt());
    state.progress = std::chrono::milliseconds(json["progress_ms"_L1].toInt());
    state.isPlaying = json["is_playling"_L1].toBool();

    return state;
}

auto SpotifyPlaybackState::repeatStateFromString(const QString &str) -> SpotifyPlaybackState::RepeatState
{
    if (str == "track"_L1) return RepeatState::Track;

    if (str == "context"_L1) return RepeatState::Context;

    return RepeatState::Off;
}

auto SpotifyPlaybackState::shuffleStateFromString(const QString &str) -> SpotifyPlaybackState::ShuffleState
{
    if (str == "on"_L1) return ShuffleState::On;

    return ShuffleState::Off;
}

auto SpotifyContext::fromJson(const QJsonObject &json) -> SpotifyContext
{
    SpotifyContext context;

    if (json.isEmpty())
    {
        context.isNull = true;
    }
    else
    {
        context.type = SpotifyUtils::typeFromString(json["type"_L1].toString());
        context.href = json["href"_L1].toString();
        context.uri = json["uri"_L1].toString();
    }

    return context;
}
