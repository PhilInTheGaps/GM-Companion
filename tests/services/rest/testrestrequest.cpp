#include "src/services/rest/restrequest.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

TEST(RestRequestTest, CanConstructGetRequest)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    const RestRequest restRequest(networkRequest, RestRequest::Type::GET, Option::Authenticated);

    EXPECT_EQ(restRequest.id(), -1);
    EXPECT_EQ(restRequest.type(), RestRequest::Type::GET);
    EXPECT_EQ(restRequest.request(), networkRequest);
    EXPECT_TRUE(restRequest.data().isEmpty());
    EXPECT_EQ(restRequest.verb(), "GET");
    EXPECT_EQ(restRequest.options(), Option::Authenticated);
}

TEST(RestRequestTest, CanConstructCustomRequest)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    const RestRequest restRequest(5, networkRequest, RestRequest::Type::CUSTOM, Option::Authenticated, "data", "TEST");

    EXPECT_EQ(restRequest.id(), 5);
    EXPECT_EQ(restRequest.type(), RestRequest::Type::CUSTOM);
    EXPECT_EQ(restRequest.request(), networkRequest);
    EXPECT_EQ(restRequest.data(), "data");
    EXPECT_EQ(restRequest.verb(), "TEST");
    EXPECT_EQ(restRequest.options(), Option::Authenticated);
}

TEST(RestRequestTest, CanSetId)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    RestRequest restRequest(networkRequest, RestRequest::Type::GET, Option::None);

    restRequest.id(3);
    EXPECT_EQ(restRequest.id(), 3);
}

TEST(RestRequestTest, CanSetOptions)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    RestRequest restRequest(networkRequest, RestRequest::Type::GET, Option::None);

    restRequest.options(Option::Authenticated);
    EXPECT_TRUE(restRequest.options().testFlag(Option::Authenticated));

    restRequest.options(Option::None);
    EXPECT_FALSE(restRequest.options().testFlag(Option::Authenticated));
}

TEST(RestRequestTest, CanCorrectlySetVerb)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));

    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::GET, Option::None).verb(), "GET");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::PUT, Option::None).verb(), "PUT");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::POST, Option::None).verb(), "POST");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::CUSTOM, Option::None).verb(), "");
}
