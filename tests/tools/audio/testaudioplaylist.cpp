#include "src/tools/audio/playlist/audioplaylist.h"
#include "tests/testhelper/abstracttest.h"

class AudioPlaylistTest : public AbstractTest
{
protected:
    static void parsePlaylist(const QString &url, AudioPlaylist::Type expectedType);
};

void AudioPlaylistTest::parsePlaylist(const QString &url, AudioPlaylist::Type expectedType)
{
    QFile projectFile(url);
    EXPECT_TRUE(projectFile.open(QIODevice::ReadOnly)) << "Could not open test playlist file";
    auto data = projectFile.readAll();
    projectFile.close();

    EXPECT_FALSE(data.isEmpty()) << "Playlist file does not contain data";
    const AudioPlaylist playlist(data, nullptr);

    EXPECT_EQ(playlist.type(), expectedType);
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_FALSE(playlist.isEmpty());

    foreach (const auto *file, playlist.files())
    {
        EXPECT_TRUE(file);
        EXPECT_FALSE(file->url().isEmpty());
    }
}

TEST_F(AudioPlaylistTest, ParsePlaylists)
{
    parsePlaylist(u":/resources/audioplaylist/test.m3u"_s, AudioPlaylist::Type::m3u);
    parsePlaylist(u":/resources/audioplaylist/test.pls"_s, AudioPlaylist::Type::pls);
}
