#include "playerapi.h"
#include "spotify/spotify.h"
#include "spotify/spotifyutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/networkutils.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QUrlQuery>
#include <algorithm>

Q_LOGGING_CATEGORY(gmSpotifyPlayer, "gm.spotify.api.player")

using namespace AsyncFuture;

PlayerAPI::PlayerAPI(Spotify *parent) : QObject(parent), m_spotify(parent)
{
}

auto PlayerAPI::play() const -> QFuture<RestNetworkReply *>
{
    return m_spotify->put(QUrl("https://api.spotify.com/v1/me/player/play"));
}

auto PlayerAPI::play(const QString &deviceId, const QJsonObject &body) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/play");

    if (!deviceId.isEmpty())
    {
        QUrlQuery query{{QStringLiteral("device_id"), deviceId}};
        url.setQuery(query);
    }

    const QJsonDocument content(body);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), content.toJson(QJsonDocument::JsonFormat::Compact));
}

auto PlayerAPI::play(const QString &uri) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    const auto type = SpotifyUtils::getUriType(uri);

    if (type == SpotifyUtils::SpotifyType::Track || type == SpotifyUtils::SpotifyType::Episode)
    {
        return play(makePlayBody("", {uri}, -1, -1));
    }

    return play(makePlayBody(uri, {}, -1, -1));
}

auto PlayerAPI::play(const QJsonObject &body) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return play("", body);
}

auto PlayerAPI::makePlayBody(const QString &contextUri, const QStringList &uris, int position, int positionMs)
    -> QJsonObject
{
    QJsonObject body;

    if (!contextUri.isEmpty())
    {
        body["context_uri"] = contextUri;
    }

    if (!uris.isEmpty())
    {
        body["uris"] = QJsonArray::fromStringList(uris);
    }

    if (position > -1)
    {
        QJsonObject offset{{"position", position}};
        body["offset"] = offset;
    }

    if (positionMs > -1)
    {
        body["position_ms"] = positionMs;
    }

    return body;
}

auto PlayerAPI::pause() const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return pause(QLatin1String());
}

auto PlayerAPI::pause(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/pause");

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{QStringLiteral("device_id"), deviceId}};
        url.setQuery(query);
    }

    return m_spotify->put(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::next() const -> QFuture<RestNetworkReply *>
{
    return next(QLatin1String());
}

auto PlayerAPI::next(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/next");

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{QStringLiteral("device_id"), deviceId}};
        url.setQuery(query);
    }

    return m_spotify->post(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::previous() const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return previous(QLatin1String());
}

auto PlayerAPI::previous(const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/previous");

    if (!deviceId.isEmpty())
    {
        QUrlQuery query = {{QStringLiteral("device_id"), deviceId}};
        url.setQuery(query);
    }

    return m_spotify->post(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::seek(int positionMs) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return seek(positionMs, QLatin1String());
}

auto PlayerAPI::seek(int positionMs, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/seek");
    QUrlQuery query = {{QStringLiteral("position_ms"), QString::number(positionMs)}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(QStringLiteral("device_id"), deviceId);
    }

    url.setQuery(query);

    return m_spotify->put(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::getState() const -> QFuture<QSharedPointer<SpotifyPlaybackState>>
{
    return getState({}, QLatin1String());
}

auto PlayerAPI::getState(const QStringList &additionalTypes, const QString &market) const
    -> QFuture<QSharedPointer<SpotifyPlaybackState>>
{
    QUrl url("https://api.spotify.com/v1/me/player");
    QUrlQuery query;

    if (!additionalTypes.isEmpty())
    {
        query.addQueryItem(QStringLiteral("additional_types"), additionalTypes.join(','));
    }

    if (!market.isEmpty())
    {
        query.addQueryItem(QStringLiteral("market"), market);
    }

    url.setQuery(query);

    const auto callback = [](gsl::owner<RestNetworkReply *> reply) -> QFuture<QSharedPointer<SpotifyPlaybackState>> {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyPlayer()) << reply->errorText();
            reply->deleteLater();
            return {};
        }

        const auto data = reply->data();
        reply->deleteLater();

        return completed(SpotifyPlaybackState::fromJson(QJsonDocument::fromJson(data)));
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto PlayerAPI::getCurrentlyPlaying() const -> QFuture<QSharedPointer<SpotifyCurrentTrack>>
{
    return getCurrentlyPlaying({}, QLatin1String());
}

auto PlayerAPI::getCurrentlyPlaying(const QStringList &additionalTypes, const QString &market) const
    -> QFuture<QSharedPointer<SpotifyCurrentTrack>>
{
    QUrl url("https://api.spotify.com/v1/me/player/currently-playing");
    QUrlQuery query;

    if (!additionalTypes.isEmpty())
    {
        query.addQueryItem(QStringLiteral("additional_types"), additionalTypes.join(','));
    }

    if (!market.isEmpty())
    {
        query.addQueryItem(QStringLiteral("market"), market);
    }

    query.addQueryItem(QStringLiteral("market"), QStringLiteral("from_token"));
    url.setQuery(query);

    const auto callback = [](gsl::owner<RestNetworkReply *> reply) -> QFuture<QSharedPointer<SpotifyCurrentTrack>> {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyPlayer()) << reply->errorText();
            reply->deleteLater();
            return {};
        }

        const auto data = reply->data();
        reply->deleteLater();

        return completed(SpotifyCurrentTrack::fromJson(data));
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto PlayerAPI::transfer(const QStringList &deviceIds) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return transfer(deviceIds, false);
}

auto PlayerAPI::transfer(const QStringList &deviceIds, bool play) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player");

    const QJsonObject body{{QStringLiteral("device_ids"), QJsonArray::fromStringList(deviceIds)},
                           {QStringLiteral("play"), play}};

    const auto doc = QJsonDocument(body);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url), doc.toJson(QJsonDocument::Compact));
}

auto PlayerAPI::devices() const -> QFuture<QSharedPointer<SpotifyDeviceList>>
{
    const QUrl url("https://api.spotify.com/v1/me/player/devices");

    const auto callback = [](gsl::owner<RestNetworkReply *> reply) {
        const auto data = reply->data();
        reply->deleteLater();

        const auto json = QJsonDocument::fromJson(data).object();
        const auto devices = json[QStringLiteral("devices")].toArray();
        return SpotifyDevice::fromJson(devices);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto PlayerAPI::repeat(SpotifyRepeatMode mode) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return repeat(mode, QLatin1String());
}

auto PlayerAPI::repeat(SpotifyRepeatMode mode, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    switch (mode)
    {
    case SpotifyRepeatMode::Track:
        return repeat(QStringLiteral("track"), deviceId);
    case SpotifyRepeatMode::Context:
        return repeat(QStringLiteral("context"), deviceId);
    case SpotifyRepeatMode::Off:
        return repeat(QStringLiteral("off"), deviceId);
    default:
        qCCritical(gmSpotifyPlayer()) << "repeat(): unknown repeat mode" << mode;
        return {};
    }
}

auto PlayerAPI::repeat(const QString &state, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/repeat");
    QUrlQuery query = {{QStringLiteral("state"), state}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(QStringLiteral("device_id"), deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::volume(int volumePercent) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return volume(volumePercent, QLatin1String());
}

auto PlayerAPI::volume(int volumePercent, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/volume");
    QUrlQuery query = {{QStringLiteral("volume_percent"), QString::number(volumePercent)}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(QStringLiteral("device_id"), deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::shuffle(bool state) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return shuffle(state, QLatin1String());
}

auto PlayerAPI::shuffle(bool state, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/shuffle");
    QUrlQuery query = {{QStringLiteral("volume_percent"), state ? "true" : "false"}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(QStringLiteral("device_id"), deviceId);
    }

    url.setQuery(query);
    return m_spotify->put(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::getRecentlyPlayed(int limit) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return getRecentlyPlayed({}, {}, limit);
}

auto PlayerAPI::getRecentlyPlayed(const QDateTime &after, const QDateTime &before, int limit) const
    -> QFuture<gsl::owner<RestNetworkReply *>>
{
    QUrl url("https://api.spotify.com/v1/me/player/recently-played");
    QUrlQuery query;

    if (!after.isNull())
    {
        query.addQueryItem(QStringLiteral("after"), QString::number(after.toMSecsSinceEpoch()));
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
            query.addQueryItem(QStringLiteral("before"), QString::number(before.toMSecsSinceEpoch()));
        }
    }

    query.addQueryItem(QStringLiteral("limit"), QString::number(std::clamp(limit, 0, MAX_RECENTLY_PLAYED)));

    url.setQuery(query);
    return m_spotify->get(NetworkUtils::makeJsonRequest(url));
}

auto PlayerAPI::addToQueue(const QString &uri) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return addToQueue(uri, QLatin1String());
}

auto PlayerAPI::addToQueue(const QString &uri, const QString &deviceId) const -> QFuture<gsl::owner<RestNetworkReply *>>
{
    const auto type = SpotifyUtils::getUriType(uri);
    if (!(type == SpotifyUtils::SpotifyType::Track || type == SpotifyUtils::SpotifyType::Episode))
    {
        qCWarning(gmSpotifyPlayer()) << R"(addToQueue(): "uri" must be a track or episode!)";
        return {};
    }

    QUrl url("https://api.spotify.com/v1/me/player/queue");
    QUrlQuery query = {{QStringLiteral("uri"), uri}};

    if (!deviceId.isEmpty())
    {
        query.addQueryItem(QStringLiteral("device_id"), deviceId);
    }

    url.setQuery(query);
    return m_spotify->post(NetworkUtils::makeJsonRequest(url));
}
