#include "youtube.h"
#include "utils/fileutils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QMediaFormat>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

Q_LOGGING_CATEGORY(gmYouTube, "gm.service.youtube")

YouTube::YouTube(QObject *parent) : Service{QStringLiteral("YouTube"), parent}
{
}

auto YouTube::instance() -> YouTube *
{
    if (!m_instance)
    {
        m_instance = new YouTube(nullptr);
    }

    return m_instance;
}

void YouTube::setNetworkManager(QNetworkAccessManager *networkManager)
{
    m_connector.setNetworkManager(networkManager);
}

void YouTube::selectNewPipedInstance()
{
    m_connector.selectBestInstance();
}

auto YouTube::getStreamInfoAsync(const VideoId &id, Options options) -> QFuture<YouTubeVideo>
{
    if (!id.isValid()) return {};

    const auto path = u"/streams/%1"_s.arg(id.toString());
    const auto request = QNetworkRequest(path);

    return m_connector.get(request, options)
        .then([this, id, options](const RestReply &reply) {
            bool ok = false;

            if (!reply.hasError())
            {
                auto video = parseStreamResponse(reply.data(), id, ok);

                if (ok)
                {
                    return QtFuture::makeReadyFuture(std::move(video));
                }
            }

            m_connector.selectBestInstance();
            return getStreamInfoAsync(id, options);
        })
        .unwrap();
}

auto YouTube::getPlaylistInfoAsync(const PlaylistId &id, Options options) -> QFuture<YouTubePlaylist>
{
    if (!id.isValid()) return {};

    const auto path = u"/playlists/%1"_s.arg(id.toString());
    const auto request = QNetworkRequest(path);

    return m_connector.get(request, options)
        .then([this, id, options](const RestReply &reply) {
            bool ok = false;

            if (!reply.hasError())
            {
                auto playlist = parsePlaylistResponse(reply.data(), id, options, ok);

                if (ok)
                {
                    return playlist;
                }
            }

            m_connector.selectBestInstance();
            return getPlaylistInfoAsync(id, options);
        })
        .unwrap();
}

void YouTube::connectService()
{
    // nop
}

void YouTube::disconnectService()
{
    // nop
}

auto YouTube::parseStreamResponse(const QByteArray &data, const VideoId &id, bool &ok) -> YouTubeVideo
{
    QJsonParseError parseError;
    const auto doc = QJsonDocument::fromJson(data, &parseError);

    YouTubeVideo video;
    video.id = id;

    if (parseError.error != QJsonParseError::NoError)
    {
        ok = false;
        return video;
    }

    video.title = doc["title"_L1].toString();
    video.uploader = doc["uploader"_L1].toString();
    video.thumbnailUrl = doc["thumbnailUrl"_L1].toString();

    int bestBitrate = 0;
    auto supportedFileFormats = QMediaFormat().supportedFileFormats(QMediaFormat::ConversionMode::Decode);
    auto supportedAudioCodecs = QMediaFormat().supportedAudioCodecs(QMediaFormat::ConversionMode::Decode);

    foreach (const auto &stream, doc["audioStreams"_L1].toArray())
    {
        if (stream["bitrate"_L1].toInt() > bestBitrate)
        {
            auto format = FileUtils::mimeTypeStringToFileFormat(stream["mimeType"_L1].toString());
            auto codec = FileUtils::audioCodecNameToEnum(stream["codec"_L1].toString());

            if (supportedFileFormats.contains(format) && supportedAudioCodecs.contains(codec))
            {
                bestBitrate = stream["bitrate"_L1].toInt();
                video.audioStreamUrl = stream["url"_L1].toString();

                qCDebug(gmYouTube()) << "Found good stream:" << bestBitrate << stream["mimeType"_L1].toString()
                                     << stream["codec"_L1].toString();
            }
        }
    }

    ok = true;
    return video;
}

auto YouTube::parsePlaylistResponse(const QByteArray &data, const PlaylistId &id, Options options, bool &ok)
    -> QFuture<YouTubePlaylist>
{
    QJsonParseError parseError;
    const auto doc = QJsonDocument::fromJson(data, &parseError);

    YouTubePlaylist playlist;
    playlist.id = id;

    if (parseError.error != QJsonParseError::NoError)
    {
        ok = false;
        return QtFuture::makeReadyFuture(std::move(playlist));
    }

    playlist.title = doc["name"_L1].toString();
    playlist.uploader = doc["uploader"_L1].toString();
    playlist.thumbnailUrl = doc["thumbnailUrl"_L1].toString();

    foreach (const auto &entry, doc["relatedStreams"_L1].toArray())
    {
        playlist.streams.push_back(YouTubeVideo{VideoId(entry["url"_L1].toString()), entry["title"_L1].toString(),
                                                entry["uploader"_L1].toString(), entry["thumbnailUrl"_L1].toString(),
                                                u""_s});
    }

    ok = true;

    const auto nextpage = QUrl::toPercentEncoding(doc["nextpage"_L1].toString(), "", ":");
    if (!nextpage.isEmpty())
    {
        return continueLoadingOfPlaylist(std::move(playlist), nextpage, options);
    }

    return QtFuture::makeReadyFuture(std::move(playlist));
}

auto YouTube::continueLoadingOfPlaylist(YouTubePlaylist &&playlist, const QString &nextpage, Options options)
    -> QFuture<YouTubePlaylist>
{
    const auto path = u"/nextpage/playlists/%1?nextpage=%2"_s.arg(playlist.id.toString(), nextpage);
    const auto request = QNetworkRequest(path);

    return m_connector.get(request, options)
        .then([this, playlist = std::move(playlist), options](const RestReply &reply) mutable {
            if (reply.hasError())
            {
                qCDebug(gmYouTube()) << "Error while retrieving next page of playlist:" << reply.errorText();
                return QtFuture::makeReadyFuture(std::move(playlist));
            }

            const auto doc = QJsonDocument::fromJson(reply.data());
            foreach (const auto &entry, doc["relatedStreams"_L1].toArray())
            {
                playlist.streams.push_back(YouTubeVideo{VideoId(entry["url"_L1].toString()),
                                                        entry["title"_L1].toString(), entry["uploader"_L1].toString(),
                                                        entry["thumbnailUrl"_L1].toString(), u""_s});
            }

            const auto nextpage = QUrl::toPercentEncoding(doc["nextpage"_L1].toString(), "", ":");
            if (!nextpage.isEmpty())
            {
                return continueLoadingOfPlaylist(std::move(playlist), nextpage, options);
            }

            return QtFuture::makeReadyFuture(std::move(playlist));
        })
        .unwrap();
}
