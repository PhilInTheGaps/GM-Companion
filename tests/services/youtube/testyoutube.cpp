#include "src/services/youtube/youtube.h"
#include "tests/testhelper/staticabstracttest.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

class YouTubeTest : public ::testing::Test
{
public:
    YouTubeTest()
    {
        auto *networkManager = new QNetworkAccessManager();
        networkManager->setTransferTimeout(5000);
        YouTube::instance()->setNetworkManager(networkManager);
    }
};

TEST_F(YouTubeTest, CanGetStreamInfos)
{
    auto future = YouTube::instance()->getStreamInfoAsync(VideoId(u"https://www.youtube.com/watch?v=dQw4w9WgXcQ"_s),
                                                          Option::None);
    StaticAbstractTest::testFutureNoAuth(future, "getAudioStreamAsync", [future]() {
        const auto res = future.result();
        EXPECT_EQ(res.id.toString().toStdString(), "dQw4w9WgXcQ");
        EXPECT_EQ(res.title.toStdString(), "Rick Astley - Never Gonna Give You Up (Official Music Video)");
        EXPECT_NE(res.uploader.toStdString(), "");
        EXPECT_NE(res.thumbnailUrl.toStdString(), "");
        EXPECT_NE(res.audioStreamUrl.toStdString(), "");
    });
}

TEST_F(YouTubeTest, CanGetPlaylistInfos)
{
    auto future = YouTube::instance()->getPlaylistInfoAsync(
        PlaylistId(u"https://www.youtube.com/playlist?list=PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz"_s), Option::None);

    StaticAbstractTest::testFutureNoAuth(future, "getPlaylistInfoAsync", [future]() {
        const auto res = future.result();
        EXPECT_EQ(res.id.toString().toStdString(), "PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz");
        EXPECT_EQ(res.title.toStdString(), "Hades Singles Playlist");
        EXPECT_NE(res.uploader.toStdString(), "");
        EXPECT_NE(res.thumbnailUrl.toStdString(), "");
        EXPECT_FALSE(res.streams.empty());
    });
}

TEST_F(YouTubeTest, CanGetLongPlaylistInfos)
{
    auto future = YouTube::instance()->getPlaylistInfoAsync(
        PlaylistId(u"https://www.youtube.com/playlist?list=PLUckL1YFRK3tiL676pRCURVuZ8RHgu974"_s), Option::None);

    StaticAbstractTest::testFutureNoAuth(future, "getPlaylistInfoAsync", [future]() {
        const auto res = future.result();
        EXPECT_EQ(res.id.toString().toStdString(), "PLUckL1YFRK3tiL676pRCURVuZ8RHgu974");
        EXPECT_GT(res.streams.size(), 100);
    });
}
