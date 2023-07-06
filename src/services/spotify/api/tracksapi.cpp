#include "tracksapi.h"
#include "spotify/spotify.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/networkutils.h"
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyTracks, "gm.services.spotify.api.tracks")

using namespace Qt::Literals::StringLiterals;
using namespace AsyncFuture;

constexpr size_t MAX_TRACK_COUNT = 50;

TracksAPI::TracksAPI(Spotify *parent) : QObject(parent), m_spotify(parent)
{
}

auto TracksAPI::getTrack(const QString &id) -> QFuture<QSharedPointer<SpotifyTrack>>
{
    if (id.isEmpty())
    {
        qCCritical(gmSpotifyTracks()) << "getTrack(): id must not be empty!";
        return {};
    }

    QUrl url(u"https://api.spotify.com/v1/tracks/%1"_s.arg(id));

    QUrlQuery query;
    query.addQueryItem(u"market"_s, u"from_token"_s);
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

auto TracksAPI::getTracks(const QStringList &ids) const -> QFuture<std::vector<QSharedPointer<SpotifyTrack>>>
{
    if (ids.isEmpty())
    {
        qCCritical(gmSpotifyTracks()) << "getTracks(): id list must not be empty!";
        return {};
    }

    return getTracks(ids, {});
}

auto TracksAPI::getTracks(const QStringList &ids, std::vector<QSharedPointer<SpotifyTrack>> &&previous) const
    -> QFuture<std::vector<QSharedPointer<SpotifyTrack>>>
{
    auto batch = getNextBatch(ids, previous);
    if (batch.isEmpty()) return completed<std::vector<QSharedPointer<SpotifyTrack>>>(previous);

    QUrl url(u"https://api.spotify.com/v1/tracks"_s);

    QUrlQuery query;
    query.addQueryItem(u"ids"_s, batch.join(','));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [this, ids, previous](RestNetworkReply *reply) mutable {
        if (reply->hasError())
        {
            qCWarning(gmSpotifyTracks()) << "getTracks():" << reply->errorText();
            reply->deleteLater();
            return QFuture<std::vector<QSharedPointer<SpotifyTrack>>>();
        }

        const auto data = reply->data();
        reply->deleteLater();

        if (previous.empty()) previous = SpotifyTrack::fromJsonArray(data);
        else
        {
            auto newTracks = SpotifyTrack::fromJsonArray(data);
            previous.insert(previous.end(), newTracks.begin(), newTracks.end());
        }

        if (static_cast<size_t>(ids.length()) > previous.size()) return getTracks(ids, std::move(previous));

        return completed(previous);
    };

    return observe(m_spotify->get(NetworkUtils::makeJsonRequest(url))).subscribe(callback).future();
}

auto TracksAPI::getNextBatch(const QStringList &ids, const std::vector<QSharedPointer<SpotifyTrack>> &previous)
    -> QStringList
{
    size_t remaining = ids.length() - previous.size();
    if (remaining < 1) return {};

    const auto nextBatchSize = std::min(remaining, MAX_TRACK_COUNT);
    const auto lastIndex = nextBatchSize + previous.size() - 1;

    QStringList batch;
    batch.reserve(nextBatchSize);

    for (auto i = previous.size(); i <= lastIndex; i++)
    {
        batch << ids.at(i);
    }

    return batch;
}
