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

    const auto future = File::getDataAsync(path, cached, fileAccess.get());
    testFuture(
        future, "File::getDataAsync",
        [future, content]() {
            EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";

            auto *result = future.result();
            EXPECT_TRUE(result->success()) << "File::getDataAsync did not return a valid result.";
            EXPECT_EQ(result->data(), content);
            result->deleteLater();
        },
        cached);
}

void AbstractTest::verifyThatFileExists(const QString &path, bool shouldExist)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying that file" << path << (shouldExist ? "exists" : "does not exist") << "...";

    auto future = File::checkAsync(path, false, fileAccess.get());
    testFuture(future, "File::checkAsync", [future, path, shouldExist]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(shouldExist == future.result()->exists())
            << "Apparently the file " << path.constData() << " does" << (shouldExist ? " not" : "")
            << "exist when it should" << (shouldExist ? "" : " not");
        future.result()->deleteLater();
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
        fileAccess = std::make_unique<FileAccessLocal>(nullptr);
    }

    EXPECT_TRUE(fileAccess);
}

/// Moves content of a folder into a temp dir.
/// Returns new path.
auto AbstractTest::backupUserFolder(const QString &userFolder) -> QString
{
    return StaticAbstractTest::backupUserFolder(userFolder, tempDir);
}
