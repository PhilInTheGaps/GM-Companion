#include "abstracttest.h"
#include "src/tools/audio/playlist/audioplaylist.h"
#include "src/tools/audio/playlist/audioplaylistfactory.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

class AudioPlaylistTest : public AbstractTest
{
protected:
    void parsePlaylist(const QString &url, AudioPlaylist::Type expectedType)
    {
        {
            QFile projectFile(url);
            EXPECT_TRUE(projectFile.open(QIODevice::ReadOnly)) << "Could not open test playlist file";
            auto data = projectFile.readAll();
            projectFile.close();

            EXPECT_FALSE(data.isEmpty()) << "Playlist file does not contain data";
            auto playlist = AudioPlaylistFactory::build(data, nullptr);

            EXPECT_EQ(playlist->type(), expectedType);
            EXPECT_EQ(playlist->length(), 3);
            EXPECT_FALSE(playlist->isEmpty());

            foreach (const auto *file, playlist->files())
            {
                EXPECT_TRUE(file);
                EXPECT_FALSE(file->url().isEmpty());
            }
        }
    }
};

TEST_F(AudioPlaylistTest, ParsePlaylists)
{
    parsePlaylist(u":/resources/audioplaylist/test.m3u"_s, AudioPlaylist::Type::m3u);
    parsePlaylist(u":/resources/audioplaylist/test.pls"_s, AudioPlaylist::Type::pls);
}

TEST_F(AudioPlaylistTest, CanModify)
{
    AudioPlaylist playlist;

    EXPECT_TRUE(playlist.isEmpty());
    EXPECT_EQ(playlist.length(), 0);

    AudioFile file0(u"/test.mp3"_s, AudioFile::Source::File, u"Test"_s, nullptr);
    playlist.append(&file0);
    EXPECT_FALSE(playlist.isEmpty());
    EXPECT_EQ(playlist.length(), 1);
    EXPECT_EQ(playlist.at(0), &file0);
    EXPECT_EQ(playlist.constFirst(), &file0);

    AudioFile file1(u"/test1.mp3"_s, AudioFile::Source::File, u"Test1"_s, nullptr);
    AudioFile file2(u"/test2.mp3"_s, AudioFile::Source::File, u"Test2"_s, nullptr);
    playlist.setFiles({&file1, &file2});
    EXPECT_EQ(playlist.length(), 2);
    EXPECT_EQ(playlist.at(0), &file1);
    EXPECT_EQ(playlist.at(1), &file2);

    playlist.insert(1, &file0);
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_EQ(playlist.at(1), &file0);
    EXPECT_EQ(playlist.at(2), &file2);

    playlist.clear();
    EXPECT_TRUE(playlist.isEmpty());
    EXPECT_EQ(playlist.length(), 0);
}

TEST_F(AudioPlaylistTest, CanReplace)
{
    AudioPlaylist playlist;
    AudioFile file0(u"/test.mp3"_s, AudioFile::Source::File, u"Test"_s, nullptr);
    AudioFile file1(u"/test1.mp3"_s, AudioFile::Source::File, u"Test1"_s, nullptr);
    AudioFile file2(u"/test2.mp3"_s, AudioFile::Source::File, u"Test2"_s, nullptr);
    AudioFile file3(u"/test3.mp3"_s, AudioFile::Source::File, u"Test3"_s, nullptr);

    playlist.setFiles({&file0, &file1});
    playlist.replace(0, {&file2, &file3});
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_EQ(playlist.at(0), &file2);
    EXPECT_EQ(playlist.at(1), &file3);

    playlist.setFiles({&file0, &file1});
    playlist.replace(1, {&file2, &file3});
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_EQ(playlist.at(1), &file2);
    EXPECT_EQ(playlist.at(2), &file3);
}

TEST_F(AudioPlaylistTest, CanReplaceWithPlaylist)
{
    AudioPlaylist playlist;
    AudioFile file0(u"/test.mp3"_s, AudioFile::Source::File, u"Test"_s, nullptr);
    AudioFile file1(u"/test1.mp3"_s, AudioFile::Source::File, u"Test1"_s, nullptr);
    playlist.setFiles({&file0, &file1});

    AudioPlaylist playlist2;
    AudioFile file2(u"/test2.mp3"_s, AudioFile::Source::File, u"Test2"_s, nullptr);
    AudioFile file3(u"/test3.mp3"_s, AudioFile::Source::File, u"Test3"_s, nullptr);
    playlist2.setFiles({&file2, &file3});

    playlist.replace(0, playlist2);
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_EQ(playlist.at(0), &file2);
    EXPECT_EQ(playlist.at(1), &file3);

    playlist.setFiles({&file0, &file1});
    playlist.replace(1, playlist2);
    EXPECT_EQ(playlist.length(), 3);
    EXPECT_EQ(playlist.at(1), &file2);
    EXPECT_EQ(playlist.at(2), &file3);
}

TEST_F(AudioPlaylistTest, CanShuffle)
{
    AudioPlaylist playlist;
    AudioFile file0(u"/test.mp3"_s, AudioFile::Source::File, u"Test"_s, nullptr);
    AudioFile file1(u"/test1.mp3"_s, AudioFile::Source::File, u"Test1"_s, nullptr);
    AudioFile file2(u"/test2.mp3"_s, AudioFile::Source::File, u"Test2"_s, nullptr);
    AudioFile file3(u"/test3.mp3"_s, AudioFile::Source::File, u"Test3"_s, nullptr);

    playlist.setFiles({&file0, &file1, &file2, &file3});

    for (int i = 0; i < 5; i++)
    {
        playlist.shuffle();

        if (playlist.at(0) != &file0 || playlist.at(1) != &file1 || playlist.at(2) != &file2 ||
            playlist.at(3) != &file3)
        {
            SUCCEED();
            return;
        }
    }

    FAIL() << "Attempted to shuffle the playlist 5 times, the order was always the same";
}
