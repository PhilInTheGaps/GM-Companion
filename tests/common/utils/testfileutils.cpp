#include "utils/fileutils.h"
#include <gtest/gtest.h>

struct DirFoldersTest
{
    const char *description;
    QStringList folders;
    QString dir;
};

TEST(FileUtilsTest, DirFromFolders)
{
    std::vector<DirFoldersTest> tests = {
        {"relative path", {"folder1", "folder2", "folder3"}, "folder1/folder2/folder3"},
        {"absolute path", {"", "folder1", "folder2", "folder3"}, "/folder1/folder2/folder3"},
        {"absolute windows path", {"c:", "folder1", "folder2", "folder3"}, "c:/folder1/folder2/folder3"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::dirFromFolders(test.folders), test.dir) << test.description;
    }
}

struct StringIOTest
{
    const char *description;
    QString input;
    QString output;
};

TEST(FileUtilsTest, DirFromPath)
{
    std::vector<StringIOTest> tests = {{"relative path", "some/strange/file.mp3", "some/strange"},
                                       {"absolute path", "/some/strange/file.mp3", "/some/strange"},
                                       {"short path", "some/file.mp3", "some"},
                                       {"root path", "/file.mp3", "/"},
                                       {"relative short path", "file.mp3", ""}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::dirFromPath(test.input), test.output) << test.description;
    }
}

TEST(FileUtilsTest, FoldersFromDir)
{
    std::vector<DirFoldersTest> tests = {
        {"relative path", {"folder1", "folder2", "folder3"}, "folder1/folder2/folder3"},
        {"absolute path", {"", "folder1", "folder2", "folder3"}, "/folder1/folder2/folder3"},
        {"absolute windows path", {"c:", "folder1", "folder2", "folder3"}, "c:/folder1/folder2/folder3"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::foldersFromDir(test.dir), test.folders) << test.description;
    }
}

TEST(FileUtilsTest, Suffix)
{
    std::vector<StringIOTest> tests = {{"file path", "some/strange/file.mp3", "mp3"},
                                       {"only file name", "file.doc", "doc"},
                                       {"empty", "", ""},
                                       {"multiple dots", "some/path/file.jpeg.json", "json"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::suffix(test.input), test.output) << test.description;
    }
}

TEST(FileUtilsTest, FileName)
{
    std::vector<StringIOTest> tests = {{"file path", "some/strange/file.mp3", "file.mp3"},
                                       {"only file name", "file.doc", "file.doc"},
                                       {"empty", "", ""},
                                       {"multiple dots", "some/path/file.jpeg.json", "file.jpeg.json"},
                                       {"folder with /", "some/path/", "path"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::fileName(test.input), test.output) << test.description;
        EXPECT_EQ(FileUtils::fileName(QStringView(test.input)), test.output) << test.description;
    }
}

struct IncFileNameTest
{
    const char *description;
    QString input;
    QString output;
    QString suffix;
};

TEST(FileUtilsTest, IncrementFileName)
{
    std::vector<IncFileNameTest> tests = {
        {"file path without number", "some/file", "some/file_0", ".mp3"},
        {"file path with number", "some/file_3", "some/file_4", ".mp3"},
        {"multiple undescores", "some/cool_file_2_1", "some/cool_file_2_2", ".mp3"},
        {"ends with undescore", "file_", "file_0", ".mp3"},
        {"empty", "", "", ""},
        {"long path", "some/very/strange/file_0", "some/very/strange/file_1", ".mp3"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::incrementName(test.input), test.output) << test.description;
        EXPECT_EQ(FileUtils::incrementFileName(test.input + test.suffix), test.output + test.suffix)
            << test.description;
    }
}

struct FileInDirTest
{
    const char *description;
    QString dir;
    QString filename;
    QString output;
};

TEST(FileUtilsTest, FileInDir)
{
    std::vector<FileInDirTest> tests = {
        {"with seperator", "some/path/", "test", "some/path/test"},
        {"with double seperator", "some/path/", "/test", "some/path/test"},
        {"with double seperator and starting seperator", "/some/path/", "/test", "/some/path/test"},
        {"with seperator and leading seperator in file", "some/path", "/test", "some/path/test"},
        {"with seperator and trailing seperator in file", "some/path/", "test/", "some/path/test/"},
        {"with double seperator and trailing seperator in file", "some/path/", "/test/", "some/path/test/"},
        {"without seperator", "some/path", "test", "some/path/test"},
        {"empty", "", "", ""},
        {"empty dir", "", "test", "test"},
        {"top level", "/", "home", "/home"},
        {"DOS seperator", "some\\path\\", "test", "some\\path\\test"},
        {"DOS seperator missing", "some\\path", "test", "some\\path\\test"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(FileUtils::fileInDir(test.filename, test.dir), test.output) << test.description;
    }
}
