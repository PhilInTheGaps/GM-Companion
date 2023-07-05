#include "utils/fileutils.h"
#include <QObject>
#include <QtTest>

class TestFileUtils : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

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

    void fileInDir_data();
    void fileInDir();
};

void TestFileUtils::dirFromFolders_data()
{
    QTest::addColumn<QStringList>("folders");
    QTest::addColumn<QString>("dir");

    QTest::newRow("relative path") << QStringList({"folder1", "folder2", "folder3"}) << "folder1/folder2/folder3";
    QTest::newRow("absolute path") << QStringList({"", "folder1", "folder2", "folder3"}) << "/folder1/folder2/folder3";
    QTest::newRow("absolute windows path")
        << QStringList({"c:", "folder1", "folder2", "folder3"}) << "c:/folder1/folder2/folder3";
}

void TestFileUtils::dirFromFolders()
{
    QFETCH(QStringList, folders);
    QFETCH(QString, dir);

    QCOMPARE(FileUtils::dirFromFolders(folders), dir);
}

void TestFileUtils::dirFromPath_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("relative path") << "some/strange/file.mp3"
                                   << "some/strange";
    QTest::newRow("absolute path") << "/some/strange/file.mp3"
                                   << "/some/strange";
    QTest::newRow("short path") << "some/file.mp3"
                                << "some";
    QTest::newRow("root path") << "/file.mp3"
                               << "/";
    QTest::newRow("relative short path") << "file.mp3"
                                         << "";
}

void TestFileUtils::dirFromPath()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::dirFromPath(input), output);
}

void TestFileUtils::foldersFromDir_data()
{
    QTest::addColumn<QString>("dir");
    QTest::addColumn<QStringList>("folders");

    QTest::newRow("relative path") << "folder1/folder2/folder3" << QStringList({"folder1", "folder2", "folder3"});
    QTest::newRow("absolute path") << "/folder1/folder2/folder3" << QStringList({"", "folder1", "folder2", "folder3"});
    QTest::newRow("absolute windows path")
        << "c:/folder1/folder2/folder3" << QStringList({"c:", "folder1", "folder2", "folder3"});
}

void TestFileUtils::foldersFromDir()
{
    QFETCH(QString, dir);
    QFETCH(QStringList, folders);

    QCOMPARE(FileUtils::foldersFromDir(dir), folders);
}

void TestFileUtils::suffix_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("file path") << "some/strange/file.mp3"
                               << "mp3";
    QTest::newRow("only file name") << "file.doc"
                                    << "doc";
    QTest::newRow("empty") << ""
                           << "";
    QTest::newRow("multiple dots") << "some/path/file.jpeg.json"
                                   << "json";
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

    QTest::newRow("file path") << "some/strange/file.mp3"
                               << "file.mp3";
    QTest::newRow("only file name") << "file.doc"
                                    << "file.doc";
    QTest::newRow("empty") << ""
                           << "";
    QTest::newRow("multiple dots") << "some/path/file.jpeg.json"
                                   << "file.jpeg.json";
    QTest::newRow("folder with /") << "some/path/"
                                   << "path";
}

void TestFileUtils::fileName()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::fileName(input), output);
    QCOMPARE(FileUtils::fileName(QStringView(input)), output);
}

void TestFileUtils::incrementFileName_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("suffix");

    QTest::newRow("file path without number") << "some/file"
                                              << "some/file_0"
                                              << ".mp3";
    QTest::newRow("file path with number") << "some/file_3"
                                           << "some/file_4"
                                           << ".mp3";
    QTest::newRow("multiple undescores") << "some/cool_file_2_1"
                                         << "some/cool_file_2_2"
                                         << ".mp3";
    QTest::newRow("ends with undescore") << "file_"
                                         << "file_0"
                                         << ".mp3";
    QTest::newRow("empty") << ""
                           << ""
                           << "";
    QTest::newRow("long path") << "some/very/strange/file_0"
                               << "some/very/strange/file_1"
                               << ".mp3";
}

void TestFileUtils::incrementFileName()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, suffix);

    QCOMPARE(FileUtils::incrementName(input), output);
    QCOMPARE(FileUtils::incrementFileName(input + suffix), output + suffix);
}

void TestFileUtils::fileInDir_data()
{
    QTest::addColumn<QString>("dir");
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("output");

    QTest::newRow("with seperator") << "some/path/"
                                    << "test"
                                    << "some/path/test";
    QTest::newRow("with double seperator") << "some/path/"
                                           << "/test"
                                           << "some/path/test";
    QTest::newRow("with double seperator and starting seperator") << "/some/path/"
                                                                  << "/test"
                                                                  << "/some/path/test";
    QTest::newRow("with seperator and leading seperator in file") << "some/path"
                                                                  << "/test"
                                                                  << "some/path/test";
    QTest::newRow("with seperator and trailing seperator in file") << "some/path/"
                                                                   << "test/"
                                                                   << "some/path/test/";
    QTest::newRow("with double seperator and trailing seperator in file") << "some/path/"
                                                                          << "/test/"
                                                                          << "some/path/test/";
    QTest::newRow("without seperator") << "some/path"
                                       << "test"
                                       << "some/path/test";
    QTest::newRow("empty") << ""
                           << ""
                           << "";
    QTest::newRow("empty dir") << ""
                               << "test"
                               << "test";
    QTest::newRow("top level") << "/"
                               << "home"
                               << "/home";
    QTest::newRow("DOS seperator") << "some\\path\\"
                                   << "test"
                                   << "some\\path\\test";
    QTest::newRow("DOS seperator missing") << "some\\path"
                                           << "test"
                                           << "some\\path\\test";
}

void TestFileUtils::fileInDir()
{
    QFETCH(QString, dir);
    QFETCH(QString, filename);
    QFETCH(QString, output);

    QCOMPARE(FileUtils::fileInDir(filename, dir), output);
}

QTEST_APPLESS_MAIN(TestFileUtils)
#include "testfileutils.moc"
