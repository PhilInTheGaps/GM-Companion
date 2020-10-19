#include <QtTest>
#include <QObject>
#include "../../src/utils/fileutils.h"

class TestFileUtils : public QObject
{
    Q_OBJECT
public:
    TestFileUtils() = default;

private slots:
    void dirFromFolders_data();
    void dirFromFolders();

    void dirFromPath_data();
    void dirFromPath();

    void foldersFromDir_data();
    void foldersFromDir();

    void suffix_data();
    void suffix();

    void fileName_data();
    void fileName();

    void incrementFileName_data();
    void incrementFileName();
};

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

void TestFileUtils::dirFromPath_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("relative path") << "some/strange/file.mp3" << "some/strange";
    QTest::newRow("absolute path") << "/some/strange/file.mp3" << "/some/strange";
    QTest::newRow("short path") << "some/file.mp3" << "some";
    QTest::newRow("root path") << "/file.mp3" << "/";
    QTest::newRow("relative short path") << "file.mp3" << "";
}

void TestFileUtils::dirFromPath()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::dirFromPath(input), output);
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

void TestFileUtils::suffix_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("file path") << "some/strange/file.mp3" << "mp3";
    QTest::newRow("only file name") << "file.doc" << "doc";
    QTest::newRow("empty") << "" << "";
    QTest::newRow("multiple dots") << "some/path/file.jpeg.json" << "json";
}

void TestFileUtils::suffix()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::suffix(input), output);
}

void TestFileUtils::fileName_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("file path") << "some/strange/file.mp3" << "file.mp3";
    QTest::newRow("only file name") << "file.doc" << "file.doc";
    QTest::newRow("empty") << "" << "";
    QTest::newRow("multiple dots") << "some/path/file.jpeg.json" << "file.jpeg.json";
}

void TestFileUtils::fileName()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::fileName(input), output);
}

void TestFileUtils::incrementFileName_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("file path without number") << "some/file.mp3" << "some/file_0.mp3";
    QTest::newRow("file path with number") << "some/file_3.mp3" << "some/file_4.mp3";
    QTest::newRow("multiple undescores") << "some/cool_file_2_1.mp3" << "some/cool_file_2_2.mp3";
    QTest::newRow("ends with undescore") << "file_.mp3" << "file_0.mp3";
    QTest::newRow("empty") << "" << "";
    QTest::newRow("long path") << "some/very/strange/file_0.mp3" << "some/very/strange/file_1.mp3";
}

void TestFileUtils::incrementFileName()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::incrementFileName(input), output);
}

QTEST_APPLESS_MAIN(TestFileUtils)
#include "testfileutils.moc"
