#include "src/services/youtube/videoid.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

TEST(VideoIdTest, CanRecognizeValidIds)
{
    EXPECT_TRUE(VideoId(u"9bZkp7q19f0"_s).isValid());
    EXPECT_TRUE(VideoId(u"_kmeFXjjGfk"_s).isValid());
    EXPECT_TRUE(VideoId(u"AI7ULzgf8RU"_s).isValid());

    EXPECT_FALSE(VideoId(u""_s).isValid()) << "empty id";
    EXPECT_FALSE(VideoId(u"pI2I2zqzeK"_s).isValid()) << "too short";
    EXPECT_FALSE(VideoId(u"pI2I2z zeKg"_s).isValid()) << "gaps";
}

TEST(VideoIdTest, CanFindIdsFromUrls)
{
    EXPECT_TRUE(VideoId(u"youtube.com/watch?v=yIVRs6YSbOM"_s).isValid());
    EXPECT_TRUE(VideoId(u"youtu.be/yIVRs6YSbOM"_s).isValid());
    EXPECT_TRUE(VideoId(u"youtube.com/embed/yIVRs6YSbOM"_s).isValid());
    EXPECT_TRUE(VideoId(u"/watch?v=oksESAMg7WM"_s).isValid());

    EXPECT_FALSE(VideoId(u"youtube.com/abc?v=pI2I2zqzeKg"_s).isValid());
    EXPECT_FALSE(VideoId(u"youtu.be/watch?v=abc"_s).isValid());
    EXPECT_FALSE(VideoId(u"youtube.com/embed/"_s).isValid());
    EXPECT_FALSE(VideoId(u"https://localhost/watch?v=yIVRs6YSbOM"_s).isValid());
}

TEST(VideoIdTest, CanGetIdAsString)
{
    EXPECT_EQ(VideoId(u"9bZkp7q19f0"_s).toString().toStdString(), "9bZkp7q19f0");
    EXPECT_TRUE(VideoId(u""_s).toString().isEmpty());
}

TEST(VideoIdTest, CanGetIdAsUrl)
{
    EXPECT_EQ(VideoId(u"9bZkp7q19f0"_s).toUrl().toStdString(), "https://www.youtube.com/watch?v=9bZkp7q19f0");
    EXPECT_TRUE(VideoId(u""_s).toString().isEmpty());
}
