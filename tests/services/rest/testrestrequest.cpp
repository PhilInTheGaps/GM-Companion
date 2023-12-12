#include "src/services/rest/restrequest.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

TEST(RestRequestTest, CanConstructGetRequest)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    const RestRequest restRequest(networkRequest, RestRequest::Type::GET);

    EXPECT_EQ(restRequest.id(), -1);
    EXPECT_EQ(restRequest.type(), RestRequest::Type::GET);
    EXPECT_EQ(restRequest.request(), networkRequest);
    EXPECT_TRUE(restRequest.data().isEmpty());
    EXPECT_EQ(restRequest.verb(), "GET");
}

TEST(RestRequestTest, CanConstructCustomRequest)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    const RestRequest restRequest(5, networkRequest, RestRequest::Type::CUSTOM, "data", "TEST");

    EXPECT_EQ(restRequest.id(), 5);
    EXPECT_EQ(restRequest.type(), RestRequest::Type::CUSTOM);
    EXPECT_EQ(restRequest.request(), networkRequest);
    EXPECT_EQ(restRequest.data(), "data");
    EXPECT_EQ(restRequest.verb(), "TEST");
}

TEST(RestRequestTest, CanSetId)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    RestRequest restRequest(networkRequest, RestRequest::Type::GET);

    restRequest.id(3);
    EXPECT_EQ(restRequest.id(), 3);
}

TEST(RestRequestTest, CanSetAuthRequirements)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));
    RestRequest restRequest(networkRequest, RestRequest::Type::GET);

    restRequest.isAuthRequired(true);
    EXPECT_TRUE(restRequest.isAuthRequired());

    restRequest.isAuthRequired(false);
    EXPECT_FALSE(restRequest.isAuthRequired());
}

TEST(RestRequestTest, CanCorrectlySetVerb)
{
    const QNetworkRequest networkRequest(QUrl(u"https://localhost"_s));

    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::GET).verb(), "GET");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::PUT).verb(), "PUT");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::POST).verb(), "POST");
    EXPECT_EQ(RestRequest(networkRequest, RestRequest::Type::CUSTOM).verb(), "");
}
