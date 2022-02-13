#include "spotifyplaybackstate.h"

auto SpotifyPlaybackState::fromJson(const QJsonDocument &json) -> QSharedPointer<SpotifyPlaybackState>
{
    return fromJson(json.object());
}

auto SpotifyPlaybackState::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyPlaybackState>
{
    auto *state = new SpotifyPlaybackState();

    state->device = SpotifyDevice::fromJson(json[QStringLiteral("device")].toObject());
    state->repeatState = repeatStateFromString(json[QStringLiteral("repeat_state")].toString());
    state->shuffleState = shuffleStateFromString(json[QStringLiteral("shuffle_state")].toString());
    state->context = SpotifyContext::fromJson(json[QStringLiteral("context")].toObject());
    state->timestamp = QDateTime::fromMSecsSinceEpoch(json[QStringLiteral("timestamp_ms")].toInt());
    state->progressMs = json[QStringLiteral("progress_ms")].toInt();
    state->isPlaying = json[QStringLiteral("is_playling")].toBool();

    return QSharedPointer<SpotifyPlaybackState>(state);
}

auto SpotifyPlaybackState::repeatStateFromString(const QString &str) -> SpotifyPlaybackState::RepeatState
{
    if (str == QStringLiteral("track")) return RepeatState::Track;

    if (str == QStringLiteral("context")) return RepeatState::Context;

    return RepeatState::Off;
}

auto SpotifyPlaybackState::shuffleStateFromString(const QString &str) -> SpotifyPlaybackState::ShuffleState
{
    if (str == QStringLiteral("on")) return ShuffleState::On;

    return ShuffleState::Off;
}

auto SpotifyContext::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyContext>
{
    auto *context = new SpotifyContext();

    if (json.isEmpty())
    {
        context->isNull = true;
    }
    else
    {
        context->type = SpotifyUtils::typeFromString(json[QStringLiteral("type")].toString());
        context->href = json[QStringLiteral("href")].toString();
        context->uri = json[QStringLiteral("uri")].toString();
    }

    return QSharedPointer<SpotifyContext>(context);
}
