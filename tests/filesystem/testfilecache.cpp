#include "filesystem/cache/filecache.h"
#include <QtTest>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

constexpr int EXPIRATION_TIME_MS = 5;

TEST(FileCacheTest, CanGetData)
{
    FileCache cache(EXPIRATION_TIME_MS);
    QByteArray data;

    EXPECT_FALSE(cache.tryGetData(u"key"_s, data));

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));

    EXPECT_TRUE(cache.tryGetData(u"key"_s, data));
    EXPECT_EQ(data, "data");
}

TEST(FileCacheTest, CanNotGetExpiredData)
{
    FileCache cache(EXPIRATION_TIME_MS);
    QByteArray data;

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));

    QTest::qSleep(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.tryGetData(u"key"_s, data));
    EXPECT_NE(data, "data");
}

TEST(FileCacheTest, CanUpdateExpiredData)
{
    FileCache cache(EXPIRATION_TIME_MS);
    QByteArray data;

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));

    QTest::qSleep(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "updated data"));
    EXPECT_TRUE(cache.tryGetData(u"key"_s, data));
    EXPECT_EQ(data, "updated data");
}

TEST(FileCacheTest, CanRemoveEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));
    EXPECT_TRUE(cache.removeEntry(u"key"_s));

    EXPECT_FALSE(cache.checkEntry(u"key"_s));
}

TEST(FileCacheTest, CanNotRemoveMissingEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.removeEntry(u"key"_s));
}

TEST(FileCacheTest, CanMoveEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));
    EXPECT_TRUE(cache.moveEntry(u"key"_s, u"moved/key"_s));

    EXPECT_FALSE(cache.checkEntry(u"key"_s));
    EXPECT_TRUE(cache.checkEntry(u"moved/key"_s));
}

TEST(FileCacheTest, CanMoveOverrideEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data0"));
    EXPECT_TRUE(cache.createOrUpdateEntry(u"key2"_s, "data1"));

    EXPECT_TRUE(cache.moveEntry(u"key"_s, u"key2"_s));

    EXPECT_FALSE(cache.checkEntry(u"key"_s));
    EXPECT_TRUE(cache.checkEntry(u"key2"_s));

    QByteArray data;
    EXPECT_TRUE(cache.tryGetData(u"key2"_s, data));
    EXPECT_EQ(data, "data0");
}

TEST(FileCacheTest, CanNotMoveMissingEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.checkEntry(u"key"_s));
    EXPECT_FALSE(cache.moveEntry(u"key"_s, u"copy/key"_s));
    EXPECT_FALSE(cache.checkEntry(u"copy/key"_s));
}

TEST(FileCacheTest, CanNotMoveOutdatedEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));
    QTest::qSleep(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.moveEntry(u"key"_s, u"copy/key"_s));
    EXPECT_FALSE(cache.checkEntry(u"copy/key"_s));
}

TEST(FileCacheTest, CanCopyEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));
    EXPECT_TRUE(cache.copyEntry(u"key"_s, u"copy/key"_s));

    QByteArray data;
    EXPECT_TRUE(cache.tryGetData(u"key"_s, data));
    EXPECT_EQ(data, "data");

    EXPECT_TRUE(cache.tryGetData(u"copy/key"_s, data));
    EXPECT_EQ(data, "data");
}

TEST(FileCacheTest, CanNotCopyMissingEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.checkEntry(u"key"_s));
    EXPECT_FALSE(cache.copyEntry(u"key"_s, u"copy/key"_s));
    EXPECT_FALSE(cache.checkEntry(u"copy/key"_s));
}

TEST(FileCacheTest, CanNotCopyOutdatedEntry)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));
    QTest::qSleep(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.copyEntry(u"key"_s, u"copy/key"_s));
    EXPECT_FALSE(cache.checkEntry(u"copy/key"_s));
}

TEST(FileCacheTest, CanCheckEntries)
{
    FileCache cache(EXPIRATION_TIME_MS);

    EXPECT_FALSE(cache.checkEntry(u"key"_s));

    EXPECT_TRUE(cache.createOrUpdateEntry(u"key"_s, "data"));

    EXPECT_TRUE(cache.checkEntry(u"key"_s));
}
