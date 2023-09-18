#include "utils/networkutils.h"
#include <gtest/gtest.h>

struct AuthHeaderTest
{
    QString username;
    QString password;
    QByteArray header;
};

TEST(NetworkUtilsTest, BasicAuthHeader)
{
    std::vector<AuthHeaderTest> tests = {{"rophil", "hunter2", "Basic cm9waGlsOmh1bnRlcjI="}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(NetworkUtils::basicAuthHeader(test.username, test.password), test.header);
    }
}

struct HttpUrlTest
{
    QString url;
    bool isHttp;
};

TEST(NetworkUtilsTest, IsHttpUrl)
{
    std::vector<HttpUrlTest> tests = {{"http://example.com", true},
                                      {"https://example.com", true},
                                      {"ftp://example.com", false},
                                      {"file://test.mp3", false}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(NetworkUtils::isHttpUrl(test.url), test.isHttp);
    }
}
