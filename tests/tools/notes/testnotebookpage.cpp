#include "src/tools/notes/notebookchapter.h"
#include "src/tools/notes/notebookpage.h"
#include <QSignalSpy>
#include <gtest/gtest.h>
#include <string>

using namespace Qt::Literals::StringLiterals;
using namespace std;

class NoteBookPageTest : public ::testing::Test
{
public:
    NoteBookPageTest()
    {
        page = new NoteBookPage(u"Test Page"_s, 1, &chapter);
    }

protected:
    NoteBookChapter chapter = NoteBookChapter(u"Test Chapter"_s, nullptr);
    QPointer<NoteBookPage> page;
};

TEST_F(NoteBookPageTest, CanConstructPage)
{
    ASSERT_TRUE(page);
    EXPECT_TRUE(page->isSaved());
    EXPECT_TRUE(page->content().isEmpty());
    EXPECT_TRUE(page->html().isEmpty());
    EXPECT_EQ(page->cursorPosition(), 0);
}

TEST_F(NoteBookPageTest, CanModifyContent)
{
    QSignalSpy htmlSpy(page, &NoteBookPage::htmlGenerated);

    page->setContent(u"not modified"_s, false);
    EXPECT_TRUE(page->isSaved());
    htmlSpy.wait();

    page->setContent(u"# page"_s, true);
    htmlSpy.wait();
    EXPECT_FALSE(page->isSaved());
    EXPECT_FALSE(page->html().isEmpty());
    EXPECT_EQ(page->content().toStdString(), "# page"s);
    EXPECT_EQ(page->origContent().toStdString(), "not modified"s);

    page->setIsSaved(true);
    EXPECT_TRUE(page->isSaved());

    page->onContentLoaded(u"# page 2"_s);
    htmlSpy.wait();
    EXPECT_TRUE(page->isSaved());
    EXPECT_EQ(page->content().toStdString(), "# page 2"s);
}

TEST_F(NoteBookPageTest, CanSave)
{
    QSignalSpy htmlSpy(page, &NoteBookPage::htmlGenerated);

    page->setContent(u"content"_s, true);
    htmlSpy.wait();
    EXPECT_FALSE(page->isSaved());

    QSignalSpy spy(page, &NoteBookPage::savePage);
    page->save();
    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(NoteBookPageTest, CanRename)
{
    page->rename(u"page.txt"_s);
    EXPECT_EQ(page->name().toStdString(), "page.txt"s);
    EXPECT_TRUE(page->wasRenamed());

    // should automatically add file extension if it is missing
    page->rename(u"page"_s);
    EXPECT_EQ(page->name().toStdString(), "page.md"s);
}

TEST_F(NoteBookPageTest, CanRemove)
{
    ASSERT_TRUE(page);

    QObject::connect(page, &NoteBookPage::deletePage, &chapter, [this]() { delete page; });

    page->remove();

    EXPECT_FALSE(page);
}

TEST_F(NoteBookPageTest, CanOpenAndClose)
{
    QSignalSpy htmlSpy(page, &NoteBookPage::htmlGenerated);

    EXPECT_FALSE(page->isOpen());

    page->isCurrent(true);
    page->setContent(u"content"_s, true);
    htmlSpy.wait();

    EXPECT_TRUE(page->keepOpen());
    EXPECT_TRUE(page->isCurrent());

    page->close();
    EXPECT_FALSE(page->keepOpen());
    EXPECT_FALSE(page->isCurrent());
}
