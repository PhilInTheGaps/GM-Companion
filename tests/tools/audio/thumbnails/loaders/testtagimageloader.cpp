#include "src/common/utils/fileutils.h"
#include "src/tools/audio/thumbnails/loaders/tagimageloader.h"
#include "tests/testhelper/abstracttest.h"
#include <gtest/gtest.h>

class TagImageLoaderTest : public AbstractTest
{
};

TEST_F(TagImageLoaderTest, LoadFromFile)
{
    const QStringList files = {"test.mp3", "test.ogg", "test.wav"};

    for (const auto &file : files)
    {
        const auto resource = FileUtils::fileInDir(file, ":/resources/audiofiles");
        auto *tempFile = copyResourceToTempFile(resource);

        const auto future = TagImageLoader::loadFromFileAsync(tempFile->fileName(), true);
        testFuture(future, "loadFromFile", [future, tempFile]() {
            tempFile->remove();
            tempFile->deleteLater();
            const auto pixmap = future.result();
            EXPECT_FALSE(pixmap.isNull());
        });
    }
}
