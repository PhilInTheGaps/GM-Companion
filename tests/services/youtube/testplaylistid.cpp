#include "src/services/youtube/playlistid.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

TEST(PlaylistIdTest, CanRecognizeValidIds)
{
    EXPECT_TRUE(PlaylistId(u"PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H"_s).isValid());
    EXPECT_TRUE(PlaylistId(u"PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr"_s).isValid());

    EXPECT_FALSE(PlaylistId(u""_s).isValid()) << "empty id";
    EXPECT_FALSE(PlaylistId(u"a"_s).isValid()) << "too short";
    EXPECT_FALSE(PlaylistId(u"PLOU2XLYxmsIJGErt 5rrCqaSGTMyyqNt2H"_s).isValid()) << "gaps";
}

TEST(PlaylistIdTest, CanFindIdsFromUrls)
{
    EXPECT_TRUE(PlaylistId(u"https://www.youtube.com/playlist?list=PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H"_s).isValid());
    EXPECT_TRUE(
        PlaylistId(u"https://www.youtube.com/watch?v=b8m9zhNAgKs&list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr"_s).isValid());
    EXPECT_TRUE(PlaylistId(u"https://youtu.be/b8m9zhNAgKs/?list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVA"_s).isValid());
    EXPECT_TRUE(
        PlaylistId(u"https://www.youtube.com/embed/b8m9zhNAgKs/?list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr"_s).isValid());

    EXPECT_FALSE(PlaylistId(u"youtube.com/watch?v=yIVRs6YSbOM"_s).isValid());
    EXPECT_FALSE(PlaylistId(u"youtu.be/yIVRs6YSbOM"_s).isValid());
    EXPECT_FALSE(PlaylistId(u"youtube.com/embed/yIVRs6YSbOM"_s).isValid());

    EXPECT_FALSE(PlaylistId(u"https://www.youtube.com/playlist"_s).isValid());
    EXPECT_FALSE(PlaylistId(u"https://youtu.be/"_s).isValid());
    EXPECT_FALSE(PlaylistId(u"https://www.youtube.com/embed/"_s).isValid());
}

TEST(PlaylistIdTest, CanGetIdAsString)
{
    EXPECT_EQ(PlaylistId(u"PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H"_s).toString().toStdString(),
              "PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H");
    EXPECT_TRUE(PlaylistId(u""_s).toString().isEmpty());
}

TEST(PlaylistIdTest, CanGetIdAsUrl)
{
    EXPECT_EQ(PlaylistId(u"PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H"_s).toUrl().toStdString(),
              "https://www.youtube.com/playlist?list=PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H");
    EXPECT_TRUE(PlaylistId(u""_s).toString().isEmpty());
}
