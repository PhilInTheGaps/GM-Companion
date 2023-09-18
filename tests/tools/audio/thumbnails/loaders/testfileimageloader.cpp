#include "src/filesystem/fileaccesslocal.h"
#include "src/tools/audio/thumbnails/loaders/fileimageloader.h"
#include "tests/testhelper/abstracttest.h"
#include "utils/fileutils.h"
#include <QObject>
#include <QStringList>
#include <gtest/gtest.h>

class FileImageLoaderTest : public AbstractTest
{
public:
    explicit FileImageLoaderTest() : AbstractTest()
    {
        fileAccess = std::make_unique<Files::FileAccessLocal>(nullptr);
        Files::FileAccess::setInstance(fileAccess.get());
    }
};

TEST_F(FileImageLoaderTest, LoadImageAsync)
{
    const QStringList files = {"test.png", "test.jpg", "test.tif", "test.gif"};

    for (const auto &file : files)
    {
        const auto resource = FileUtils::fileInDir(file, ":/resources/images");
        auto *tempFile = copyResourceToTempFile(resource);

        const auto future = FileImageLoader::loadImageAsync(tempFile->fileName());
        testFuture(future, "loadImageAsync", [future, tempFile]() {
            tempFile->remove();
            tempFile->deleteLater();

            const auto pixmap = future.result();
            EXPECT_FALSE(pixmap.isNull());
        });
    }
}
