#include "src/common/exceptions/notimplementedexception.h"
#include "src/services/youtube/pipedconnector.h"
#include "tests/testhelper/abstractmocknetworkmanager.h"
#include "tests/testhelper/mocknetworkreply.h"
#include "tests/testhelper/staticabstracttest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto API_HOST = "piped-instances.kavin.rocks";
constexpr auto WIKI_HOST = "raw.githubusercontent.com";

class PipedMockNetworkManager : public AbstractMockNetworkManager
{
    Q_OBJECT
public:
    explicit PipedMockNetworkManager(const QStringList &hosts, QObject *parent = nullptr)
        : AbstractMockNetworkManager{hosts, parent}
    {
        setTransferTimeout(5000);
    }

    void enableApi(bool enable)
    {
        m_isApiEnabled = enable;
    }

    void enableCdn(bool enable)
    {
        m_isCdnEnabled = enable;
    }

protected:
    auto sendMockReply(Operation, const QNetworkRequest &req, const QByteArray &) -> QNetworkReply * override
    {
        if (req.url().path() == "/wiki/TeamPiped/Piped-Frontend/Instances.md"_L1)
        {
            const QByteArray data =
                m_isCdnEnabled ? "Instance Name | Instance API URL | Instance Location(s) | CDN | Registered Users\n"
                                 "------------- | ---------------- | -------------------- | --- | ----------------\n"
                                 "kavin.rocks (Official) | https://pipedapi.kavin.rocks | _ | Yes | \n"
                                 "tokhmi.xyz | https://pipedapi.tokhmi.xyz | _ | Yes | \n"
                                 "moomoo.me | https://pipedapi.moomoo.me | _ | Yes | \n"
                                 "kavin.rocks libre (Official) | https://pipedapi-libre.kavin.rocks | _ | No | \n"
                               : "Instance Name | Instance API URL | Instance Location(s) | CDN | Registered Users\n"
                                 "------------- | ---------------- | -------------------- | --- | ----------------\n"
                                 "kavin.rocks libre (Official) | https://pipedapi-libre.kavin.rocks | _ | No | \n"
                                 "mint.lgbt | https://pa.mint.lgbt | _ | No | \n";

            emit replySent();
            return MockNetworkReply::successGeneric(data, this);
        }

        if (m_isApiEnabled && (req.url().path().isEmpty() || req.url().path() == "/"_L1))
        {
            const QByteArray data =
                m_isCdnEnabled ? "[{\"name\":\"instance0\",\"api_url\":\"https://"
                                 "pipedapi.instance0.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":true,\"registered\":214820,\"last_checked\":"
                                 "1702215032,\"cache\":true,\"s3_enabled\":true,\"image_proxy_url\":\"https://"
                                 "pipedproxy.instance0.mock\",\"registration_disabled\":false},"
                                 "{\"name\":\"instance1\",\"api_url\":\"https://"
                                 "pipedapi.instance1.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":true,\"registered\":1457,\"last_checked\":1702215032,"
                                 "\"cache\":true,\"s3_enabled\":false,\"image_proxy_url\":\"https://"
                                 "piped-proxy.instance1.mock\",\"registration_disabled\":false},"
                                 "{\"name\":\"instance2\",\"api_url\":\"https://"
                                 "pipedapi.instance2.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":false,\"registered\":344,\"last_checked\":1702215032,"
                                 "\"cache\":true,\"s3_enabled\":false,\"image_proxy_url\":\"https://"
                                 "pipedproxy.instance2.mock\",\"registration_disabled\":false}]"
                               : "[{\"name\":\"instance0\",\"api_url\":\"https://"
                                 "pipedapi.instance0.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":false,\"registered\":214820,\"last_checked\":"
                                 "1702215032,\"cache\":true,\"s3_enabled\":true,\"image_proxy_url\":\"https://"
                                 "pipedproxy.instance0.mock\",\"registration_disabled\":false},"
                                 "{\"name\":\"instance1\",\"api_url\":\"https://"
                                 "pipedapi.instance1.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":false,\"registered\":1457,\"last_checked\":1702215032,"
                                 "\"cache\":true,\"s3_enabled\":false,\"image_proxy_url\":\"https://"
                                 "piped-proxy.instance1.mock\",\"registration_disabled\":false},"
                                 "{\"name\":\"instance2\",\"api_url\":\"https://"
                                 "pipedapi.instance2.mock\",\"locations\":\"_\",\"version\":\"2023-12-07-2c5b9ef\","
                                 "\"up_to_date\":true,\"cdn\":false,\"registered\":344,\"last_checked\":1702215032,"
                                 "\"cache\":true,\"s3_enabled\":false,\"image_proxy_url\":\"https://"
                                 "pipedproxy.instance2.mock\",\"registration_disabled\":false}]";

            emit replySent();
            return MockNetworkReply::successGeneric(data, this);
        }

        emit replySent();
        return MockNetworkReply::notFound(this);
    }

private:
    bool m_isCdnEnabled = true;
    bool m_isApiEnabled = true;
};

auto makeNetworkManager() -> QNetworkAccessManager *
{
    auto *networkManager = new QNetworkAccessManager();
    networkManager->setTransferTimeout(5000);
    return networkManager;
}

auto makeMockNetworkManager(bool cdn, bool api) -> QNetworkAccessManager *
{
    auto *networkManager = new PipedMockNetworkManager({API_HOST, WIKI_HOST});
    networkManager->enableCdn(cdn);
    networkManager->enableApi(api);
    networkManager->setTransferTimeout(5000);
    return networkManager;
}

TEST(PipedConnectorTest, CanFetchPipedInstances)
{
    PipedConnector connector;
    connector.setNetworkManager(makeNetworkManager());

    auto future = connector.fetchInstancesAsync();
    StaticAbstractTest::testFutureNoAuth(future, "fetchPipedInstancesAsync", [&connector]() {
        auto pipedInstances = connector.instances();
        EXPECT_GT(pipedInstances.size(), 0);

        bool hasAtLeastOneCdn = false;

        for (const auto &pipedInstance : pipedInstances)
        {
            EXPECT_FALSE(pipedInstance.name.isEmpty());
            EXPECT_FALSE(pipedInstance.apiUrl.isEmpty());
            EXPECT_FALSE(pipedInstance.locations.isEmpty());

            if (pipedInstance.cdn)
            {
                hasAtLeastOneCdn = true;
            }
        }

        EXPECT_TRUE(hasAtLeastOneCdn);
    });
}

TEST(PipedConnectorTest, CanFetchPipedInstancesFromApi)
{
    PipedConnector connector;
    connector.setNetworkManager(makeMockNetworkManager(true, true));

    auto future = connector.fetchInstancesAsync();
    StaticAbstractTest::testFutureNoAuth(future, "fetchPipedInstancesAsync", [&connector]() {
        auto pipedInstances = connector.instances();
        EXPECT_GT(pipedInstances.size(), 0);

        bool hasAtLeastOneCdn = false;
        bool hasApiInstance = false;

        for (const auto &pipedInstance : pipedInstances)
        {
            EXPECT_FALSE(pipedInstance.name.isEmpty());
            EXPECT_FALSE(pipedInstance.apiUrl.isEmpty());
            EXPECT_FALSE(pipedInstance.locations.isEmpty());

            if (pipedInstance.cdn)
            {
                hasAtLeastOneCdn = true;
            }

            if (pipedInstance.name == "instance0"_L1)
            {
                hasApiInstance = true;
            }
        }

        EXPECT_TRUE(hasAtLeastOneCdn);
        EXPECT_TRUE(hasApiInstance);
    });
}

TEST(PipedConnectorTest, CanFetchPipedInstancesFromWiki)
{
    PipedConnector connector;
    connector.setNetworkManager(makeMockNetworkManager(true, false));

    auto future = connector.fetchInstancesAsync();
    StaticAbstractTest::testFutureNoAuth(future, "fetchPipedInstancesAsync", [&connector]() {
        auto pipedInstances = connector.instances();
        EXPECT_GT(pipedInstances.size(), 0);

        bool hasAtLeastOneCdn = false;
        bool hasApiInstance = false;

        for (const auto &pipedInstance : pipedInstances)
        {
            EXPECT_FALSE(pipedInstance.name.isEmpty());
            EXPECT_FALSE(pipedInstance.apiUrl.isEmpty());
            EXPECT_FALSE(pipedInstance.locations.isEmpty());

            if (pipedInstance.cdn)
            {
                hasAtLeastOneCdn = true;
            }
        }

        EXPECT_TRUE(hasAtLeastOneCdn);
        EXPECT_FALSE(hasApiInstance);
    });
}

TEST(PipedConnectorTest, CanSelectBestInstanceWithCdn)
{
    PipedConnector connector;
    connector.setNetworkManager(makeMockNetworkManager(true, true));

    auto future = connector.fetchInstancesAsync();
    StaticAbstractTest::testFutureNoAuth(future, "fetchPipedInstancesAsync", [&connector]() {
        for (int i = 0; i < 100; i++)
        {
            connector.selectBestInstance();
            EXPECT_TRUE(connector.currentInstance().cdn);
        }
    });
}

TEST(PipedConnectorTest, CanSelectBestInstanceWithoutCdn)
{
    PipedConnector connector;
    connector.setNetworkManager(makeMockNetworkManager(false, true));

    auto future = connector.fetchInstancesAsync();
    StaticAbstractTest::testFutureNoAuth(future, "fetchPipedInstancesAsync", [&connector]() {
        for (int i = 0; i < 100; i++)
        {
            connector.selectBestInstance();
            EXPECT_FALSE(connector.currentInstance().cdn);
        }
    });
}

TEST(PipedConnectorTest, VerifyAccessIsGranted)
{
    PipedConnector connector;
    EXPECT_TRUE(connector.isAccessGranted());
}

void testGetRequest(PipedConnector &connector, int currentFailCount)
{
    auto tryAgain = [&connector, currentFailCount]() {
        connector.selectBestInstance();
        testGetRequest(connector, currentFailCount + 1);
    };

    auto future = connector.get(QNetworkRequest(QUrl(u"/streams/9bZkp7q19f0"_s)), false);
    StaticAbstractTest::testFutureNoAuth(future, "get stream", [tryAgain, future, currentFailCount]() {
        auto reply = future.result();

        if (reply.hasError() && currentFailCount < 10)
        {
            tryAgain();
            return;
        }

        EXPECT_FALSE(reply.hasError());
        EXPECT_FALSE(reply.data().isEmpty());

        QJsonParseError parseError;
        auto doc = QJsonDocument::fromJson(reply.data(), &parseError);

        if (parseError.error != QJsonParseError::NoError && currentFailCount < 10)
        {
            tryAgain();
            return;
        }

        EXPECT_EQ(parseError.error, QJsonParseError::NoError);
        auto title = doc.object()["title"_L1].toString();
        EXPECT_FALSE(title.isEmpty());
    });
}

TEST(PipedConnectorTest, CanSendGetRequests)
{
    auto *connector = new PipedConnector;
    connector->setNetworkManager(makeNetworkManager());

    testGetRequest(*connector, 0);
}

TEST(PipedConnectorTest, ThrowOnNonGetRequests)
{
    PipedConnector connector;
    connector.setNetworkManager(makeNetworkManager());

    EXPECT_THROW(connector.put(QNetworkRequest(), {}), NotImplementedException);
    EXPECT_THROW(connector.post(QNetworkRequest(), {}), NotImplementedException);
    EXPECT_THROW(connector.customRequest(QNetworkRequest(), "", {}), NotImplementedException);
}

#include "testpipedconnector.moc"
