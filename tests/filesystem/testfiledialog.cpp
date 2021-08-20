#include <QtTest>
#include <QObject>
#include <functional>

#include "file.h"
#include "fileaccess.h"
#include "fileaccesslocal.h"
#include "filedialog/filedialog.h"
#include "filedialog/fileobject.h"

using namespace Files;

class TestFileDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void enterAndGetTopLevel();
    void enterTopLevelFoldersOnly();
    void moveThroughStructure();

private:
    QTemporaryDir tempDir;
    FileDialog *dialog = nullptr;

    static constexpr int WAIT_TIME_MS = 1000;
    const QStringList topLevelFiles = {
        "file1", "file2", "file3", "file4"
    };
    const QStringList topLevelFolders = {
        "dir1", "dir2", "dir3", "dir4"
    };

    int previousEntryCount = 0;

    void createFile(const QString& file);
    void createDir(const QString& directory);
    void createTestFiles();
    void waitForSignals(QSignalSpy &spy, int count, const QString &dir);

    void enterDir(const std::function<void()>& enter,
                  const QString &dir,
                  bool canGoForward, bool canGoBack,
                  const QStringList &expectedFiles,
                  const QStringList &expectedFolders);

    void enterDir(const QString& dir,
                  bool canGoForward, bool canGoBack,
                  const QStringList &expectedFiles,
                  const QStringList &expectedFolders);

    void enterDir(int index, const QString &dir,
                  bool canGoForward, bool canGoBack,
                  const QStringList &expectedFiles,
                  const QStringList &expectedFolders);

    void goForward(const QString& dir,
                   bool canGoForward, bool canGoBack,
                   const QStringList &expectedFiles,
                   const QStringList &expectedFolders);

    void goBack(const QString &dir,
                bool canGoForward, bool canGoBack,
                const QStringList &expectedFiles,
                const QStringList &expectedFolders);
};

void TestFileDialog::createFile(const QString &file)
{
    auto future = File::saveAsync(tempDir.filePath(file), QByteArray());
    future.waitForFinished();
}

void TestFileDialog::createDir(const QString &directory)
{
    auto future = File::createDirAsync(tempDir.filePath(directory));
    future.waitForFinished();
}

void TestFileDialog::createTestFiles()
{
    for (const auto& dirName : topLevelFolders)
    {
        createDir(dirName);
    }

    createDir("dir1/dir1_1/dir1_1_1");
    createDir("dir1/dir1_1/dir1_1_2");
    createDir("dir1/dir1_2/dir1_2_1");

    for (const auto& fileName : topLevelFiles)
    {
        createFile(fileName);
    }
}

void TestFileDialog::waitForSignals(QSignalSpy &spy, int count, const QString& dir)
{
    while (spy.count() < count)
    {
        QVERIFY2(spy.wait(WAIT_TIME_MS),
                 QString("Timeout at signal %1/%2 (entering %3)")
                 .arg(spy.count())
                 .arg(count)
                 .arg(dir).toUtf8());
    }
}

void TestFileDialog::enterDir(const std::function<void ()> &enter, const QString& dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    QSignalSpy dirChangeSpy(dialog, &FileDialog::currentDirChanged);
    QSignalSpy forwardChangeSpy(dialog, &FileDialog::canGoForwardChanged);
    QSignalSpy backChangeSpy(dialog, &FileDialog::canGoBackChanged);
    QSignalSpy filesChangeSpy(dialog, &FileDialog::entriesChanged);
    QSignalSpy loadingSpy(dialog, &FileDialog::isLoadingChanged);

    enter();

    // 2 signals: once for clearing list, second for new files
    int expectedFileCount = expectedFiles.count() + expectedFolders.count();
    int expectedSignals = previousEntryCount == 0 ? 0 : 1;
    expectedSignals += expectedFileCount != 0 ? 1 : 0;

    waitForSignals(filesChangeSpy, expectedSignals, dir);
    QCOMPARE(filesChangeSpy.count(), expectedSignals);

    waitForSignals(dirChangeSpy, 1, dir);
    QCOMPARE(dialog->currentDir(), dir);
    QCOMPARE(dialog->currentDir(), dirChangeSpy.takeFirst().first().toString());

    waitForSignals(forwardChangeSpy, 1, dir);
    QCOMPARE(dialog->canGoForward(), canGoForward);

    waitForSignals(backChangeSpy, 1, dir);
    QCOMPARE(dialog->canGoBack(), canGoBack);

    waitForSignals(loadingSpy, 2, dir);
    QVERIFY(!dialog->isLoading());

    QStringList files;
    QStringList folders;

    for (auto *object : dialog->entries())
    {
        auto *fileObject = qobject_cast<FileObject*>(object);
        QVERIFY(fileObject);

        if (fileObject->isFolder())
        {
            folders << fileObject->name();
            QVERIFY(expectedFolders.contains(fileObject->name()));
        }
        else
        {
            files << fileObject->name();
            QVERIFY(expectedFiles.contains(fileObject->name()));
        }
    }

    QCOMPARE(files.length(), expectedFiles.length());
    QCOMPARE(folders.length(), expectedFolders.length());
    previousEntryCount = files.count() + folders.count();
}

void TestFileDialog::enterDir(const QString &dir, bool canGoForward,
                              bool canGoBack, const QStringList &expectedFiles,
                              const QStringList &expectedFolders)
{
    auto enter = [this, dir](){ dialog->setCurrentDir(dir); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::enterDir(int index, const QString& dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    auto enter = [this, index](){ dialog->enterFolder(index); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::goForward(const QString& dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    auto enter = [this](){ dialog->forward(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::goBack(const QString& dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    auto enter = [this](){ dialog->back(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::initTestCase()
{
    FileAccess::setInstance(new FileAccessLocal(this));
    createTestFiles();

    dialog = new FileDialog(this);
    QVERIFY(dialog);
}

void TestFileDialog::cleanupTestCase()
{
    tempDir.remove();
}

void TestFileDialog::enterAndGetTopLevel()
{
    dialog->folderMode(false);
    enterDir(tempDir.path(), false, true, topLevelFiles, topLevelFolders);
}

void TestFileDialog::enterTopLevelFoldersOnly()
{
    dialog->folderMode(true);
    enterDir(tempDir.path(), false, true, {}, topLevelFolders);
}

void TestFileDialog::moveThroughStructure()
{
    dialog->folderMode(false);
    enterDir(tempDir.path(), false, true, topLevelFiles, topLevelFolders);

    enterDir(0, tempDir.filePath("dir1"), false, true, {}, { "dir1_1", "dir1_2" });
    enterDir(0, tempDir.filePath("dir1/dir1_1"), false, true, {}, { "dir1_1_1", "dir1_1_2" });
    enterDir(1, tempDir.filePath("dir1/dir1_1/dir1_1_2"), false, true, {}, {});

    // No subfolder exists, stay in current one
    enterDir(1, tempDir.filePath("dir1/dir1_1/dir1_1_2"), false, true, {}, {});

    goBack(tempDir.filePath("dir1/dir1_1"), true, true, {}, { "dir1_1_1", "dir1_1_2" });
    goForward(tempDir.filePath("dir1/dir1_1/dir1_1_2"), false, true, {}, {});

    goBack(tempDir.filePath("dir1/dir1_1"), true, true, {}, { "dir1_1_1", "dir1_1_2" });
    goBack(tempDir.filePath("dir1"), true, true, {}, { "dir1_1", "dir1_2" });
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);

    enterDir(1, tempDir.filePath("dir2"), false, true, {}, {});
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);
    goForward(tempDir.filePath("dir2"), false, true, {}, {});
    goBack(tempDir.path(), true, true, topLevelFiles, topLevelFolders);

    enterDir(0, tempDir.filePath("dir1"), false, true, {}, { "dir1_1", "dir1_2" });
    enterDir(1, tempDir.filePath("dir1/dir1_2"), false, true, {}, { "dir1_2_1" });
    goBack(tempDir.filePath("dir1"), true, true, {}, { "dir1_1", "dir1_2" });
}

QTEST_GUILESS_MAIN(TestFileDialog)
#include "testfiledialog.moc"
