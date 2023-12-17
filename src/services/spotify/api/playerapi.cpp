#include "playerapi.h"
#include "spotify/spotify.h"
#include "spotify/spotifyutils.h"
#include "utils/networkutils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QUrlQuery>
#include <algorithm>

Q_LOGGING_CATEGORY(gmSpotifyPlayer, "gm.spotify.api.player")

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto MAX_RECENTLY_PLAYED = 50;

PlayerAPI::PlayerAPI(Spotify *parent) : m_spotify(parent)
{
}

auto PlayerAPI::play() const -> QFuture<RestReply>
{
    return m_spotify->put(QUrl(u"https://api.spotify.com/v1/me/player/play"_s), {}, Option::Authenticated);
}

auto PlayerAPI::play(const QString &deviceId, const QJsonObject &body) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/play"_s);

    if (!deviceId.isEmpty())
    {
        QUrlQuery query{{u"device_id"_s, deviceId}};
        url.setQuery(query);
    }

    const QJsonDocument content(body);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), content.toJson(QJsonDocument::JsonFormat::Compact),
                          Option::Authenticated);
}

auto PlayerAPI::play(const QString &uri) const -> QFuture<RestReply>
{
    if (const auto type = SpotifyUtils::getUriType(uri);
        type == SpotifyUtils::SpotifyType::Track || type == SpotifyUtils::SpotifyType::Episode)
    {
        return play(makePlayBody(u""_s, {uri}, -1, -1));
    }

    return play(makePlayBody(uri, {}, -1, -1));
}

auto PlayerAPI::play(const QJsonObject &body) const -> QFuture<RestReply>
{
    return play(u""_s, body);
}

auto PlayerAPI::makePlayBody(const QString &contextUri, const QStringList &uris, int position, int positionMs)
    -> QJsonObject
{
    QJsonObject body;

    if (!contextUri.isEmpty())
    {
        body["context_uri"_L1] = contextUri;
    }

    if (!uris.isEmpty())
    {
        body["uris"_L1] = QJsonArray::fromStringList(uris);
    }

    if (position > -1)
    {
        QJsonObject offset{{"position", position}};
        body["offset"_L1] = offset;
    }

    if (positionMs > -1)
    {
        body["position_ms"_L1] = positionMs;
    }

    return body;
}

auto PlayerAPI::pause() const -> QFuture<RestReply>
{
    return pause(u""_s);
}

auto PlayerAPI::pause(const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/pause"_s);

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{u"device_id"_s, deviceId}};
        url.setQuery(query);
    }

    return m_spotify->put(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::next() const -> QFuture<RestReply>
{
    return next(u""_s);
}

auto PlayerAPI::next(const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/next"_s);

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{u"device_id"_s, deviceId}};
        url.setQuery(query);
    }

    return m_spotify->post(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::previous() const -> QFuture<RestReply>
{
    return previous(u""_s);
}

auto PlayerAPI::previous(const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/previous"_s);

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{u"device_id"_s, deviceId}};
        url.setQuery(query);
    }

    return m_spotify->post(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::seek(int positionMs) const -> QFuture<RestReply>
{
    return seek(positionMs, u""_s);
}

auto PlayerAPI::seek(int positionMs, const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/seek"_s);
    QUrlQuery query = {{u"position_ms"_s, QString::number(positionMs)}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(u"device_id"_s, deviceId);
    }

    url.setQuery(query);

    return m_spotify->put(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::getState() -> QFuture<SpotifyPlaybackState>
{
    return getState({}, u""_s);
}

auto PlayerAPI::getState(const QStringList &additionalTypes, const QString &market) -> QFuture<SpotifyPlaybackState>
{
    QUrl url(u"https://api.spotify.com/v1/me/player"_s);
    QUrlQuery query;

    if (!additionalTypes.isEmpty())
    {
        query.addQueryItem(u"additional_types"_s, additionalTypes.join(','));
    }

    if (!market.isEmpty())
    {
        query.addQueryItem(u"market"_s, market);
    }

    url.setQuery(query);

    const auto callback = [](const RestReply &reply) -> QFuture<SpotifyPlaybackState> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyPlayer()) << reply.errorText();
            return {};
        }

        return QtFuture::makeReadyFuture(SpotifyPlaybackState::fromJson(QJsonDocument::fromJson(reply.data())));
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated).then(callback).unwrap();
}

auto PlayerAPI::getCurrentlyPlaying() -> QFuture<SpotifyCurrentTrack>
{
    return getCurrentlyPlaying({}, u""_s);
}

auto PlayerAPI::getCurrentlyPlaying(const QStringList &additionalTypes, const QString &market)
    -> QFuture<SpotifyCurrentTrack>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/currently-playing"_s);
    QUrlQuery query;

    if (!additionalTypes.isEmpty())
    {
        query.addQueryItem(u"additional_types"_s, additionalTypes.join(','));
    }

    if (!market.isEmpty())
    {
        query.addQueryItem(u"market"_s, market);
    }

    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [](const RestReply &reply) -> QFuture<SpotifyCurrentTrack> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyPlayer()) << reply.errorText();
            return {};
        }

        return QtFuture::makeReadyFuture(SpotifyCurrentTrack::fromJson(reply.data()));
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated).then(callback).unwrap();
}

auto PlayerAPI::transfer(const QStringList &deviceIds) const -> QFuture<RestReply>
{
    return transfer(deviceIds, false);
}

auto PlayerAPI::transfer(const QStringList &deviceIds, bool play) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player"_s);

    const QJsonObject body{{u"device_ids"_s, QJsonArray::fromStringList(deviceIds)}, {u"play"_s, play}};

    const auto doc = QJsonDocument(body);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), doc.toJson(QJsonDocument::Compact),
                          Option::Authenticated);
}

auto PlayerAPI::devices() -> QFuture<SpotifyDeviceList>
{
    const QUrl url(u"https://api.spotify.com/v1/me/player/devices"_s);

    const auto callback = [](const RestReply &reply) {
        const auto json = QJsonDocument::fromJson(reply.data()).object();
        const auto devices = json["devices"_L1].toArray();
        return SpotifyDevice::fromJson(devices);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated).then(callback);
}

auto PlayerAPI::repeat(SpotifyRepeatMode mode) const -> QFuture<RestReply>
{
    return repeat(mode, u""_s);
}

auto PlayerAPI::repeat(SpotifyRepeatMode mode, const QString &deviceId) const -> QFuture<RestReply>
{
    switch (mode)
    {
    case SpotifyRepeatMode::Track:
        return repeat(u"track"_s, deviceId);
    case SpotifyRepeatMode::Context:
        return repeat(u"context"_s, deviceId);
    case SpotifyRepeatMode::Off:
        return repeat(u"off"_s, deviceId);
    default:
        qCCritical(gmSpotifyPlayer()) << "repeat(): unknown repeat mode" << static_cast<int>(mode);
        return {};
    }
}

auto PlayerAPI::repeat(const QString &state, const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/repeat"_s);
    QUrlQuery query = {{u"state"_s, state}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(u"device_id"_s, deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::volume(int volumePercent) const -> QFuture<RestReply>
{
    return volume(volumePercent, u""_s);
}

auto PlayerAPI::volume(int volumePercent, const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/volume"_s);
    QUrlQuery query = {{u"volume_percent"_s, QString::number(volumePercent)}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(u"device_id"_s, deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::shuffle(bool state) const -> QFuture<RestReply>
{
    return shuffle(state, u""_s);
}

auto PlayerAPI::shuffle(bool state, const QString &deviceId) const -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/shuffle"_s);
    QUrlQuery query = {{u"volume_percent"_s, state ? "true" : "false"}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(u"device_id"_s, deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}

auto PlayerAPI::getRecentlyPlayed(int limit) const -> QFuture<RestReply>
{
    return getRecentlyPlayed({}, {}, limit);
}

auto PlayerAPI::getRecentlyPlayed(const QDateTime &after, const QDateTime &before, int limit) const
    -> QFuture<RestReply>
{
    QUrl url(u"https://api.spotify.com/v1/me/player/recently-played"_s);
    QUrlQuery query;

    if (!after.isNull())
    {
        query.addQueryItem(u"after"_s, QString::number(after.toMSecsSinceEpoch()));
    }

    if (!before.isNull())
    {
        if (!after.isNull())
        {
            qCWarning(gmSpotifyPlayer())
                << R"(getRecentlyPlayed(): "before" and "after" have been declared at the same time -> using only "after")";
        }
        else
        {
            query.addQueryItem(u"before"_s, QString::number(before.toMSecsSinceEpoch()));
        }
    }

    query.addQueryItem(u"limit"_s, QString::number(std::clamp(limit, 0, MAX_RECENTLY_PLAYED)));

    url.setQuery(query);
    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated | Option::LowPriority);
}

auto PlayerAPI::addToQueue(const QString &uri) const -> QFuture<RestReply>
{
    return addToQueue(uri, u""_s);
}

auto PlayerAPI::addToQueue(const QString &uri, const QString &deviceId) const -> QFuture<RestReply>
{
    if (const auto type = SpotifyUtils::getUriType(uri);
        !(type == SpotifyUtils::SpotifyType::Track || type == SpotifyUtils::SpotifyType::Episode))
    {
        qCWarning(gmSpotifyPlayer()) << R"(addToQueue(): "uri" must be a track or episode!)";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/me/player/queue"_s);
    QUrlQuery query = {{u"uri"_s, uri}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(u"device_id"_s, deviceId);
    }

    url.setQuery(query);
    return m_spotify->post(NetworkUtils::makeJsonRequest(url), {}, Option::Authenticated);
}
