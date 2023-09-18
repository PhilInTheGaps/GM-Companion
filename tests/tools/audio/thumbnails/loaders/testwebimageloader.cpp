#include "src/tools/audio/thumbnails/loaders/webimageloader.h"
#include "tests/testhelper/abstracttest.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <gtest/gtest.h>

class WebImageLoaderTest : public AbstractTest
{
public:
    explicit WebImageLoaderTest() : AbstractTest()
    {
        networkManager = std::make_unique<QNetworkAccessManager>(nullptr);
    }
};

TEST_F(WebImageLoaderTest, LoadImageAsync)
{
    const QStringList urls = {"https://sipi.usc.edu/database/preview/textures/1.1.01.png"};

    for (const auto &url : urls)
    {
        const auto future = WebImageLoader::loadImageAsync(url, networkManager.get());

        testFutureNoAuth(future, "loadImageAsync", [future] {
            const auto pixmap = future.result();
            EXPECT_FALSE(pixmap.isNull());
        });
    }
}
