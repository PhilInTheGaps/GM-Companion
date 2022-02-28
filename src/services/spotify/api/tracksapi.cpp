#include "tracksapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyTracks, "gm.services.spotify.api.tracks")

using namespace AsyncFuture;

TracksAPI::TracksAPI(Spotify *parent) : QObject(parent), m_spotify(parent) {}

auto TracksAPI::getTrack(const QString &id) -> QFuture<QSharedPointer<SpotifyTrack>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyTracks()) << "getTrack(): id must not be empty!";
        return {};
    }

    QUrl url(QStringLiteral("https://api.spotify.com/v1/tracks/%1").arg(id));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("market"), QStringLiteral("from_token"));
    url.setQuery(query);

    const auto callback = [](RestNetworkReply *reply) {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyTracks()) << "getTrack():" << reply->errorText();
            reply->deleteLater();
            return QFuture<QSharedPointer<SpotifyTrack>>();
        }

        const auto data = reply->data();
        reply->deleteLater();

        auto track = SpotifyTrack::fromJson(data);
        return completed(track);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto TracksAPI::getTracks(const QStringList &ids) const -> QFuture<QVector<QSharedPointer<SpotifyTrack>>>
{
    if (ids.isEmpty())
    {
        qCCritical(gmSpotifyTracks()) << "getTracks(): id list must not be empty!";
        return {};
    }

    return getTracks(ids, {});
}

auto TracksAPI::getTracks(const QStringList &ids, QVector<QSharedPointer<SpotifyTrack>> &&previous) const -> QFuture<QVector<QSharedPointer<SpotifyTrack>>>
{
    auto batch = getNextBatch(ids, previous);
    if (batch.isEmpty()) return completed(previous);

    QUrl url(QStringLiteral("https://api.spotify.com/v1/tracks"));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("ids"), batch.join(','));
    query.addQueryItem(QStringLiteral("market"), QStringLiteral("from_token"));
    url.setQuery(query);

    const auto callback = [this, ids, previous](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyTracks()) << "getTracks():" << reply->errorText();
            reply->deleteLater();
            return QFuture<QVector<QSharedPointer<SpotifyTrack>>>();
        }

        const auto data = reply->data();
        reply->deleteLater();

        if (previous.isEmpty())
            previous = SpotifyTrack::fromJsonArray(data);
        else
            previous << SpotifyTrack::fromJsonArray(data);

        if (ids.length() > previous.length())
            return getTracks(ids, std::move(previous));

        return completed(previous);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto TracksAPI::getNextBatch(const QStringList &ids, const QVector<QSharedPointer<SpotifyTrack>> &previous) -> QStringList
{
    auto remaining = ids.length() - previous.length();
    if (remaining < 1) return {};

    const auto nextBatchSize = std::min(remaining, MAX_TRACK_COUNT);
    const auto lastIndex = nextBatchSize + previous.length() - 1;

    QStringList batch;
    batch.reserve(nextBatchSize);

    for (auto i = previous.length(); i <= lastIndex ; i++)
    {
        batch << ids[i];
    }

    return batch;
}


