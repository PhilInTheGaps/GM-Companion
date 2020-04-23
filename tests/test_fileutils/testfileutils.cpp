#include <QtTest>
#include "testfileutils.h"
#include "../../src/utils/fileutils.h"

void TestFileUtils::dirFromFolders_data()
{
    QTest::addColumn<QStringList>("folders");
    QTest::addColumn<QString>(    "dir");

    QTest::newRow("relative path") << QStringList({ "folder1", "folder2", "folder3" }) << "folder1/folder2/folder3";
    QTest::newRow("absolute path") << QStringList({ "", "folder1", "folder2", "folder3" }) << "/folder1/folder2/folder3";
    QTest::newRow("absolute windows path") << QStringList({ "c:", "folder1", "folder2", "folder3" }) << "c:/folder1/folder2/folder3";
}

void TestFileUtils::dirFromFolders()
{
    QFETCH(QStringList, folders);
    QFETCH(QString,     dir);

    QCOMPARE(FileUtils::dirFromFolders(folders), dir);
}

void TestFileUtils::foldersFromDir_data()
{
    QTest::addColumn<QString>(    "dir");
    QTest::addColumn<QStringList>("folders");

    QTest::newRow("relative path") << "folder1/folder2/folder3" << QStringList({ "folder1", "folder2", "folder3" });
    QTest::newRow("absolute path") << "/folder1/folder2/folder3" << QStringList({ "", "folder1", "folder2", "folder3" });
    QTest::newRow("absolute windows path") << "c:/folder1/folder2/folder3" << QStringList({ "c:", "folder1", "folder2", "folder3" });
}

void TestFileUtils::foldersFromDir()
{
    QFETCH(QString,     dir);
    QFETCH(QStringList, folders);

    QCOMPARE(FileUtils::foldersFromDir(dir), folders);
}

QTEST_APPLESS_MAIN(TestFileUtils)
