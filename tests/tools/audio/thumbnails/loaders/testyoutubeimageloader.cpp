#include "services/youtube/youtube.h"
#include "src/tools/audio/thumbnails/loaders/youtubeimageloader.h"
#include "tests/testhelper/staticabstracttest.h"
#include <QNetworkAccessManager>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto makeNetworkManager() -> QNetworkAccessManager *
{
    auto *networkManager = new QNetworkAccessManager();
    networkManager->setTransferTimeout(5000);
    YouTube::instance()->setNetworkManager(networkManager);
    return networkManager;
}

TEST(YouTubeImageLoaderTest, CanNotLoadNullVideoImage)
{
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(VideoId(), makeNetworkManager()).isCanceled());
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(YouTubeVideo(), makeNetworkManager()).isCanceled());
}

TEST(YouTubeImageLoaderTest, CanLoadVideoImage)
{
    const auto future = YouTubeImageLoader::loadImageAsync(VideoId(u"https://www.youtube.com/watch?v=dQw4w9WgXcQ"_s),
                                                           makeNetworkManager());

    StaticAbstractTest::testFutureNoAuth(future, "loadImageAsync", [future] {
        EXPECT_TRUE(future.isFinished());
        EXPECT_GT(future.resultCount(), 0);
        const auto pixmap = future.result();
        EXPECT_FALSE(pixmap.isNull());
    });

    // should be in cache now
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(VideoId(u"https://www.youtube.com/watch?v=dQw4w9WgXcQ"_s),
                                                   makeNetworkManager())
                    .isFinished());
}

TEST(YouTubeImageLoaderTest, CanNotLoadNullPlaylistImage)
{
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(PlaylistId(), makeNetworkManager()).isCanceled());
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(YouTubePlaylist(), makeNetworkManager()).isCanceled());
}

TEST(YouTubeImageLoaderTest, CanLoadPlaylistImage)
{
    const auto future = YouTubeImageLoader::loadImageAsync(
        PlaylistId(u"https://www.youtube.com/playlist?list=PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz"_s),
        makeNetworkManager());

    StaticAbstractTest::testFutureNoAuth(future, "loadImageAsync", [future] {
        EXPECT_GT(future.resultCount(), 0);
        const auto pixmap = future.result();
        EXPECT_FALSE(pixmap.isNull());
    });

    // should be in cache now
    EXPECT_TRUE(YouTubeImageLoader::loadImageAsync(
                    PlaylistId(u"https://www.youtube.com/playlist?list=PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz"_s),
                    makeNetworkManager())
                    .isFinished());
}
