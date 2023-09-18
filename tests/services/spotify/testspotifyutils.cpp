#include "src/services/spotify/spotifyutils.h"
#include <gtest/gtest.h>

struct UriTypeTest
{
    const char *description;
    QString uri;
    SpotifyUtils::SpotifyType type;
};

TEST(SpotifyUtilsTest, GetUriType)
{
    std::vector<UriTypeTest> tests = {
        {"album uri", "spotify:album:4Io5vWtmV1rFj4yirKb4y4", SpotifyUtils::SpotifyType::Album},
        {"album web url", "https://open.spotify.com/album/132qAo1cDiEJdA3fv4xyNK?si=8wLG5MJHS8WguOzbP4O36g",
         SpotifyUtils::SpotifyType::Album},
        {"playlist uri", "spotify:playlist:37i9dQZF1DX4tMcImWolDJ", SpotifyUtils::SpotifyType::Playlist},
        {"user playlist uri", "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo",
         SpotifyUtils::SpotifyType::Playlist},
        {"user playlist web url", "https://open.spotify.com/playlist/2Tegyx2csiWbOhS6lPqRrm?si=0e114fe76a56450e",
         SpotifyUtils::SpotifyType::Playlist},
        {"track uri", "spotify:track:59WN2psjkt1tyaxjspN8fp", SpotifyUtils::SpotifyType::Track},
        {"track web url", "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad",
         SpotifyUtils::SpotifyType::Track},
        {"artist uri", "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj", SpotifyUtils::SpotifyType::Artist},
        {"artist web url", "https://open.spotify.com/artist/5M52tdBnJaKSvOpJGz8mfZ?si=avm9TJwVTB2n08LClUq5YA",
         SpotifyUtils::SpotifyType::Artist},
        {"episode uri", "spotify:episode:0Q86acNRm6V9GYx55SXKwf", SpotifyUtils::SpotifyType::Episode},
        {"episode web url", "https://open.spotify.com/episode/4K9wijPGBjwXjnSiEIkRLy?si=71701a1d55c14e82",
         SpotifyUtils::SpotifyType::Episode},
        {"episode api url", "https://api.spotify.com/v1/episodes/5Xt5DXGzch68nYYamXrNxZ",
         SpotifyUtils::SpotifyType::Episode},
        {"show uri", "spotify:show:5CnDmMUG0S5bSSw612fs8C", SpotifyUtils::SpotifyType::Show},
        {"show web url", "https://open.spotify.com/show/6qABy0MzyKURDbwLKZkeK6?si=79e98224367048ef",
         SpotifyUtils::SpotifyType::Show},
        {"local", "spotify:local:Jeremy+Soule+%2F+Julian+Soule:Guild+Wars+2+Original+Game+Soundtrack:The+Seraph:193",
         SpotifyUtils::SpotifyType::Local},
        {"unknown uri", "spotify:something:xxxxxxxxx", SpotifyUtils::SpotifyType::Unknown}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(SpotifyUtils::getUriType(test.uri), test.type) << test.description;
    }
}

struct IdFromUriTest
{
    const char *description;
    QString uri;
    QString id;
};

TEST(SpotifyUtilsTest, GetIdFromUri)
{
    std::vector<IdFromUriTest> tests = {
        {"album uri", "spotify:album:4Io5vWtmV1rFj4yirKb4y4", "4Io5vWtmV1rFj4yirKb4y4"},
        {"album web url", "https://open.spotify.com/album/132qAo1cDiEJdA3fv4xyNK?si=8wLG5MJHS8WguOzbP4O36g",
         "132qAo1cDiEJdA3fv4xyNK"},
        {"playlist uri", "spotify:playlist:37i9dQZF1DX4tMcImWolDJ", "37i9dQZF1DX4tMcImWolDJ"},
        {"user playlist uri", "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo", "0Q6hJZYIEu3LwbyBBHjjHo"},
        {"user playlist web url", "https://open.spotify.com/playlist/2Tegyx2csiWbOhS6lPqRrm?si=0e114fe76a56450e",
         "2Tegyx2csiWbOhS6lPqRrm"},
        {"track uri", "spotify:track:59WN2psjkt1tyaxjspN8fp", "59WN2psjkt1tyaxjspN8fp"},
        {"track web url", "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad",
         "0W35nxtHtFlseSojmygEsf"},
        {"artist uri", "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj", "2d0hyoQ5ynDBnkvAbJKORj"},
        {"artist web url", "https://open.spotify.com/artist/5M52tdBnJaKSvOpJGz8mfZ?si=avm9TJwVTB2n08LClUq5YA",
         "5M52tdBnJaKSvOpJGz8mfZ"},
        {"episode uri", "spotify:episode:0Q86acNRm6V9GYx55SXKwf", "0Q86acNRm6V9GYx55SXKwf"},
        {"episode web url", "https://open.spotify.com/episode/4K9wijPGBjwXjnSiEIkRLy?si=71701a1d55c14e82",
         "4K9wijPGBjwXjnSiEIkRLy"},
        {"show uri", "spotify:show:5CnDmMUG0S5bSSw612fs8C", "5CnDmMUG0S5bSSw612fs8C"},
        {"show web url", "https://open.spotify.com/show/6qABy0MzyKURDbwLKZkeK6?si=79e98224367048ef",
         "6qABy0MzyKURDbwLKZkeK6"},
        {"unknown uri", "spotify:something:xxxxxxxxx", "xxxxxxxxx"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(SpotifyUtils::getIdFromUri(test.uri), test.id) << test.description;
    }
}

struct IdFromHrefTest
{
    const char *description;
    QString href;
    QString id;
};

TEST(SpotifyUtilsTest, GetIdFromHref)
{
    std::vector<IdFromHrefTest> tests = {
        {"album", "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2",
         "6akEvsycLGftJxYudPjmqK"},
        {"playlist", "https://api.spotify.com/v1/users/spotify_espa%C3%B1a/playlists/21THa8j9TaSGuXYNBU5tsC/tracks",
         "21THa8j9TaSGuXYNBU5tsC"},
        {"track", "https://api.spotify.com/v1/tracks/5TiMM0jscVmWgMwR42RLup", "5TiMM0jscVmWgMwR42RLup"},
        {"artist", "https://api.spotify.com/v1/artists/2d0hyoQ5ynDBnkvAbJKORj", "2d0hyoQ5ynDBnkvAbJKORj"},
        {"episode", "https://api.spotify.com/v1/episodes/4K9wijPGBjwXjnSiEIkRLy", "4K9wijPGBjwXjnSiEIkRLy"},
        {"show", "https://api.spotify.com/v1/shows/6qABy0MzyKURDbwLKZkeK6", "6qABy0MzyKURDbwLKZkeK6"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(SpotifyUtils::getIdFromHref(test.href), test.id) << test.description;
    }
}

struct IsUriTest
{
    const char *description;
    QString input;
    bool result;
};

TEST(SpotifyUtilsTest, IsUri)
{
    std::vector<IsUriTest> tests = {
        {"track uri", "spotify:track:59WN2psjkt1tyaxjspN8fp", true},
        {"playlist uri", "spotify:playlist:37i9dQZF1DX4tMcImWolDJ", true},
        {"album uri", "spotify:album:4Io5vWtmV1rFj4yirKb4y4", true},
        {"artist uri", "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj", true},
        {"api href", "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2", false},
        {"web href", "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf", false}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(SpotifyUtils::isUri(test.input), test.result) << test.description;
    }
}

struct MakeUriTest
{
    const char *description;
    QString input;
    QString result;
};

TEST(SpotifyUtilsTest, MakeUri)
{
    std::vector<MakeUriTest> tests = {
        {"uri", "spotify:track:59WN2psjkt1tyaxjspN8fp", "spotify:track:59WN2psjkt1tyaxjspN8fp"},
        {"api href", "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2",
         "spotify:album:6akEvsycLGftJxYudPjmqK"},
        {"web href", "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad",
         "spotify:track:0W35nxtHtFlseSojmygEsf"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(SpotifyUtils::makeUri(test.input), test.result) << test.description;
    }
}
