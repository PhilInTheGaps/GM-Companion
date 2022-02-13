#include "spotifyutils.h"
#include <QStringList>
#include <QRegularExpression>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyUtils, "gm.service.spotify.utils")

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getUriType(const QString& uri) -> SpotifyType
{
    if (uri.contains(QStringLiteral(":album:"))
            || uri.contains(QStringLiteral("/album/"))
            || uri.contains(QStringLiteral("/albums/"))) return Album;

    if (uri.contains(QStringLiteral(":playlist:"))
            || uri.contains(QStringLiteral("/playlist/"))
            || uri.contains(QStringLiteral("/playlists/"))) return Playlist;

    if (uri.contains(QStringLiteral(":track:"))
            || uri.contains(QStringLiteral("/track/"))
            || uri.contains(QStringLiteral("/tracks/"))) return Track;

    if (uri.contains(QStringLiteral(":artist:"))
            || uri.contains(QStringLiteral("/artist/"))
            || uri.contains(QStringLiteral("/artists/"))) return Artist;

    if (uri.contains(QStringLiteral(":episode:"))
            || uri.contains(QStringLiteral("/episode/"))
            || uri.contains(QStringLiteral("/episodes/"))) return Episode;

    if (uri.contains(QStringLiteral(":show:"))
            || uri.contains(QStringLiteral("/show/"))
            || uri.contains(QStringLiteral("/shows/"))) return Show;

    if (uri.contains(QStringLiteral(":local:"))
            || uri.contains(QStringLiteral("/local/"))) return Local;

    return Unknown;
}

auto SpotifyUtils::typeFromString(const QString &str) -> SpotifyUtils::SpotifyType
{
    if (str == QStringLiteral("album")) return Album;
    if (str == QStringLiteral("playlist")) return Playlist;
    if (str == QStringLiteral("track")) return Track;
    if (str == QStringLiteral("artist")) return Artist;
    if (str == QStringLiteral("episode")) return Episode;
    if (str == QStringLiteral("show")) return Show;
    if (str == QStringLiteral("local")) return Local;

    qCCritical(gmSpotifyUtils()) << "Unknown type:" << str;
    return Unknown;
}

/**
 * @brief Get Spotify ID from Spotify URI
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * or https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad
 * @return Spotify ID like 6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getIdFromUri(const QString &uri)->QString
{
    const auto regex = QRegularExpression(ID_FROM_URI);
    const auto match = regex.match(uri);

    if (match.isValid() && match.hasMatch())
    {
        return match.captured(QStringLiteral("id"));
    }

    return {};
}

/**
 * @brief Get the Spotify ID from href
 * @param href Spotify href like
 * https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2
 */
auto SpotifyUtils::getIdFromHref(const QString& href)->QString
{
    const auto regex = QRegularExpression(ID_FROM_HREF);
    const auto match = regex.match(href);

    if (match.isValid() && match.hasMatch())
    {
        return match.captured(QStringLiteral("id"));
    }

    return {};
}

auto SpotifyUtils::makeUri(const QString &input) -> QString
{
    if (isUri(input)) return input;

    const auto regex = QRegularExpression(ID_FROM_HREF);
    const auto match = regex.match(input);

    auto type = match.captured(QStringLiteral("type"));
    if (type.endsWith('s')) type.chop(1); // if make singular, albums -> album

    const auto id = match.captured(QStringLiteral("id"));

    return QStringLiteral("spotify:%1:%2").arg(type, id);
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
    case Album:
    case Playlist:
    case Show:
        return true;
    case Track:
    case Episode:
        return false;
    default:
        qCCritical(gmSpotifyUtils()) << "isContainerType(): unhandled type" << type;
        return false;
    }
}
