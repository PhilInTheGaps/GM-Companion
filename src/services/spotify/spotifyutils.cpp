#include "spotifyutils.h"
#include <QStringList>

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * @return 0: Album, 1: Playlist, 2: Track, 3: Artist, 4: episode, 5: show -1:
 * unknown
 */
auto SpotifyUtils::getUriType(const QString& uri)->int
{
    if (uri.contains("album:")) return 0;

    if (uri.contains("playlist:")) return 1;

    if (uri.contains("track:")) return 2;

    if (uri.contains("artist:")) return 3;

    if (uri.contains("episode:")) return 4;

    if (uri.contains("show:")) return 5;

    return -1;
}

/**
 * @brief Get Spotify ID from Spotify URI
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * @return Spotify ID like 6rqhFgbbKwnb9MLmUQDhG6
 */
auto SpotifyUtils::getIdFromUri(QString uri)->QString
{
    return uri = uri.right(uri.count() - uri.lastIndexOf(":") - 1);
}

/**
 * @brief Get the Spotify ID from href
 * @param href Spotify href like
 * https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2
 */
auto SpotifyUtils::getIdFromHref(const QString& href)->QString
{
    auto split = href.split("/");
    auto index = -1;

    if (split.contains("albums"))
    {
        index = split.indexOf("albums") + 1;
    }
    else if (split.contains("playlists"))
    {
        index = split.indexOf("playlists") + 1;
    }
    else if (split.contains("tracks"))
    {
        index = split.indexOf("tracks") + 1;
    }

    return split[index];
}
