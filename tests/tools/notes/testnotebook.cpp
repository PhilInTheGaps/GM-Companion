#include "src/tools/notes/notebook.h"
#include "src/tools/notes/notebookchapter.h"
#include "src/tools/notes/notebookpage.h"
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

TEST(NoteBookTest, VerifyHierarchy)
{
    QObject root;
    auto *item = new TreeItem(u"item0"_s, 0, true, &root);
    auto *book = new NoteBook(u"book"_s, item);
    auto *chapter = new NoteBookChapter(u"chapter"_s, book);
    auto *pageBook = new NoteBookPage(u"page_book.md"_s, 1, book);
    auto *pageChapter = new NoteBookPage(u"page_chapter.md"_s, 2, chapter);

    EXPECT_TRUE(root.children().contains(item));
    EXPECT_TRUE(item->children().contains(book));
    EXPECT_TRUE(book->children().contains(chapter));
    EXPECT_TRUE(book->children().contains(pageBook));
    EXPECT_TRUE(chapter->children().contains(pageChapter));
}

TEST(NoteBookTest, CanToggleAndLoad)
{
    NoteBook book(u"TestBook"_s, nullptr);
    EXPECT_FALSE(book.isOpen());
    EXPECT_TRUE(book.chapters().isEmpty());
    EXPECT_TRUE(book.pages().isEmpty());

    // should load on initial toggle
    QSignalSpy spyChapters(&book, &NoteBook::loadChapters);
    QSignalSpy spyPages(&book, &NoteBook::loadPages);

    EXPECT_TRUE(book.canToggle());
    book.toggle();

    spyChapters.wait(10);
    EXPECT_EQ(spyChapters.count(), 1);
    spyChapters.clear();

    spyPages.wait(10);
    EXPECT_EQ(spyPages.count(), 1);
    spyPages.clear();

    book.onChaptersLoaded();
    book.onPagesLoaded();

    // should not load on further toggles
    book.toggle();
    book.toggle();

    EXPECT_FALSE(spyChapters.wait(10));
    EXPECT_TRUE(spyChapters.isEmpty());

    EXPECT_FALSE(spyPages.wait(10));
    EXPECT_TRUE(spyPages.isEmpty());
}

TEST(NoteBookTest, CanCreateChaptersAndPages)
{
    NoteBook book(u"TestBook"_s, nullptr);

    QSignalSpy spyChapters(&book, &NoteBook::createChapter);
    QSignalSpy spyPages(&book, &NoteBook::createPage);

    book.create(book.creatables().at(0), u"Test Chapter"_s);

    spyChapters.wait(10);
    EXPECT_EQ(spyChapters.count(), 1);
    spyChapters.clear();

    EXPECT_EQ(spyPages.count(), 0);

    book.create(book.creatables().at(1), u"Test Page"_s);
    spyPages.wait(10);
    EXPECT_EQ(spyPages.count(), 1);
    spyPages.clear();

    EXPECT_EQ(spyChapters.count(), 0);
}

TEST(NoteBookTest, CanNotCreateInvalidCreatables)
{
    NoteBook book(u"TestBook"_s, nullptr);

    QSignalSpy spyChapters(&book, &NoteBook::createChapter);
    QSignalSpy spyPages(&book, &NoteBook::createPage);

    book.create(u""_s, u""_s);
    book.create(u"InvalidType"_s, u""_s);
    book.create(u"InvalidType"_s, u"Invalid"_s);

    EXPECT_FALSE(spyChapters.wait(10));
    EXPECT_FALSE(spyPages.wait(10));

    EXPECT_EQ(spyChapters.count(), 0);
    EXPECT_EQ(spyPages.count(), 0);
}

TEST(NoteBookTest, CanRecognizeChildren)
{
    NoteBook book(u"TestBook"_s, nullptr);
    book.toggle();

    auto *object = new QObject(&book);
    ASSERT_TRUE(object);
    book.onChildItemAdded();
    EXPECT_EQ(book.chapters().count(), 0);
    EXPECT_EQ(book.pages().count(), 0);
    EXPECT_EQ(book.childItems().count(), 0);

    auto *chapter = new NoteBookChapter(u"Test Chapter"_s, &book);
    ASSERT_TRUE(chapter);
    book.onChildItemAdded();
    EXPECT_EQ(book.chapters().count(), 1);
    EXPECT_EQ(book.pages().count(), 0);
    EXPECT_EQ(book.childItems().count(), 1);

    auto *page = new NoteBookPage(u"Test Page"_s, 1, &book);
    ASSERT_TRUE(page);
    book.onChildItemAdded();
    EXPECT_EQ(book.chapters().count(), 1);
    EXPECT_EQ(book.pages().count(), 1);
    EXPECT_EQ(book.childItems().count(), 2);
}
