#include "abstractaccesstest.h"
#include "src/filesystem/file.h"
#include "utils/fileutils.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;

AbstractAccessTest::AbstractAccessTest()
{
    File::init(nullptr, nullptr);
}

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
    createTestFile(u"test1"_s, "This is test 1.");
    createTestFile(u"test2"_s, "This is test 2.");
    createTestFile(u"test3"_s, "This is test 3.");
    createTestFile(u"test4"_s, "");
    createTestFile(u"test5"_s, "To be moved.");
    createTestFile(u"test6"_s, "To be deleted.");
    createTestFile(u"test7"_s, "To be copied.");

#ifndef Q_OS_WIN
    // windows can't handle filenames with special characters
    createTestFile(u"file&with\"special characters"_s, "file&with\"special characters");
#endif

    createDirAndVerify(getFilePath(u"testdirempty"_s));
    createDirAndVerify(getFilePath(u"testdirwithfile"_s));
    saveFileAndVerify(getFilePath(FileUtils::fileInDir(u"file"_s, u"testdirwithfile"_s)), "");
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
    verifyFileContent(getFilePath(u"test1"_s), QByteArray("This is test 1."), false);

    // Test file one again, this time it should be in cache
    verifyFileContent(getFilePath(u"test1"_s), QByteArray("This is test 1."), true);

#ifndef Q_OS_WIN
    // File with special characters in name and content
    verifyFileContent(getFilePath(u"file&with\"special characters"_s), QByteArray("file&with\"special characters"),
                      false);
#endif

    // File that does not exist
    expectWarning();
    const auto future2 = File::getDataAsync(getFilePath(u"missing-file"_s), false, fileAccess);
    testFuture(future2, "File::getDataAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";

        const auto &result = future2.result();
        EXPECT_FALSE(result->success()) << "File::getDataAsync did not return a valid result.";
    });

    // Multiple files
    const auto future3 = File::getDataAsync({getFilePath(u"test1"_s), getFilePath(u"test2"_s), getFilePath(u"test3"_s)},
                                            false, fileAccess);
    const QList<QByteArray> expectedData = {QByteArray("This is test 1."), QByteArray("This is test 2."),
                                            QByteArray("This is test 3.")};

    testFuture(
        future3, "File::getDataAsync",
        [future3, expectedData]() {
            EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
            auto results = future3.result();

            EXPECT_EQ(results.size(), expectedData.length())
                << "File::getDataAsync - number of results does not match expected number.";

            int index = 0;
            for (const auto &result : results)
            {
                EXPECT_TRUE(result->success()) << "File::getDataAsync did not return a valid result.";
                EXPECT_EQ(result->data(), expectedData[index++]);
            }
        },
        false);
}

void AbstractAccessTest::saveFileAndVerify(const QString &path, const QByteArray &data)
{
    auto future = File::saveAsync(path, data, fileAccess);
    testFuture(future, "File::saveAsync", [this, future, path, data]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not save file.";
        verifyFileContent(path, data);
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
        {"existing file", u"test4"_s, QByteArray("File saved.")},
        {"file that did not exist", u"this-file-did-not-exist"_s, QByteArray("File saved.")},
        {"file in new folder", u"saves/new-file"_s, QByteArray("File saved.")},
        {"file in new folders", u"saves/a/b/c/new-file"_s, QByteArray("File saved.")},
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
    auto future = File::moveAsync(getFilePath(u"test5"_s), getFilePath(u"test5-moved"_s), fileAccess);
    testFuture(future, "File::moveAsync", [this, future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not move file test5 to test5-moved";
        verifyFileContent(getFilePath(u"test5-moved"_s), "To be moved.");
    });

    // Move to folder
    auto future2 = File::moveAsync(getFilePath(u"test5-moved"_s), getFilePath(u"moves/test1"_s), fileAccess);
    testFuture(future2, "File::moveAsync", [this, future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future2.result()->success()) << "Could not move file test5-moved to moves/test1";
        verifyFileContent(getFilePath(u"moves/test1"_s), "To be moved.");
    });

    // Move nonexisting file
    expectWarning();
    auto future3 = File::moveAsync(getFilePath(u"test5"_s), getFilePath(u"test5-moved"_s), fileAccess);
    testFuture(future3, "File::moveAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future3.result()->success())
            << "Could move file test5 to test5-moved (again) but it should no longer exist.";
    });

    // Try to move file to a location already taken
    expectWarning();
    auto future4 = File::moveAsync(getFilePath(u"test5-moved"_s), getFilePath(u"test1"_s), fileAccess);
    testFuture(future4, "File::moveAsync", [future4]() {
        EXPECT_FALSE(future4.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future4.result()->success())
            << "Could move file test5-moved to test1 but the location already exists.";
    });
}

void AbstractAccessTest::removeFileAndVerify(const QString &path)
{
    auto future = File::deleteAsync(path, fileAccess);
    testFuture(future, "File::deleteAsync", [this, future, path]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Could not delete file " << path.toStdString();
        verifyThatFileExists(path, false);
    });
}

/// Delete a file
void AbstractAccessTest::deleteAsync()
{
    // Delete existing file
    removeFileAndVerify(getFilePath(u"test6"_s));

    // Try to delete nonexisting file
    expectWarning();
    auto future = File::deleteAsync(getFilePath(u"test6-missing"_s), fileAccess);
    testFuture(future, "File::deleteAsync", [future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future.result()->success()) << "Could delete file but it does not exist.";
    });

    // Delete empty dir
    removeFileAndVerify(getFilePath(u"testdirempty"_s));

    // Delete non-empty dir
    removeFileAndVerify(getFilePath(u"testdirwithfile"_s));
}

void AbstractAccessTest::copyFileAndVerify(const QString &path, const QString &copy)
{
    auto future = File::copyAsync(path, copy, fileAccess);
    testFuture(future, "File::copyAsync", [this, future, path, copy]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success())
            << "Could not copy file " << path.toStdString() << " to " << copy.toStdString();
        verifyThatFileExists(copy, true);
    });
}

/// Copy a file
void AbstractAccessTest::copyAsync()
{
    // Copy existing file
    copyFileAndVerify(getFilePath(u"test7"_s), getFilePath(u"test7-copy"_s));

    // Copy nonexisting file
    expectWarning();
    auto future2 = File::copyAsync(getFilePath(u"test7-missing"_s), getFilePath(u"test7-copy2"_s), fileAccess);
    testFuture(future2, "File::copyAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future2.result()->success()) << "Could copy file but it does not exist.";
    });

    // Try to copy file to exising location
    expectWarning();
    auto future3 = File::copyAsync(getFilePath(u"test7-copy"_s), getFilePath(u"test1"_s), fileAccess);
    testFuture(future3, "File::copyAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future3.result()->success()) << "Could copy file but the location already exists.";
    });

    // Copy file to folder
    copyFileAndVerify(getFilePath(u"test7"_s), getFilePath(u"copies/test1"_s));
}

/// List files inside a directory
void AbstractAccessTest::listAsync()
{
    // List files in folder
    auto future = File::listAsync(getFilePath(), true, false, fileAccess);
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
    });

    // List files and folders in folder
    auto future2 = File::listAsync(getFilePath(), true, true, fileAccess);
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

        const auto &filesWithWildcard = future2.result()->filesFull(u"*1"_s);
        EXPECT_EQ(filesWithWildcard.length(), 1);

        for (const auto &file : filesWithWildcard)
        {
            EXPECT_TRUE(file.endsWith("1"_L1));
        }
    });

    // List only folders in folder
    auto future3 = File::listAsync(getFilePath(), false, true, fileAccess);
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
    });
}

void AbstractAccessTest::createDirAndVerify(const QString &path)
{
    auto future = File::createDirAsync(path, fileAccess);
    testFuture(future, "File::createDirAsync", [this, future, path]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_TRUE(future.result()->success()) << "Apparently the directory could not be created.";
        verifyThatFileExists(path);
    });
}

/// Create a directory
void AbstractAccessTest::createDirAsync()
{
    // Create new dir
    createDirAndVerify(getFilePath(u"created_dir"_s));

    // Try to create dir that already exists
    expectWarning();
    auto future = File::createDirAsync(getFilePath(u"created_dir"_s), fileAccess);
    testFuture(future, "File::createDirAsync", [future]() {
        EXPECT_FALSE(future.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future.result()->success()) << "Apparently a directory that already existed could be created.";
    });
}

/// Check if files exist
void AbstractAccessTest::checkAsync()
{
    // Check single file
    verifyThatFileExists(getFilePath(u"test1"_s));

    // Check single file that does not exist
    auto future2 = File::checkAsync(getFilePath(u"test1-missing"_s), false, fileAccess);
    testFuture(future2, "File::checkAsync", [future2]() {
        EXPECT_FALSE(future2.isCanceled()) << "QFuture is canceled!";
        EXPECT_FALSE(future2.result()->exists()) << "Apparently the file exists when it should not.";
    });

    // Check single file
    auto files = QStringList{getFilePath(u"test1"_s), getFilePath(u"test2"_s), getFilePath(u"test3"_s),
                             getFilePath(u"missing-file"_s)};

    auto future3 = File::checkAsync(files, false, fileAccess);
    testFuture(future3, "File::checkAsync", [future3]() {
        EXPECT_FALSE(future3.isCanceled()) << "QFuture is canceled!";
        EXPECT_EQ(future3.result()->existing().length(), 3) << "Incorrect number of files found.";
        EXPECT_EQ(future3.result()->missing().length(), 1) << "Incorrect number of files not found.";
    });
}
