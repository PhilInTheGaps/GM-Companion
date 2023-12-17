#include "abstracttest.h"
#include "settings/settingsmanager.h"
#include "src/filesystem/file.h"
#include "src/filesystem/fileaccesslocal.h"
#include <QDebug>
#include <QDir>

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Common::Settings;

AbstractTest::AbstractTest() : StaticAbstractTest()
{
    cloudMode = SettingsManager::instance()->get(u"cloudMode"_s, u"local"_s);
    SettingsManager::instance()->set(u"cloudMode"_s, u"local"_s);

    tempDir.setAutoRemove(true);
}

AbstractTest::~AbstractTest()
{
    SettingsManager::instance()->set(u"cloudMode"_s, cloudMode);
}

void AbstractTest::verifyFileContent(const QString &path, const QByteArray &content, Options options)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying content of file" << path << "(cached:" << options << ")";

    const auto future = File::getDataAsync(path, options, fileAccess);
    testFuture(
        future, "File::getDataAsync",
        [future, content]() {
            ASSERT_FALSE(future.isCanceled()) << "QFuture is canceled!";

            const auto &result = future.result();
            EXPECT_TRUE(result.success())
                << "File::getDataAsync did not return a valid result: " << result.errorMessage().toStdString();
            EXPECT_EQ(result.data(), content);
        },
        options);
}

void AbstractTest::verifyThatFileExists(const QString &path, bool shouldExist)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying that file" << path << (shouldExist ? "exists" : "does not exist") << "...";

    auto future = File::checkAsync(path, Option::None, fileAccess);
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
