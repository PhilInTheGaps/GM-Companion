#include <QObject>
#include <QtTest>
#include <functional>

#include "file.h"
#include "fileaccess.h"
#include "fileaccesslocal.h"
#include "filedialogbackend.h"
#include "fileobject.h"

using namespace Files;
using namespace Qt::Literals::StringLiterals;

class TestFileDialog : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void enterAndGetTopLevel();
    void enterTopLevelFoldersOnly();
    void moveThroughStructure();

private:
    QTemporaryDir tempDir;
    FileDialogBackend *dialog = nullptr;

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

void TestFileDialog::waitForSignals(QSignalSpy &spy, int count, const QString &dir)
{
    while (spy.count() < count)
    {
        QVERIFY2(spy.wait(WAIT_TIME_MS),
                 u"Timeout at signal %1/%2 (entering %3)"_s.arg(spy.count()).arg(count).arg(dir).toUtf8());
    }
}

void TestFileDialog::enterDir(const std::function<void()> &enter, const QString &dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    QSignalSpy dirChangeSpy(dialog, &FileDialogBackend::currentDirChanged);
    QSignalSpy forwardChangeSpy(dialog, &FileDialogBackend::canGoForwardChanged);
    QSignalSpy backChangeSpy(dialog, &FileDialogBackend::canGoBackChanged);
    QSignalSpy filesChangeSpy(dialog, &FileDialogBackend::entriesChanged);
    QSignalSpy loadingSpy(dialog, &FileDialogBackend::isLoadingChanged);

    enter();

    // 2 signals: once for clearing list, second for new files
    int expectedFileCount = expectedFiles.count() + expectedFolders.count();
    int expectedSignals = previousEntryCount == 0 ? 0 : 1;
    expectedSignals += expectedFileCount != 0 ? 1 : 0;

    waitForSignals(filesChangeSpy, expectedSignals, dir);
    QCOMPARE(filesChangeSpy.count(), expectedSignals);

    waitForSignals(dirChangeSpy, 1, dir);
    QCOMPARE(dialog->currentDir(), dir);
    QCOMPARE(dialog->currentDir(), dirChangeSpy.takeFirst().constFirst().toString());

    waitForSignals(forwardChangeSpy, 1, dir);
    QCOMPARE(dialog->canGoForward(), canGoForward);

    waitForSignals(backChangeSpy, 1, dir);
    QCOMPARE(dialog->canGoBack(), canGoBack);

    waitForSignals(loadingSpy, 2, dir);
    QVERIFY(!dialog->isLoading());

    QStringList files;
    QStringList folders;

    for (auto *fileObject : dialog->entries())
    {
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

void TestFileDialog::enterDir(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                              const QStringList &expectedFolders)
{
    auto enter = [this, dir]() { dialog->setCurrentDir(dir); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::enterDir(int index, const QString &dir, bool canGoForward, bool canGoBack,
                              const QStringList &expectedFiles, const QStringList &expectedFolders)
{
    auto enter = [this, index]() { dialog->enterFolder(index); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::goForward(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                               const QStringList &expectedFolders)
{
    auto enter = [this]() { dialog->forward(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::goBack(const QString &dir, bool canGoForward, bool canGoBack, const QStringList &expectedFiles,
                            const QStringList &expectedFolders)
{
    auto enter = [this]() { dialog->back(); };
    enterDir(enter, dir, canGoForward, canGoBack, expectedFiles, expectedFolders);
}

void TestFileDialog::initTestCase()
{
    FileAccess::setInstance(new FileAccessLocal(this));
    createTestFiles();

    dialog = new FileDialogBackend(this);
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

QTEST_GUILESS_MAIN(TestFileDialog)
#include "testfiledialog.moc"
