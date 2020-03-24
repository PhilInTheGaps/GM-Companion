#include <QtTest>
#include "testspotify.h"
#include "../../src/services/spotify.h"

void TestSpotify::getUriType_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<int>(    "type");

    QTest::newRow("album") << "spotify:album:4Io5vWtmV1rFj4yirKb4y4" << 0;
    QTest::newRow("playlist") << "spotify:playlist:37i9dQZF1DX4tMcImWolDJ" << 1;
    QTest::newRow("user playlist") << "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo" << 1;
    QTest::newRow("track") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << 2;
    QTest::newRow("artist") << "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj" << 3;
    QTest::newRow("episode") << "spotify:episode:0Q86acNRm6V9GYx55SXKwf" << 4;
    QTest::newRow("show") << "spotify:show:5CnDmMUG0S5bSSw612fs8C" << 5;
    QTest::newRow("unknown") << "spotify:something:xxxxxxxxx" << -1;
}

void TestSpotify::getUriType()
{
    QFETCH(QString, uri);
    QFETCH(int,     type);

    QCOMPARE(Spotify::getUriType(uri), type);
}

void TestSpotify::getId_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<QString>("id");

    QTest::newRow("album") << "spotify:album:4Io5vWtmV1rFj4yirKb4y4" << "4Io5vWtmV1rFj4yirKb4y4";
    QTest::newRow("playlist") << "spotify:playlist:37i9dQZF1DX4tMcImWolDJ" << "37i9dQZF1DX4tMcImWolDJ";
    QTest::newRow("user playlist") << "spotify:user:bezoing:playlist:0Q6hJZYIEu3LwbyBBHjjHo" << "0Q6hJZYIEu3LwbyBBHjjHo";
    QTest::newRow("track") << "spotify:track:59WN2psjkt1tyaxjspN8fp" << "59WN2psjkt1tyaxjspN8fp";
    QTest::newRow("artist") << "spotify:artist:2d0hyoQ5ynDBnkvAbJKORj" << "2d0hyoQ5ynDBnkvAbJKORj";
    QTest::newRow("episode") << "spotify:episode:0Q86acNRm6V9GYx55SXKwf" << "0Q86acNRm6V9GYx55SXKwf";
    QTest::newRow("show") << "spotify:show:5CnDmMUG0S5bSSw612fs8C" << "5CnDmMUG0S5bSSw612fs8C";
    QTest::newRow("unknown") << "spotify:something:xxxxxxxxx" << "xxxxxxxxx";
}

void TestSpotify::getId()
{
    QFETCH(QString, uri);
    QFETCH(QString, id);

    QCOMPARE(Spotify::getId(uri), id);
}

QTEST_APPLESS_MAIN(TestSpotify)
