#include "app/ui/FileDialog/filedialogbackend.h"
#include "app/ui/FileDialog/fileobject.h"
#include "src/filesystem/file.h"
#include "src/filesystem/fileaccess.h"
#include "src/filesystem/fileaccesslocal.h"
#include <QList>
#include <QSignalSpy>
#include <functional>
#include <gtest/gtest.h>
#include <memory>

using namespace Files;
using namespace Qt::Literals::StringLiterals;

class FileDialogTest : public ::testing::Test
{
public:
    FileDialogTest()
    {
        fileAccess = std::make_unique<FileAccessLocal>(nullptr);
        FileAccess::setInstance(fileAccess.get());

        createTestFiles();

        dialog = std::make_unique<FileDialogBackend>(nullptr);
    }

    ~FileDialogTest()
    {
        tempDir.remove();
    }

protected:
    void enterAndGetTopLevel();
    void enterTopLevelFoldersOnly();
    void moveThroughStructure();

private:
    QTemporaryDir tempDir;
    std::unique_ptr<FileDialogBackend> dialog = nullptr;
    std::unique_ptr<FileAccessLocal> fileAccess = nullptr;

    static constexpr int WAIT_TIME_MS = 1000;
    const QStringList topLevelFiles = {"file1", "file2", "file3", "file4"};
    const QStringList topLevelFolders = {"dir1", "dir2", "dir3", "dir4"};

    int previousEntryCount = 0;

    void createFile(const QString &file);
    void createDir(const QString &directory);
    void createTestFiles();
    void waitForSignals(QSignalSpy &spy, int count, const QString &dir);

    void enterDir(const std::function<void()> &enter, const QString &dir, bool canGoForward, bool canGoBack,
                  const QStringList &expectedFiles, const QStringList &expectedFolders);

    void enterDir(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                  const QStringList &expectedFolders);

    void enterDir(int index, const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                  const QStringList &expectedFolders);

    void goForward(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                   const QStringList &expectedFolders);

    void goBack(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                const QStringList &expectedFolders);
};

void FileDialogTest::createFile(const QString &file)
{
    auto future = File::saveAsync(tempDir.filePath(file), QByteArray());
    future.waitForFinished();
}

void FileDialogTest::createDir(const QString &directory)
{
    auto future = File::createDirAsync(tempDir.filePath(directory));
    future.waitForFinished();
}

void FileDialogTest::createTestFiles()
{
    for (const auto &dirName : topLevelFolders)
    {
        createDir(dirName);
    }

    createDir(u"dir1/dir1_1/dir1_1_1"_s);
    createDir(u"dir1/dir1_1/dir1_1_2"_s);
    createDir(u"dir1/dir1_2/dir1_2_1"_s);

    for (const auto &fileName : topLevelFiles)
    {
        createFile(fileName);
    }
}

void FileDialogTest::waitForSignals(QSignalSpy &spy, int count, const QString &dir)
{
    while (spy.count() < count)
    {
        EXPECT_TRUE(spy.wait(WAIT_TIME_MS))
            << "Timeout at signal " << spy.count() << "/" << count << " (entering " << dir.constData();
    }
}

void FileDialogTest::enterDir(const std::function<void()> &enter, const QString &dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    QSignalSpy dirChangeSpy(dialog.get(), &FileDialogBackend::currentDirChanged);
    QSignalSpy forwardChangeSpy(dialog.get(), &FileDialogBackend::canGoForwardChanged);
    QSignalSpy backChangeSpy(dialog.get(), &FileDialogBackend::canGoBackChanged);
    QSignalSpy filesChangeSpy(dialog.get(), &FileDialogBackend::entriesChanged);
    QSignalSpy loadingSpy(dialog.get(), &FileDialogBackend::isLoadingChanged);

    enter();

    // 2 signals: once for clearing list, second for new files
    int expectedFileCount = expectedFiles.count() + expectedFolders.count();
    int expectedSignals = previousEntryCount == 0 ? 0 : 1;
    expectedSignals += expectedFileCount != 0 ? 1 : 0;

    waitForSignals(filesChangeSpy, expectedSignals, dir);
    EXPECT_EQ(filesChangeSpy.count(), expectedSignals);

    waitForSignals(dirChangeSpy, 1, dir);
    EXPECT_EQ(dialog->currentDir(), dir);
    EXPECT_EQ(dialog->currentDir(), dirChangeSpy.takeFirst().constFirst().toString());

    waitForSignals(forwardChangeSpy, 1, dir);
    EXPECT_EQ(dialog->canGoForward(), canGoForward);

    waitForSignals(backChangeSpy, 1, dir);
    EXPECT_EQ(dialog->canGoBack(), canGoBack);

    waitForSignals(loadingSpy, 2, dir);
    EXPECT_FALSE(dialog->isLoading());

    QStringList files;
    QStringList folders;

    foreach (auto *fileObject, dialog->entries())
    {
        EXPECT_TRUE(fileObject);

        if (fileObject->isFolder())
        {
            folders << fileObject->name();
            EXPECT_TRUE(expectedFolders.contains(fileObject->name()));
        }
        else
        {
            files << fileObject->name();
            EXPECT_TRUE(expectedFiles.contains(fileObject->name()));
        }
    }

    EXPECT_EQ(files.length(), expectedFiles.length());
    EXPECT_EQ(folders.length(), expectedFolders.length());
    previousEntryCount = files.count() + folders.count();
}

void FileDialogTest::enterDir(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                              const QStringList &expectedFolders)
{
    auto enter = [this, dir]() { dialog->setCurrentDir(dir); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void FileDialogTest::enterDir(int index, const QString &dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    auto enter = [this, index]() { dialog->enterFolder(index); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void FileDialogTest::goForward(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                               const QStringList &expectedFolders)
{
    auto enter = [this]() { dialog->forward(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void FileDialogTest::goBack(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                            const QStringList &expectedFolders)
{
    auto enter = [this]() { dialog->back(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void FileDialogTest::enterAndGetTopLevel()
{
    dialog->folderMode(false);
    enterDir(tempDir.path(), false, true, topLevelFiles, topLevelFolders);
}

void FileDialogTest::enterTopLevelFoldersOnly()
{
    dialog->folderMode(true);
    enterDir(tempDir.path(), false, true, {}, topLevelFolders);
}

void FileDialogTest::moveThroughStructure()
{
    dialog->folderMode(false);
    enterDir(tempDir.path(), false, true, topLevelFiles, topLevelFolders);

    enterDir(0, tempDir.filePath(u"dir1"_s), false, true, {}, {"dir1_1", "dir1_2"});
    enterDir(0, tempDir.filePath(u"dir1/dir1_1"_s), false, true, {}, {"dir1_1_1", "dir1_1_2"});
    enterDir(1, tempDir.filePath(u"dir1/dir1_1/dir1_1_2"_s), false, true, {}, {});

    // No subfolder exists, stay in current one
    enterDir(1, tempDir.filePath(u"dir1/dir1_1/dir1_1_2"_s), false, true, {}, {});

    goBack(tempDir.filePath(u"dir1/dir1_1"_s), true, true, {}, {"dir1_1_1", "dir1_1_2"});
    goForward(tempDir.filePath(u"dir1/dir1_1/dir1_1_2"_s), false, true, {}, {});

    goBack(tempDir.filePath(u"dir1/dir1_1"_s), true, true, {}, {"dir1_1_1", "dir1_1_2"});
    goBack(tempDir.filePath(u"dir1"_s), true, true, {}, {"dir1_1", "dir1_2"});
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);

    enterDir(1, tempDir.filePath(u"dir2"_s), false, true, {}, {});
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);
    goForward(tempDir.filePath(u"dir2"_s), false, true, {}, {});
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);

    enterDir(0, tempDir.filePath(u"dir1"_s), false, true, {}, {"dir1_1", "dir1_2"});
    enterDir(1, tempDir.filePath(u"dir1/dir1_2"_s), false, true, {}, {"dir1_2_1"});
    goBack(tempDir.filePath(u"dir1"_s), true, true, {}, {"dir1_1", "dir1_2"});
}
