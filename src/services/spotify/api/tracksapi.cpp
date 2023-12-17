#include "tracksapi.h"
#include "spotify/spotify.h"
#include "utils/networkutils.h"
#include <QLoggingCategory>
#include <QUrlQuery>

Q_LOGGING_CATEGORY(gmSpotifyTracks, "gm.services.spotify.api.tracks")

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr size_t MAX_TRACK_COUNT = 50;

TracksAPI::TracksAPI(Spotify *parent) : m_spotify(parent)
{
}

auto TracksAPI::getTrack(const QString &id, Options options) -> QFuture<SpotifyTrack>
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

    const auto callback = [](const RestReply &reply) -> QFuture<SpotifyTrack> {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyTracks()) << "getTrack():" << reply.errorText();
            return {};
        }

        return QtFuture::makeReadyFuture(SpotifyTrack::fromJson(reply.data()));
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated | options).then(callback).unwrap();
}

auto TracksAPI::getTracks(const QStringList &ids, Options options) -> QFuture<std::vector<SpotifyTrack>>
{
    if (ids.isEmpty())
    {
        qCCritical(gmSpotifyTracks()) << "getTracks(): id list must not be empty!";
        return {};
    }

    return getTracks(ids, {}, options);
}

auto TracksAPI::getTracks(const QStringList &ids, std::vector<SpotifyTrack> &&previous, Options options)
    -> QFuture<std::vector<SpotifyTrack>>
{
    auto batch = getNextBatch(ids, previous);
    if (batch.isEmpty()) return QtFuture::makeReadyFuture(previous);

    QUrl url(u"https://api.spotify.com/v1/tracks"_s);

    QUrlQuery query;
    query.addQueryItem(u"ids"_s, batch.join(','));
    query.addQueryItem(u"market"_s, u"from_token"_s);
    url.setQuery(query);

    const auto callback = [this, ids, options, previous = std::move(previous)](const RestReply &reply) mutable {
        if (reply.hasError())
        {
            qCWarning(gmSpotifyTracks()) << "getTracks():" << reply.errorText();
            return QFuture<std::vector<SpotifyTrack>>();
        }

        if (previous.empty()) previous = SpotifyTrack::fromJsonArray(reply.data());
        else
        {
            auto newTracks = SpotifyTrack::fromJsonArray(reply.data());
            previous.insert(previous.end(), newTracks.begin(), newTracks.end());
        }

        if (ids.length() > previous.size())
        {
            return getTracks(ids, std::move(previous), options);
        }

        return QtFuture::makeReadyFuture(previous);
    };

    return m_spotify->get(NetworkUtils::makeJsonRequest(url), Option::Authenticated | options).then(callback).unwrap();
}

auto TracksAPI::getNextBatch(const QStringList &ids, const std::vector<SpotifyTrack> &previous) -> QStringList
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
