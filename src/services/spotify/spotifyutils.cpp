#include "spotifyutils.h"
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmSpotifyUtils, "gm.service.spotify.utils")

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getUriType(const QString &uri) -> SpotifyType
{
    if (uri.contains(":album:"_L1) || uri.contains("/album/"_L1) || uri.contains("/albums/"_L1))
        return SpotifyType::Album;

    if (uri.contains(":playlist:"_L1) || uri.contains("/playlist/"_L1) || uri.contains("/playlists/"_L1))
        return SpotifyType::Playlist;

    if (uri.contains(":track:"_L1) || uri.contains("/track/"_L1) || uri.contains("/tracks/"_L1))
        return SpotifyType::Track;

    if (uri.contains(":artist:"_L1) || uri.contains("/artist/"_L1) || uri.contains("/artists/"_L1))
        return SpotifyType::Artist;

    if (uri.contains(":episode:"_L1) || uri.contains("/episode/"_L1) || uri.contains("/episodes/"_L1))
        return SpotifyType::Episode;

    if (uri.contains(":show:"_L1) || uri.contains("/show/"_L1) || uri.contains("/shows/"_L1)) return SpotifyType::Show;

    if (uri.contains(":local:"_L1) || uri.contains("/local/"_L1)) return SpotifyType::Local;

    return SpotifyType::Unknown;
}

auto SpotifyUtils::typeFromString(const QString &str) -> SpotifyUtils::SpotifyType
{
    if (str == "album"_L1) return SpotifyType::Album;
    if (str == "playlist"_L1) return SpotifyType::Playlist;
    if (str == "track"_L1) return SpotifyType::Track;
    if (str == "artist"_L1) return SpotifyType::Artist;
    if (str == "episode"_L1) return SpotifyType::Episode;
    if (str == "show"_L1) return SpotifyType::Show;
    if (str == "local"_L1) return SpotifyType::Local;

    qCCritical(gmSpotifyUtils()) << "Unknown type:" << str;
    return SpotifyType::Unknown;
}

/**
 * @brief Get Spotify ID from Spotify URI
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * or https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad
 * @return Spotify ID like 6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getIdFromUri(const QString &uri) -> QString
{
    const auto regex = QRegularExpression(ID_FROM_URI);

    if (const auto match = regex.match(uri); match.isValid() && match.hasMatch())
    {
        return match.captured("id"_L1);
    }

    return {};
}

/**
 * @brief Get the Spotify ID from href
 * @param href Spotify href like
 * https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2
 */
auto SpotifyUtils::getIdFromHref(const QString &href) -> QString
{
    const auto regex = QRegularExpression(ID_FROM_HREF);

    if (const auto match = regex.match(href); match.isValid() && match.hasMatch())
    {
        return match.captured("id"_L1);
    }

    return {};
}

auto SpotifyUtils::makeUri(const QString &input) -> QString
{
    if (isUri(input)) return input;

    const auto regex = QRegularExpression(ID_FROM_HREF);
    const auto match = regex.match(input);

    auto type = match.captured("type"_L1);
    if (type.endsWith('s')) type.chop(1); // if make singular, albums -> album

    const auto id = match.captured("id"_L1);

    return u"spotify:%1:%2"_s.arg(type, id);
}

auto SpotifyUtils::isUri(const QString &input) -> bool
{
    const auto regex = QRegularExpression(IS_URI);
    const auto match = regex.match(input);
    return match.isValid() && match.hasMatch();
}

auto SpotifyUtils::isContainerType(SpotifyType type) -> bool
{
    switch (type)
    {
    case SpotifyType::Album:
    case SpotifyType::Playlist:
    case SpotifyType::Show:
        return true;
    case SpotifyType::Track:
    case SpotifyType::Episode:
        return false;
    default:
        qCCritical(gmSpotifyUtils()) << "isContainerType(): unhandled type" << (int)type;
        return false;
    }
}
