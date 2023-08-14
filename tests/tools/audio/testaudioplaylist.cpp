#include "src/tools/audio/playlist/audioplaylist.h"
#include "tests/testhelper/abstracttest.h"
#include <QObject>
#include <QtTest>

class TestAudioPlaylist : public AbstractTest
{
    Q_OBJECT
public:
    using AbstractTest::AbstractTest;

private:
    void parsePlaylist(const QString &url, AudioPlaylist::Type expectedType);

private slots:
    void parsePlaylists();
};

void TestAudioPlaylist::parsePlaylists()
{
    parsePlaylist(u":/resources/audioplaylist/test.m3u"_s, AudioPlaylist::Type::m3u);
    parsePlaylist(u":/resources/audioplaylist/test.pls"_s, AudioPlaylist::Type::pls);
}

void TestAudioPlaylist::parsePlaylist(const QString &url, AudioPlaylist::Type expectedType)
{
    QFile projectFile(url);
    QVERIFY2(projectFile.open(QIODevice::ReadOnly), "Could not open test playlist file");
    auto data = projectFile.readAll();
    projectFile.close();

    QVERIFY2(!data.isEmpty(), "Playlist file does not contain data");
    const AudioPlaylist playlist(data, this);

    QCOMPARE(playlist.type(), expectedType);
    QCOMPARE(playlist.length(), 3);
    QVERIFY(!playlist.isEmpty());

    foreach (const auto *file, playlist.files())
    {
        QVERIFY(file);
        QVERIFY(!file->url().isEmpty());
    }
}

QTEST_APPLESS_MAIN(TestAudioPlaylist)
#include "testaudioplaylist.moc"
