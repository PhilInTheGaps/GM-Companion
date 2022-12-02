#include "spotifyutils.h"
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QStringList>

Q_LOGGING_CATEGORY(gmSpotifyUtils, "gm.service.spotify.utils")

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getUriType(const QString &uri) -> SpotifyType
{
    if (uri.contains(QStringLiteral(":album:")) || uri.contains(QStringLiteral("/album/")) ||
        uri.contains(QStringLiteral("/albums/")))
        return SpotifyType::Album;

    if (uri.contains(QStringLiteral(":playlist:")) || uri.contains(QStringLiteral("/playlist/")) ||
        uri.contains(QStringLiteral("/playlists/")))
        return SpotifyType::Playlist;

    if (uri.contains(QStringLiteral(":track:")) || uri.contains(QStringLiteral("/track/")) ||
        uri.contains(QStringLiteral("/tracks/")))
        return SpotifyType::Track;

    if (uri.contains(QStringLiteral(":artist:")) || uri.contains(QStringLiteral("/artist/")) ||
        uri.contains(QStringLiteral("/artists/")))
        return SpotifyType::Artist;

    if (uri.contains(QStringLiteral(":episode:")) || uri.contains(QStringLiteral("/episode/")) ||
        uri.contains(QStringLiteral("/episodes/")))
        return SpotifyType::Episode;

    if (uri.contains(QStringLiteral(":show:")) || uri.contains(QStringLiteral("/show/")) ||
        uri.contains(QStringLiteral("/shows/")))
        return SpotifyType::Show;

    if (uri.contains(QStringLiteral(":local:")) || uri.contains(QStringLiteral("/local/"))) return SpotifyType::Local;

    return SpotifyType::Unknown;
}

auto SpotifyUtils::typeFromString(const QString &str) -> SpotifyUtils::SpotifyType
{
    if (str == QStringLiteral("album")) return SpotifyType::Album;
    if (str == QStringLiteral("playlist")) return SpotifyType::Playlist;
    if (str == QStringLiteral("track")) return SpotifyType::Track;
    if (str == QStringLiteral("artist")) return SpotifyType::Artist;
    if (str == QStringLiteral("episode")) return SpotifyType::Episode;
    if (str == QStringLiteral("show")) return SpotifyType::Show;
    if (str == QStringLiteral("local")) return SpotifyType::Local;

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
        return match.captured(QStringLiteral("id"));
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
