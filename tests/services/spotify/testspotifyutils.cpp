#include <QtTest>
#include <QObject>
#include "../../src/services/spotify/spotifyutils.h"

class TestSpotifyUtils : public QObject
{
    Q_OBJECT
public:
    TestSpotifyUtils() = default;

private slots:
    static void getUriType_data();
    static void getUriType();

    static void getIdFromUri_data();
    static void getIdFromUri();

    static void getIdFromHref_data();
    static void getIdFromHref();

    static void isUri_data();
    static void isUri();

    static void makeUri_data();
    static void makeUri();
};

void TestSpotifyUtils::getUriType_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<SpotifyUtils::SpotifyType>("type");

    QTest::newRow("album") << "spotify:album:4Io5vWtmV1rFj4yirKb4y4" << SpotifyUtils::Album;
    QTest::newRow("album") << "https://open.spotify.com/album/132qAo1cDiEJdA3fv4xyNK?si=8wLG5MJHS8WguOzbP4O36g" << SpotifyUtils::Album;

    QTest::newRow("playlist") << "spotify:playlist:37i9dQZF1DX4tMcImWolDJ" << SpotifyUtils::Playlist;
    QTest::newRow("user playlist") << "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo" << SpotifyUtils::Playlist;
    QTest::newRow("user playlist") << "https://open.spotify.com/playlist/2Tegyx2csiWbOhS6lPqRrm?si=0e114fe76a56450e" << SpotifyUtils::Playlist;

    QTest::newRow("track") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << SpotifyUtils::Track;
    QTest::newRow("track") << "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad" << SpotifyUtils::Track;

    QTest::newRow("artist") << "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj" << SpotifyUtils::Artist;
    QTest::newRow("artist") << "https://open.spotify.com/artist/5M52tdBnJaKSvOpJGz8mfZ?si=avm9TJwVTB2n08LClUq5YA" << SpotifyUtils::Artist;

    QTest::newRow("episode") << "spotify:episode:0Q86acNRm6V9GYx55SXKwf" << SpotifyUtils::Episode;
    QTest::newRow("episode") << "https://open.spotify.com/episode/4K9wijPGBjwXjnSiEIkRLy?si=71701a1d55c14e82" << SpotifyUtils::Episode;
    QTest::newRow("episode") << "https://api.spotify.com/v1/episodes/5Xt5DXGzch68nYYamXrNxZ" << SpotifyUtils::Episode;

    QTest::newRow("show") << "spotify:show:5CnDmMUG0S5bSSw612fs8C" << SpotifyUtils::Show;
    QTest::newRow("show") << "https://open.spotify.com/show/6qABy0MzyKURDbwLKZkeK6?si=79e98224367048ef" << SpotifyUtils::Show;

    QTest::newRow("local") << "spotify:local:Jeremy+Soule+%2F+Julian+Soule:Guild+Wars+2+Original+Game+Soundtrack:The+Seraph:193" << SpotifyUtils::Local;

    QTest::newRow("unknown") << "spotify:something:xxxxxxxxx" << SpotifyUtils::Unknown;
}

void TestSpotifyUtils::getUriType()
{
    QFETCH(QString, uri);
    QFETCH(SpotifyUtils::SpotifyType, type);

    qDebug() << SpotifyUtils::getUriType(uri) << type;
    QCOMPARE(SpotifyUtils::getUriType(uri), type);
}

void TestSpotifyUtils::getIdFromUri_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<QString>("id");

    QTest::newRow("album") << "spotify:album:4Io5vWtmV1rFj4yirKb4y4" << "4Io5vWtmV1rFj4yirKb4y4";
    QTest::newRow("album") << "https://open.spotify.com/album/132qAo1cDiEJdA3fv4xyNK?si=8wLG5MJHS8WguOzbP4O36g" << "132qAo1cDiEJdA3fv4xyNK";

    QTest::newRow("playlist") << "spotify:playlist:37i9dQZF1DX4tMcImWolDJ" << "37i9dQZF1DX4tMcImWolDJ";
    QTest::newRow("user playlist") << "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo" << "0Q6hJZYIEu3LwbyBBHjjHo";
    QTest::newRow("user playlist") << "https://open.spotify.com/playlist/2Tegyx2csiWbOhS6lPqRrm?si=0e114fe76a56450e" << "2Tegyx2csiWbOhS6lPqRrm";

    QTest::newRow("track") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << "59WN2psjkt1tyaxjspN8fp";
    QTest::newRow("track") << "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad" << "0W35nxtHtFlseSojmygEsf";

    QTest::newRow("artist") << "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj" << "2d0hyoQ5ynDBnkvAbJKORj";
    QTest::newRow("artist") << "https://open.spotify.com/artist/5M52tdBnJaKSvOpJGz8mfZ?si=avm9TJwVTB2n08LClUq5YA" << "5M52tdBnJaKSvOpJGz8mfZ";

    QTest::newRow("episode") << "spotify:episode:0Q86acNRm6V9GYx55SXKwf" << "0Q86acNRm6V9GYx55SXKwf";
    QTest::newRow("episode") << "https://open.spotify.com/episode/4K9wijPGBjwXjnSiEIkRLy?si=71701a1d55c14e82" << "4K9wijPGBjwXjnSiEIkRLy";

    QTest::newRow("show") << "spotify:show:5CnDmMUG0S5bSSw612fs8C" << "5CnDmMUG0S5bSSw612fs8C";
    QTest::newRow("show") << "https://open.spotify.com/show/6qABy0MzyKURDbwLKZkeK6?si=79e98224367048ef" << "6qABy0MzyKURDbwLKZkeK6";

    QTest::newRow("unknown") << "spotify:something:xxxxxxxxx" << "xxxxxxxxx";
}

void TestSpotifyUtils::getIdFromUri()
{
    QFETCH(QString, uri);
    QFETCH(QString, id);

    QCOMPARE(SpotifyUtils::getIdFromUri(uri), id);
}

void TestSpotifyUtils::getIdFromHref_data()
{
    QTest::addColumn<QString>("href");
    QTest::addColumn<QString>("id");

    QTest::newRow("album") << "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2" << "6akEvsycLGftJxYudPjmqK";
    QTest::newRow("playlist") << "https://api.spotify.com/v1/users/spotify_espa%C3%B1a/playlists/21THa8j9TaSGuXYNBU5tsC/tracks" << "21THa8j9TaSGuXYNBU5tsC";
    QTest::newRow("track") << "https://api.spotify.com/v1/tracks/5TiMM0jscVmWgMwR42RLup" << "5TiMM0jscVmWgMwR42RLup";
    QTest::newRow("artist") << "https://api.spotify.com/v1/artists/2d0hyoQ5ynDBnkvAbJKORj" << "2d0hyoQ5ynDBnkvAbJKORj";
    QTest::newRow("episode") << "https://api.spotify.com/v1/episodes/4K9wijPGBjwXjnSiEIkRLy" << "4K9wijPGBjwXjnSiEIkRLy";
    QTest::newRow("episode") << "https://api.spotify.com/v1/shows/6qABy0MzyKURDbwLKZkeK6" << "6qABy0MzyKURDbwLKZkeK6";
}

void TestSpotifyUtils::getIdFromHref()
{
    QFETCH(QString, href);
    QFETCH(QString, id);

    QCOMPARE(SpotifyUtils::getIdFromHref(href), id);
}

void TestSpotifyUtils::isUri_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("result");

    QTest::newRow("uri") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << true;
    QTest::newRow("uri") << "spotify:playlist:37i9dQZF1DX4tMcImWolDJ" << true;
    QTest::newRow("uri") << "spotify:album:4Io5vWtmV1rFj4yirKb4y4" << true;
    QTest::newRow("uri") << "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj" << true;
    QTest::newRow("href") << "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2" << false;
    QTest::newRow("href") << "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf" << false;
}

void TestSpotifyUtils::isUri()
{
    QFETCH(QString, input);
    QFETCH(bool, result);

    QCOMPARE(SpotifyUtils::isUri(input), result);
}

void TestSpotifyUtils::makeUri_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("uri") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << "spotify:track:59WN2psjkt1tyaxjspN8fp";
    QTest::newRow("href") << "https://api.spotify.com/v1/albums/6akEvsycLGftJxYudPjmqK/tracks?offset=0&limit=2" << "spotify:album:6akEvsycLGftJxYudPjmqK";
    QTest::newRow("href") << "https://open.spotify.com/track/0W35nxtHtFlseSojmygEsf?si=43b918d258c540ad" << "spotify:track:0W35nxtHtFlseSojmygEsf";
}

void TestSpotifyUtils::makeUri()
{
    QFETCH(QString, input);
    QFETCH(QString, result);

    QCOMPARE(SpotifyUtils::makeUri(input), result);
}

QTEST_APPLESS_MAIN(TestSpotifyUtils)
#include "testspotifyutils.moc"
