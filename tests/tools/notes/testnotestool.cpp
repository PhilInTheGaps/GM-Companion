#include "settings/settingsmanager.h"
#include "src/tools/notes/notebook.h"
#include "src/tools/notes/notebookchapter.h"
#include "src/tools/notes/notebookpage.h"
#include "src/tools/notes/notestool.h"
#include "testhelper/abstractmocknetworkmanager.h"
#include "testhelper/abstracttest.h"
#include "testhelper/mocknetworkreply.h"
#include "utils/fileutils.h"
#include <QDesktopServices>
#include <QFileInfo>
#include <QSignalSpy>
#include <QTest>
#include <gtest/gtest.h>

#ifndef QT_GUI_LIB
#define QT_GUI_LIB
#endif

using namespace Qt::Literals::StringLiterals;

class MockNetworkManager : public AbstractMockNetworkManager
{
    Q_OBJECT
public:
    explicit MockNetworkManager(const QString &host, QObject *parent = nullptr)
        : AbstractMockNetworkManager({host}, parent)
    {
    }

protected:
    auto sendMockReply(Operation, const QNetworkRequest &, const QByteArray &) -> QNetworkReply * override
    {
        emit replySent();
        return MockNetworkReply::successEmpty(this);
    }
};

class NotesToolTest : public AbstractTest
{
protected:
    void SetUp() override
    {
        userDataPath = backupUserFolder(SettingsManager::getPath(u"notes"_s));

        QDesktopServices::setUrlHandler(u"http"_s, &networkManager, "simulateBrowser");
        QDesktopServices::setUrlHandler(u"https"_s, &networkManager, "simulateBrowser");

        QSignalSpy modelSpy(&tool, &NotesTool::notesModelChanged);
        tool.loadData();
        StaticAbstractTest::waitForSpy(modelSpy, 1);

        {
            QSignalSpy spy(tool.notesModel(), &TreeItem::childItemsChanged);
            tool.createBook(u"book"_s);
            StaticAbstractTest::waitForSpy(spy, 1);
            EXPECT_EQ(tool.notesModel()->childItems().count(), 1);

            book = qobject_cast<NoteBook *>(tool.notesModel()->childItems().at(0));
            ASSERT_TRUE(book);
        }

        {
            QSignalSpy spy(book, &TreeItem::childItemsChanged);
            book->create(book->creatables().at(0), u"chapter"_s);
            StaticAbstractTest::waitForSpy(spy, 1);
            spy.clear();

            book->toggle();
            StaticAbstractTest::waitForSpy(spy, 1);

            chapter = qobject_cast<NoteBookChapter *>(book->childItems().at(0));
            ASSERT_TRUE(chapter);
        }

        {
            QSignalSpy spy(chapter, &TreeItem::childItemsChanged);

            chapter->create(chapter->creatables().at(0), u"page0.md"_s);
            StaticAbstractTest::waitForSpy(spy, 1);
            spy.clear();

            chapter->create(chapter->creatables().at(0), u"page1"_s);
            StaticAbstractTest::waitForSpy(spy, 1);
            spy.clear();

            chapter->toggle();
            StaticAbstractTest::waitForSpy(spy, 1);
            ASSERT_EQ(chapter->childItems().length(), 2);

            page0 = qobject_cast<NoteBookPage *>(chapter->childItems().at(0));
            ASSERT_TRUE(page0);

            page1 = qobject_cast<NoteBookPage *>(chapter->childItems().at(1));
            ASSERT_TRUE(page1);
        }

        QSignalSpy htmlSpy0(page0, &NoteBookPage::htmlGenerated);
        QSignalSpy htmlSpy1(page1, &NoteBookPage::htmlGenerated);

        page0->setContent(u"page 0"_s, false);
        StaticAbstractTest::waitForSpy(htmlSpy0, 1);

        page1->setContent(u"page 1"_s, false);
        StaticAbstractTest::waitForSpy(htmlSpy1, 1);

        // creating a text document from within the test crashes
        // so we can't test text doc interaction
        tool.setQmlTextDoc(nullptr);

        tool.setCurrentPage(page0);
        tool.setEditMode(true);
        page0->toggle();
    }

    void TearDown() override
    {
        QDesktopServices::unsetUrlHandler(u"http"_s);
        QDesktopServices::unsetUrlHandler(u"https"_s);

        restoreUserFolder(userDataPath, SettingsManager::getPath(u"notes"_s));
    }

protected:
    QString userDataPath;
    NotesTool tool = NotesTool(nullptr);
    TreeItem rootItem = TreeItem(u"/"_s, 0, true, nullptr);
    MockNetworkManager networkManager = MockNetworkManager(u"notes.mock"_s, nullptr);

    QPointer<NoteBook> book;
    QPointer<NoteBookChapter> chapter;
    QPointer<NoteBookPage> page0;
    QPointer<NoteBookPage> page1;
};

TEST_F(NotesToolTest, CanConstruct)
{
    EXPECT_EQ(tool.currentPage(), page0);
    ASSERT_EQ(tool.openedPages().count(), 1);
    EXPECT_EQ(tool.openedPages().at(0), page0);
}

TEST_F(NotesToolTest, CanEncrypt)
{
    auto unencrypted = tool.currentPage()->content();
    tool.encrypt();
    EXPECT_NE(unencrypted.toStdString(), tool.currentPage()->content().toStdString());
}

TEST_F(NotesToolTest, CanCreateNewPageWhenChapterIsOpened)
{
    auto countBefore = chapter->childItems().count();
    ASSERT_TRUE(chapter->isOpen());

    QSignalSpy spy(chapter, &TreeItem::childItemsChanged);

    chapter->create(chapter->creatables().at(0), u"page2.md"_s);
    StaticAbstractTest::waitForSpy(spy, 1);

    ASSERT_GT(chapter->childItems().count(), countBefore);
}

TEST_F(NotesToolTest, CanCreateAdditionalChapters)
{
    auto countBefore = book->childItems().count();

    QSignalSpy spy(book, &TreeItem::childItemsChanged);
    book->create(book->creatables().at(0), u"chapter2"_s);
    StaticAbstractTest::waitForSpy(spy, 1);

    ASSERT_GT(book->childItems().count(), countBefore);
}

TEST_F(NotesToolTest, CanNotCreateSameBookTwice)
{
    auto countBefore = rootItem.childItems().count();

    QSignalSpy spy(book, &TreeItem::childItemsChanged);
    tool.createBook(book->name());
    EXPECT_FALSE(spy.wait(50));

    EXPECT_EQ(countBefore, rootItem.childItems().count());
}

TEST_F(NotesToolTest, CanSavePage)
{
    QSignalSpy htmlSpy(page0, &NoteBookPage::htmlGenerated);
    page0->setContent(u"CanSavePage"_s);
    StaticAbstractTest::waitForSpy(htmlSpy, 1);

    QSignalSpy spy(page0, &NoteBookPage::isSavedChanged);
    page0->save();
    StaticAbstractTest::waitForSpy(spy, 1);

    EXPECT_TRUE(page0->isSaved());
}

TEST_F(NotesToolTest, CanNotExportWithoutTextDocument)
{
    tool.setEditMode(false);
    tool.exportPdf();
}

TEST_F(NotesToolTest, CanOpenLinks)
{
    QSignalSpy spy(&networkManager, &MockNetworkManager::replySent);
    tool.linkClicked(u"https://notes.mock"_s);
    StaticAbstractTest::waitForSpy(spy, 1);
}

TEST_F(NotesToolTest, CanCloseUnneededPages)
{
    ASSERT_EQ(tool.currentPage(), page0);

    page1->toggle();
    EXPECT_EQ(tool.openedPages().count(), 1);
    EXPECT_EQ(tool.currentPage(), page1);

    page1->keepOpen(true);
    page0->toggle();
    EXPECT_EQ(tool.openedPages().count(), 2);
    EXPECT_EQ(tool.currentPage(), page0);
}

TEST_F(NotesToolTest, CanClosePages)
{
    ASSERT_EQ(tool.currentPage(), page0);
    page0->keepOpen(true);

    page1->toggle();

    QSignalSpy spy(&tool, &NotesTool::openedPagesChanged);
    page1->close();
    StaticAbstractTest::waitForSpy(spy, 1);
    spy.clear();

    page0->close();
    StaticAbstractTest::waitForSpy(spy, 1);

    EXPECT_TRUE(tool.openedPages().isEmpty());
}

TEST_F(NotesToolTest, CanRenameBook)
{
    QSignalSpy spy(chapter, &NoteBookChapter::renameChapter);
    chapter->rename(u"TestChapterRenamed"_s);
    StaticAbstractTest::waitForSpy(spy, 1);
    EXPECT_TRUE(chapter->wasRenamed());
    EXPECT_EQ(chapter->name().toStdString(), "TestChapterRenamed");
    EXPECT_TRUE(chapter->path().endsWith("TestChapterRenamed"_L1));

    QTest::qWait(50); // wait for filesystem

    auto path = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(u"notes"_s));
    const QFileInfo info(path);
    EXPECT_TRUE(info.exists());
    EXPECT_TRUE(info.isDir());
}

TEST_F(NotesToolTest, CanRenamePage)
{
    QSignalSpy spy(page0, &NoteBookPage::renamePage);
    page0->rename(u"TestPageRenamed.md"_s);
    StaticAbstractTest::waitForSpy(spy, 1);
    EXPECT_TRUE(page0->wasRenamed());
    EXPECT_EQ(page0->name().toStdString(), "TestPageRenamed.md");
    EXPECT_TRUE(page0->path().endsWith("TestPageRenamed.md"_L1));

    QTest::qWait(50); // wait for filesystem

    auto path = FileUtils::fileInDir(page0->path(), SettingsManager::getPath(u"notes"_s));
    const QFileInfo info(path);
    EXPECT_TRUE(info.exists());
    EXPECT_TRUE(info.isFile());
}

TEST_F(NotesToolTest, CanDeleteChapter)
{
    auto path = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(u"notes"_s));

    QSignalSpy spy(chapter, &NoteBookPage::destroyed);
    chapter->remove();
    StaticAbstractTest::waitForSpy(spy, 1);
    EXPECT_FALSE(chapter);

    const QFileInfo info(path);
    EXPECT_FALSE(info.exists());
}

TEST_F(NotesToolTest, CanDeletePage)
{
    auto path = FileUtils::fileInDir(page0->path(), SettingsManager::getPath(u"notes"_s));

    QSignalSpy spy(page0, &NoteBookPage::destroyed);
    page0->remove();
    StaticAbstractTest::waitForSpy(spy, 1);
    EXPECT_FALSE(page0);

    const QFileInfo info(path);
    EXPECT_FALSE(info.exists());
}

#include "testnotestool.moc"
