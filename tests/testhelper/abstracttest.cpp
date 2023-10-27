#include "abstracttest.h"
#include "src/filesystem/file.h"
#include "src/filesystem/fileaccesslocal.h"
#include <QDebug>
#include <QDir>

using namespace Files;

AbstractTest::AbstractTest() : StaticAbstractTest()
{
    tempDir.setAutoRemove(true);
}

void AbstractTest::verifyFileContent(const QString &path, const QByteArray &content, bool cached)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying content of file" << path << "(cached:" << cached << ")";

    const auto future = File::getDataAsync(path, cached, fileAccess);
    testFuture(
        future, "File::getDataAsync",
        [future, content]() {
            ASSERT_FALSE(future.isCanceled()) << "QFuture is canceled!";

            const auto &result = future.result();
            EXPECT_TRUE(result.success())
                << "File::getDataAsync did not return a valid result: " << result.errorMessage().toStdString();
            EXPECT_EQ(result.data(), content);
        },
        cached);
}

void AbstractTest::verifyThatFileExists(const QString &path, bool shouldExist)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying that file" << path << (shouldExist ? "exists" : "does not exist") << "...";

    auto future = File::checkAsync(path, false, fileAccess);
    testFuture(future, "File::checkAsync", [future, path, shouldExist]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_EQ(shouldExist, future.result().exists())
            << "Apparently the file " << path.toStdString() << " does " << (shouldExist ? "not " : "")
            << "exist when it should" << (shouldExist ? "" : " not");
    });
}

auto AbstractTest::getFilePath(const QString &filename) const -> QString
{
    return getFilePathInTempDir(filename, tempDir);
}

void AbstractTest::checkOrCreateFileAccess()
{
    if (!fileAccess)
    {
        fileAccess = std::make_shared<FileAccessLocal>();
    }

    EXPECT_TRUE(fileAccess);
}

/// Moves content of a folder into a temp dir.
/// Returns new path.
auto AbstractTest::backupUserFolder(const QString &userFolder) -> QString
{
    return StaticAbstractTest::backupUserFolder(userFolder, tempDir);
}
