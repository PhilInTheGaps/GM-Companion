#include "src/tools/notes/notebookchapter.h"
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

TEST(NoteBookChapterTest, CanToggleAndLoad)
{
    NoteBookChapter chapter(u"TestChapter"_s, nullptr);

    EXPECT_FALSE(chapter.isOpen());
    EXPECT_TRUE(chapter.pages().isEmpty());

    // should load on initial toggle
    QSignalSpy spy(&chapter, &NoteBookChapter::loadPages);

    EXPECT_TRUE(chapter.canToggle());
    chapter.toggle();

    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
    spy.clear();

    chapter.onPagesLoaded();

    // should not load on further toggles
    chapter.toggle();
    chapter.toggle();

    EXPECT_FALSE(spy.wait(10));
    EXPECT_TRUE(spy.isEmpty());
}

TEST(NoteBookChapterTest, CanCreatePages)
{
    NoteBookChapter chapter(u"TestChapter"_s, nullptr);

    QSignalSpy spy(&chapter, &NoteBookChapter::createPage);

    chapter.create(chapter.creatables().at(0), u"Test Page"_s);
    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
}

TEST(NoteBookChapterTest, CanNotCreateInvalidCreatables)
{
    NoteBookChapter chapter(u"TestChapter"_s, nullptr);

    QSignalSpy spy(&chapter, &NoteBookChapter::createPage);

    chapter.create(u""_s, u""_s);
    chapter.create(u"InvalidType"_s, u""_s);
    chapter.create(u"InvalidType"_s, u"Invalid"_s);

    EXPECT_FALSE(spy.wait(10));
    EXPECT_EQ(spy.count(), 0);
}

TEST(NoteBookChapterTest, CanRename)
{
    NoteBookChapter chapter(u"TestChapter"_s, nullptr);

    QSignalSpy spy(&chapter, &NoteBookChapter::renameChapter);

    chapter.rename(u"TestChapterRenamed"_s);
    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
    spy.clear();
    EXPECT_TRUE(chapter.wasRenamed());
    EXPECT_EQ(chapter.name().toStdString(), "TestChapterRenamed");

    chapter.rename(u"TestChapterRenamedAgain"_s);
    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
    EXPECT_TRUE(chapter.wasRenamed());
    EXPECT_EQ(chapter.name().toStdString(), "TestChapterRenamedAgain");
}

TEST(NoteBookChapterTest, CanRemove)
{
    NoteBookChapter chapter(u"TestChapter"_s, nullptr);

    QSignalSpy spy(&chapter, &NoteBookChapter::deleteChapter);

    chapter.remove();

    spy.wait(10);
    EXPECT_EQ(spy.count(), 1);
}
