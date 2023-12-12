#include "src/services/rest/restreply.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

TEST(RestReplyTest, CanConstructWithoutErrorByConstRef)
{
    const auto errorText = u""_s;
    const auto data = "test data";
    const QList<QNetworkReply::RawHeaderPair> headers = {{"test", "value"}};

    const RestReply reply(QNetworkReply::NoError, errorText, data, headers);

    EXPECT_EQ(reply.getHeader("test"), "value");
    EXPECT_TRUE(reply.getHeader("test2").isEmpty());
    EXPECT_EQ(reply.headers().length(), 1);

    EXPECT_FALSE(reply.hasError());
    EXPECT_EQ(reply.error(), QNetworkReply::NoError);
    EXPECT_TRUE(reply.errorText().isEmpty());

    EXPECT_EQ(reply.data(), "test data");
}

TEST(RestReplyTest, CanConstructWithErrorByConstRef)
{
    const auto errorText = u"some error"_s;
    const auto data = "test data";
    const QList<QNetworkReply::RawHeaderPair> headers = {{"test", "value"}};

    const RestReply reply(QNetworkReply::UnknownNetworkError, errorText, data, headers);

    EXPECT_EQ(reply.getHeader("test"), "value");
    EXPECT_TRUE(reply.getHeader("test2").isEmpty());
    EXPECT_EQ(reply.headers().length(), 1);

    EXPECT_TRUE(reply.hasError());
    EXPECT_EQ(reply.error(), QNetworkReply::UnknownNetworkError);
    EXPECT_EQ(reply.errorText().toStdString(), "some error");

    EXPECT_EQ(reply.data(), "test data");
}

TEST(RestReplyTest, CanConstructWithoutErrorByValue)
{
    auto errorText = u""_s;
    auto data = "test data";
    const QList<QNetworkReply::RawHeaderPair> headers = {{"test", "value"}};

    const RestReply reply(QNetworkReply::NoError, std::move(errorText), std::move(data), headers);

    EXPECT_EQ(reply.getHeader("test"), "value");
    EXPECT_TRUE(reply.getHeader("test2").isEmpty());
    EXPECT_EQ(reply.headers().length(), 1);

    EXPECT_FALSE(reply.hasError());
    EXPECT_EQ(reply.error(), QNetworkReply::NoError);
    EXPECT_TRUE(reply.errorText().isEmpty());

    EXPECT_EQ(reply.data(), "test data");
}

TEST(RestReplyTest, CanConstructWithErrorByValue)
{
    auto errorText = u"some error"_s;
    auto data = "test data";
    const QList<QNetworkReply::RawHeaderPair> headers = {{"test", "value"}};

    const RestReply reply(QNetworkReply::UnknownNetworkError, std::move(errorText), std::move(data), headers);

    EXPECT_EQ(reply.getHeader("test"), "value");
    EXPECT_TRUE(reply.getHeader("test2").isEmpty());
    EXPECT_EQ(reply.headers().length(), 1);

    EXPECT_TRUE(reply.hasError());
    EXPECT_EQ(reply.error(), QNetworkReply::UnknownNetworkError);
    EXPECT_EQ(reply.errorText().toStdString(), "some error");

    EXPECT_EQ(reply.data(), "test data");
}
