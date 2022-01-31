#include "spotifyutils.h"
#include <QStringList>
#include <QRegularExpression>

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * @return 0: Album, 1: Playlist, 2: Track, 3: Artist, 4: episode, 5: show -1:
 * unknown
 */
auto SpotifyUtils::getUriType(const QString& uri) -> SpotifyType
{
    if (uri.contains("album:") || uri.contains("album/")) return Album;

    if (uri.contains("playlist:") || uri.contains("playlist/")) return Playlist;

    if (uri.contains("track:") || uri.contains("track/")) return Track;

    if (uri.contains("artist:") || uri.contains("artist/")) return Artist;

    if (uri.contains("episode:") || uri.contains("episode/")) return Episode;

    if (uri.contains("show:") || uri.contains("show/")) return Show;

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
        return match.captured(2);
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
        return match.captured(2);
    }

    return {};
}
