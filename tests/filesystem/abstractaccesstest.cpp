#include "abstractaccesstest.h"
#include "src/filesystem/file.h"
#include "utils/fileutils.h"

using namespace Files;

void AbstractAccessTest::createTestDir()
{
    createDirAndVerify(getFilePath());
}

void AbstractAccessTest::removeTestDir()
{
    removeFileAndVerify(getFilePath());
}

void AbstractAccessTest::createTestFile(const QString &path, const QByteArray &data)
{
    saveFileAndVerify(getFilePath(path), data);
}

void AbstractAccessTest::createTestFiles()
{
    createTestFile(QStringLiteral("test1"), "This is test 1.");
    createTestFile(QStringLiteral("test2"), "This is test 2.");
    createTestFile(QStringLiteral("test3"), "This is test 3.");
    createTestFile(QStringLiteral("test4"), "");
    createTestFile(QStringLiteral("test5"), "To be moved.");
    createTestFile(QStringLiteral("test6"), "To be deleted.");
    createTestFile(QStringLiteral("test7"), "To be copied.");

#ifndef Q_OS_WIN
    // windows can't handle filenames with special characters
    createTestFile(QStringLiteral("file&with\"special characters"), "file&with\"special characters");
#endif
}

void AbstractAccessTest::runAllTests()
{
    getDataAsync();
    saveAsync();
    moveAsync();
    deleteAsync();
    copyAsync();
    listAsync();
    createDirAsync();
    checkAsync();
}

/// Read data from file(s)
void AbstractAccessTest::getDataAsync()
{
    // File that exists
    verifyFileContent(getFilePath(QStringLiteral("test1")), QByteArray("This is test 1."), false);

    // Test file one again, this time it should be in cache
    verifyFileContent(getFilePath(QStringLiteral("test1")), QByteArray("This is test 1."), true);

#ifndef Q_OS_WIN
    // File with special characters in name and content
    verifyFileContent(getFilePath(QStringLiteral("file&with\"special characters")),
                      QByteArray("file&with\"special characters"), false);
#endif

    // File that does not exist
    expectWarning();
    const auto future2 = File::getDataAsync(getFilePath(QStringLiteral("missing-file")), false, fileAccess.get());
    testFuture(future2, "File::getDataAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";

        auto *result = future2.result();
        EXPECT_FALSE(result->success()) << "File::getDataAsync did not return a valid result.";
        result->deleteLater();
    });

    // Multiple files
    const auto future3 = File::getDataAsync({getFilePath(QStringLiteral("test1")), getFilePath(QStringLiteral("test2")),
                                             getFilePath(QStringLiteral("test3"))},
                                            false, fileAccess.get());
    const QList<QByteArray> expectedData = {QByteArray("This is test 1."), QByteArray("This is test 2."),
                                            QByteArray("This is test 3.")};

    testFuture(
        future3, "File::getDataAsync",
        [future3, expectedData]() {
            EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
            auto results = future3.result();

            EXPECT_TRUE(results.size() == expectedData.length())
                << "File::getDataAsync - number of results does not match expected number.";

            int index = 0;
            for (auto *result : results)
            {
                EXPECT_TRUE(result->success()) << "File::getDataAsync did not return a valid result.";
                EXPECT_EQ(result->data(), expectedData[index++]);
                result->deleteLater();
            }
        },
        false);
}

void AbstractAccessTest::saveFileAndVerify(const QString &path, const QByteArray &data)
{
    auto future = File::saveAsync(path, data, fileAccess.get());
    testFuture(future, "File::saveAsync", [this, future, path, data]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not save file.";
        verifyFileContent(path, data);
        future.result()->deleteLater();
    });
}

struct SaveAsyncTest
{
    const char *description;
    QString filename;
    QByteArray data;
};

/// Save a file
void AbstractAccessTest::saveAsync()
{
    std::vector<SaveAsyncTest> tests = {
        {"existing file", QStringLiteral("test4"), QByteArray("File saved.")},
        {"file that did not exist", QStringLiteral("this-file-did-not-exist"), QByteArray("File saved.")},
        {"file in new folder", QStringLiteral("saves/new-file"), QByteArray("File saved.")},
    };

    for (const auto &test : tests)
    {
        saveFileAndVerify(getFilePath(test.filename), test.data);
    }
}

/// Move (rename) a file
void AbstractAccessTest::moveAsync()
{
    // Move existing file
    auto future = File::moveAsync(getFilePath(QStringLiteral("test5")), getFilePath(QStringLiteral("test5-moved")),
                                  fileAccess.get());
    testFuture(future, "File::moveAsync", [this, future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not move file.";
        verifyFileContent(getFilePath(QStringLiteral("test5-moved")), QByteArray("To be moved."));
        future.result()->deleteLater();
    });

    // Move to folder
    auto future2 = File::moveAsync(getFilePath(QStringLiteral("test5-moved")),
                                   getFilePath(QStringLiteral("moves/test1")), fileAccess.get());
    testFuture(future2, "File::moveAsync", [this, future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future2.result()->success()) << "Could not move file.";
        verifyFileContent(getFilePath(QStringLiteral("moves/test1")), QByteArray("To be moved."));
        future2.result()->deleteLater();
    });

    // Move nonexisting file
    expectWarning();
    auto future3 = File::moveAsync(getFilePath(QStringLiteral("test5")), getFilePath(QStringLiteral("test5-moved")),
                                   fileAccess.get());
    testFuture(future3, "File::moveAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future3.result()->success()) << "Could move file but it should not exist.";
        future3.result()->deleteLater();
    });

    // Try to move file to a location already taken
    expectWarning();
    auto future4 = File::moveAsync(getFilePath(QStringLiteral("test5-moved")), getFilePath(QStringLiteral("test1")),
                                   fileAccess.get());
    testFuture(future4, "File::moveAsync", [future4]() {
        EXPECT_FALSE(future4.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future4.result()->success()) << "Could move file but the location already exists.";
        future4.result()->deleteLater();
    });
}

void AbstractAccessTest::removeFileAndVerify(const QString &path)
{
    auto future = File::deleteAsync(path, fileAccess.get());
    testFuture(future, "File::deleteAsync", [this, future, path]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not delete file " << path.constData();
        verifyThatFileExists(path, false);
        future.result()->deleteLater();
    });
}

/// Delete a file
void AbstractAccessTest::deleteAsync()
{
    // Delete existing file
    removeFileAndVerify(getFilePath(QStringLiteral("test6")));

    // Try to delete nonexisting file
    expectWarning();
    auto future = File::deleteAsync(getFilePath(QStringLiteral("test6-missing")), fileAccess.get());
    testFuture(future, "File::deleteAsync", [future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future.result()->success()) << "Could delete file but it does not exist.";
        future.result()->deleteLater();
    });
}

void AbstractAccessTest::copyFileAndVerify(const QString &path, const QString &copy)
{
    auto future = File::copyAsync(path, copy, fileAccess.get());
    testFuture(future, "File::copyAsync", [this, future, path, copy]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success())
            << "Could not copy file " << path.constData() << " to " << copy.constData();
        verifyThatFileExists(copy, true);
        future.result()->deleteLater();
    });
}

/// Copy a file
void AbstractAccessTest::copyAsync()
{
    // Copy existing file
    copyFileAndVerify(getFilePath(QStringLiteral("test7")), getFilePath(QStringLiteral("test7-copy")));

    // Copy nonexisting file
    expectWarning();
    auto future2 = File::copyAsync(getFilePath(QStringLiteral("test7-missing")),
                                   getFilePath(QStringLiteral("test7-copy2")), fileAccess.get());
    testFuture(future2, "File::copyAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future2.result()->success()) << "Could copy file but it does not exist.";
        future2.result()->deleteLater();
    });

    // Try to copy file to exising location
    expectWarning();
    auto future3 = File::copyAsync(getFilePath(QStringLiteral("test7-copy")), getFilePath(QStringLiteral("test1")),
                                   fileAccess.get());
    testFuture(future3, "File::copyAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future3.result()->success()) << "Could copy file but the location already exists.";
        future3.result()->deleteLater();
    });

    // Copy file to folder
    copyFileAndVerify(getFilePath(QStringLiteral("test7")), getFilePath(QStringLiteral("copies/test1")));
}

/// List files inside a directory
void AbstractAccessTest::listAsync()
{
    // List files in folder
    auto future = File::listAsync(getFilePath(), true, false, fileAccess.get());
    testFuture(future, "File::listAsync", [future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_GT(future.result()->files().length(), 2) << "Could not list files.";
        EXPECT_TRUE(future.result()->files().contains("test1")) << "List does not include test file 1.";
        EXPECT_TRUE(future.result()->files().contains("test2")) << "List does not include test file 2.";
#ifndef Q_OS_WIN
        EXPECT_TRUE(future.result()->files().contains("file&with\"special characters"))
            << "List does not include file with special characters in it's name.";
#endif
        EXPECT_FALSE(future.result()->files().contains("copies")) << "File list includes folders.";
        EXPECT_TRUE(future.result()->folders().isEmpty()) << "List includes folders when it should not.";
        future.result()->deleteLater();
    });

    // List files and folders in folder
    auto future2 = File::listAsync(getFilePath(), true, true, fileAccess.get());
    testFuture(future2, "File::listAsync", [this, future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_GT(future2.result()->files().length(), 2) << "Could not list files and folders.";
        EXPECT_TRUE(future2.result()->files().contains("test1")) << "List does not include test file 1.";
        EXPECT_TRUE(future2.result()->files().contains("test2")) << "List does not include test file 2.";
        EXPECT_TRUE(future2.result()->folders().contains("copies")) << "List does not include folders.";
        EXPECT_FALSE(future2.result()->files().contains("copies")) << "File list contains folders.";
        EXPECT_FALSE(future2.result()->folders().contains("test1")) << "Folder list contains files.";

        for (const auto &file : future2.result()->filesFull())
        {
            EXPECT_EQ(FileUtils::dirFromPath(file), getFilePath());
        }

        for (const auto &folder : future2.result()->foldersFull())
        {
            EXPECT_EQ(FileUtils::dirFromPath(folder), getFilePath());
        }

        const auto &filesWithWildcard = future2.result()->filesFull("*1");
        EXPECT_EQ(filesWithWildcard.length(), 1);

        for (const auto &file : filesWithWildcard)
        {
            EXPECT_TRUE(file.endsWith("1"));
        }

        future2.result()->deleteLater();
    });

    // List only folders in folder
    auto future3 = File::listAsync(getFilePath(), false, true, fileAccess.get());
    testFuture(future3, "File::listAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_GT(future3.result()->folders().length(), 1) << "Could not list folders.";
        EXPECT_EQ(future3.result()->folders().length(), 3); // saves, moves, copies
        EXPECT_TRUE(future3.result()->files().isEmpty())
            << "Result contains files when it should only contain folders.";
        EXPECT_TRUE(future3.result()->folders().contains("copies")) << "List does not include folder \"copies\".";
        EXPECT_FALSE(future3.result()->folders().contains("test1"))
            << "List does include files when it should only contain folders.";
        EXPECT_FALSE(future3.result()->files().contains("copies")) << "File list contains folders.";
        EXPECT_FALSE(future3.result()->folders().contains("test1")) << "Folder list contains files.";
        future3.result()->deleteLater();
    });
}

void AbstractAccessTest::createDirAndVerify(const QString &path)
{
    auto future = File::createDirAsync(path, fileAccess.get());
    testFuture(future, "File::createDirAsync", [this, future, path]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Apparently the directory could not be created.";
        verifyThatFileExists(path);
        future.result()->deleteLater();
    });
}

/// Create a directory
void AbstractAccessTest::createDirAsync()
{
    // Create new dir
    createDirAndVerify(getFilePath(QStringLiteral("created_dir")));

    // Try to create dir that already exists
    expectWarning();
    auto future = File::createDirAsync(getFilePath(QStringLiteral("created_dir")), fileAccess.get());
    testFuture(future, "File::createDirAsync", [future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future.result()->success()) << "Apparently a directory that already existed could be created.";
        future.result()->deleteLater();
    });
}

/// Check if files exist
void AbstractAccessTest::checkAsync()
{
    // Check single file
    verifyThatFileExists(getFilePath(QStringLiteral("test1")));

    // Check single file that does not exist
    auto future2 = File::checkAsync(getFilePath(QStringLiteral("test1-missing")), false, fileAccess.get());
    testFuture(future2, "File::checkAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future2.result()->exists()) << "Apparently the file exists when it should not.";
        future2.result()->deleteLater();
    });

    // Check single file
    auto files = QStringList{getFilePath(QStringLiteral("test1")), getFilePath(QStringLiteral("test2")),
                             getFilePath(QStringLiteral("test3")), getFilePath(QStringLiteral("missing-file"))};

    auto future3 = File::checkAsync(files, false, fileAccess.get());
    testFuture(future3, "File::checkAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_EQ(future3.result()->existing().length(), 3) << "Incorrect number of files found.";
        EXPECT_EQ(future3.result()->missing().length(), 1) << "Incorrect number of files not found.";
        future3.result()->deleteLater();
    });
}
