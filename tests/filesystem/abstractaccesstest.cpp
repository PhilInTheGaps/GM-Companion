#include "abstractaccesstest.h"
#include "file.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include <QtTest>

using namespace Files;
using namespace AsyncFuture;

void AbstractAccessTest::createTestFiles()
{
    createTestFile(QStringLiteral("test1"), "This is test 1.");
    createTestFile(QStringLiteral("test2"), "This is test 2.");
    createTestFile(QStringLiteral("test3"), "This is test 3.");
    createTestFile(QStringLiteral("test4"), "");
    createTestFile(QStringLiteral("test5"), "To be moved.");
    createTestFile(QStringLiteral("test6"), "To be deleted.");
    createTestFile(QStringLiteral("test7"), "To be copied.");
}

/// Read data from file(s)
void AbstractAccessTest::getDataAsync()
{
    // File that exists
    verifyFileContent(getFilePath(QStringLiteral("test1")), QByteArray("This is test 1."), false);

    // Test file one again, this time it should be in cache
    verifyFileContent(getFilePath(QStringLiteral("test1")), QByteArray("This is test 1."), true);

    // File that does not exist
    expectWarning();
    const auto future2 = File::getDataAsync(getFilePath(QStringLiteral("missing-file")), false, fileAccess);
    testFuture(future2, QStringLiteral("File::getDataAsync"), [future2](){
        auto *result = future2.result();
        QVERIFY2(!result->success(), "File::getDataAsync did not return a valid result.");
        result->deleteLater();
    });

    // Multiple files
    const auto future3 = File::getDataAsync({getFilePath(QStringLiteral("test1")),
                                       getFilePath(QStringLiteral("test2")),
                                       getFilePath(QStringLiteral("test3"))},
                                       false, fileAccess);
    const QList<QByteArray> expectedData = {
        QByteArray("This is test 1."),
        QByteArray("This is test 2."),
        QByteArray("This is test 3.")
    };

    testFuture(future3, QStringLiteral("File::getDataAsync"), [future3, expectedData](){
        auto results = future3.result();

        QVERIFY2(results.length() == expectedData.length(), "File::getDataAsync - number of results does not match expected number.");

        int index = 0;
        for (auto *result : results)
        {
            QVERIFY2(result->success(), "File::getDataAsync did not return a valid result.");
            QCOMPARE(result->data(), expectedData[index++]);
            result->deleteLater();
        }
    }, false);
}

void AbstractAccessTest::saveFileAndVerify(const QString &path, const QByteArray &data)
{
    auto future = File::saveAsync(path, data, fileAccess);
    testFuture(future, QStringLiteral("File::saveAsync"), [this, future, path, data](){
        QVERIFY2(future.result()->success(), "Could not save file.");
        verifyFileContent(path, data);
        future.result()->deleteLater();
    });
}

void AbstractAccessTest::saveAsync_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("existing file") << QStringLiteral("test4") << QByteArray("File saved.");
    QTest::newRow("file that did not exist") << QStringLiteral("this-file-did-not-exist") << QByteArray("File saved.");
    QTest::newRow("file in new folder") << QStringLiteral("saves/new-file") << QByteArray("File saved.");
}

/// Save a file
void AbstractAccessTest::saveAsync()
{
    QFETCH(QString, filename);
    QFETCH(QByteArray, data);

    saveFileAndVerify(getFilePath(filename), data);
}

/// Move (rename) a file
void AbstractAccessTest::moveAsync()
{
    // Move existing file
    auto future = File::moveAsync(getFilePath(QStringLiteral("test5")), getFilePath(QStringLiteral("test5-moved")), fileAccess);
    testFuture(future, QStringLiteral("File::moveAsync"), [this, future](){
        QVERIFY2(future.result()->success(), "Could not move file.");
        verifyFileContent(getFilePath(QStringLiteral("test5-moved")), QByteArray("To be moved."));
        future.result()->deleteLater();
    });


    // Move to folder
    auto future2 = File::moveAsync(getFilePath(QStringLiteral("test5-moved")), getFilePath(QStringLiteral("moves/test1")), fileAccess);
    testFuture(future2, QStringLiteral("File::moveAsync"), [this, future2](){
        QVERIFY2(future2.result()->success(), "Could not move file.");
        verifyFileContent(getFilePath(QStringLiteral("moves/test1")), QByteArray("To be moved."));
        future2.result()->deleteLater();
    });

    // Move nonexisting file
    expectWarning();
    auto future3 = File::moveAsync(getFilePath(QStringLiteral("test5")), getFilePath(QStringLiteral("test5-moved")), fileAccess);
    testFuture(future3, QStringLiteral("File::moveAsync"), [future3](){
        QVERIFY2(!future3.result()->success(), "Could move file but it should not exist.");
        future3.result()->deleteLater();
    });

    // Try to move file to a location already taken
    expectWarning();
    auto future4 = File::moveAsync(getFilePath(QStringLiteral("test5-moved")), getFilePath(QStringLiteral("test1")), fileAccess);
    testFuture(future4, QStringLiteral("File::moveAsync"), [future4](){
        QVERIFY2(!future4.result()->success(), "Could move file but the location already exists.");
        future4.result()->deleteLater();
    });
}

void AbstractAccessTest::removeFileAndVerify(const QString &path)
{
    auto future = File::deleteAsync(path, fileAccess);
    testFuture(future, QStringLiteral("File::deleteAsync"), [this, future, path](){
        QVERIFY2(future.result()->success(), QString("Could not delete file %1.").arg(path).toUtf8());
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
    auto future = File::deleteAsync(getFilePath(QStringLiteral("test6-missing")), fileAccess);
    testFuture(future, QStringLiteral("File::deleteAsync"), [future](){
        QVERIFY2(!future.result()->success(), "Could delete file but it does not exist.");
        future.result()->deleteLater();
    });

}

void AbstractAccessTest::copyFileAndVerify(const QString &path, const QString &copy)
{
    auto future = File::copyAsync(path, copy, fileAccess);
    testFuture(future, QStringLiteral("File::copyAsync"), [this, future, path, copy](){
        QVERIFY2(future.result()->success(), QString("Could not copy file %1 to %2").arg(path, copy).toUtf8());
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
    auto future2 = File::copyAsync(getFilePath(QStringLiteral("test7-missing")), getFilePath(QStringLiteral("test7-copy2")), fileAccess);
    testFuture(future2, QStringLiteral("File::copyAsync"), [future2](){
        QVERIFY2(!future2.result()->success(), "Could copy file but it does not exist.");
        future2.result()->deleteLater();
    });

    // Try to copy file to exising location
    expectWarning();
    auto future3 = File::copyAsync(getFilePath(QStringLiteral("test7-copy")), getFilePath(QStringLiteral("test1")), fileAccess);
    testFuture(future3, QStringLiteral("File::copyAsync"), [future3](){
        QVERIFY2(!future3.result()->success(), "Could copy file but the location already exists.");
        future3.result()->deleteLater();
    });

    // Copy file to folder
    copyFileAndVerify(getFilePath(QStringLiteral("test7")), getFilePath(QStringLiteral("copies/test1")));
}

/// List files inside a directory
void AbstractAccessTest::listAsync()
{
    // List files in folder
    auto future = File::listAsync(getFilePath(), true, false, fileAccess);
    testFuture(future, QStringLiteral("File::listAsync"), [future](){
        QVERIFY2(future.result()->files().length() > 2, "Could not list files.");
        QVERIFY2(future.result()->files().contains("test1"), "List does not include test file 1.");
        QVERIFY2(future.result()->files().contains("test2"), "List does not include test file 2.");
        QVERIFY2(!future.result()->files().contains("copies"), "File list includes folders.");
        QVERIFY2(future.result()->folders().isEmpty(), "List includes folders when it should not.");
        future.result()->deleteLater();
    });

    // List files and folders in folder
    auto future2 = File::listAsync(getFilePath(), true, true, fileAccess);
    testFuture(future2, QStringLiteral("File::listAsync"), [this, future2](){
        QVERIFY2(future2.result()->files().length() > 2, "Could not list files and folders.");
        QVERIFY2(future2.result()->files().contains("test1"), "List does not include test file 1.");
        QVERIFY2(future2.result()->files().contains("test2"), "List does not include test file 2.");
        QVERIFY2(future2.result()->folders().contains("copies"), "List does not include folders.");
        QVERIFY2(!future2.result()->files().contains("copies"), "File list contains folders.");
        QVERIFY2(!future2.result()->folders().contains("test1"), "Folder list contains files.");

        for (const auto &file : future2.result()->filesFull())
        {
            QVERIFY(FileUtils::dirFromPath(file) == getFilePath());
        }

        future2.result()->deleteLater();
    });


    // List only folders in folder
    auto future3 = File::listAsync(getFilePath(), false, true, fileAccess);
    testFuture(future3, QStringLiteral("File::listAsync"), [future3](){
        QVERIFY2(future3.result()->folders().length() > 1, "Could not list folders.");
        QVERIFY2(future3.result()->files().isEmpty(), "Result contains files when it should only contain folders.");
        QVERIFY2(future3.result()->folders().contains("copies"), "List does not include folder \"copies\".");
        QVERIFY2(!future3.result()->folders().contains("test1"), "List does include files when it should only contain folders.");
        QVERIFY2(!future3.result()->files().contains("copies"), "File list contains folders.");
        QVERIFY2(!future3.result()->folders().contains("test1"), "Folder list contains files.");
        future3.result()->deleteLater();
    });
}

void AbstractAccessTest::createDirAndVerify(const QString &path)
{
    auto future = File::createDirAsync(path, fileAccess);
    testFuture(future, QStringLiteral("File::createDirAsync"), [this, future, path](){
        QVERIFY2(future.result()->success(), "Apparently the directory could not be created.");
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
    auto future = File::createDirAsync(getFilePath(QStringLiteral("created_dir")), fileAccess);
    testFuture(future, QStringLiteral("File::createDirAsync"), [future](){
        QVERIFY2(!future.result()->success(), "Apparently a directory that already existed could be created.");
        future.result()->deleteLater();
    });
}

/// Check if files exist
void AbstractAccessTest::checkAsync()
{
    // Check single file
    verifyThatFileExists(getFilePath(QStringLiteral("test1")));

    // Check single file that does not exist
    auto future2 = File::checkAsync(getFilePath(QStringLiteral("test1-missing")), false, fileAccess);
    testFuture(future2, QStringLiteral("File::checkAsync"), [future2](){
        QVERIFY2(!future2.result()->exists(), "Apparently the file exists when it should not.");
        future2.result()->deleteLater();
    });

    // Check single file
    auto files = QStringList {
            getFilePath(QStringLiteral("test1")),
            getFilePath(QStringLiteral("test2")),
            getFilePath(QStringLiteral("test3")),
            getFilePath(QStringLiteral("missing-file"))
    };

    auto future3 = File::checkAsync(files, false, fileAccess);
    testFuture(future3, QStringLiteral("File::checkAsync"), [future3](){
        QVERIFY2(future3.result()->existing().length() == 3, "Incorrect number of files found.");
        QVERIFY2(future3.result()->missing().length() == 1, "Incorrect number of files not found.");
        future3.result()->deleteLater();
    });
}
