#include "src/services/rest/callbackserver.h"
#include "tests/testhelper/abstracttest.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <gtest/gtest.h>

class CallbackServerTest : public AbstractTest
{
protected:
    void SetUp() override
    {
        EXPECT_FALSE(m_server.isRunning());

        m_server.start(PORT, 0);

        EXPECT_TRUE(m_server.isRunning());
        EXPECT_EQ(m_server.getUrl(), QStringLiteral("http://localhost:%1").arg(PORT));
    }

    CallbackServer m_server = CallbackServer(nullptr);
    static constexpr quint16 PORT = 9999;
};

TEST_F(CallbackServerTest, CanHandleConnections)
{
    ASSERT_TRUE(m_server.isRunning());
    QNetworkAccessManager manager;

    // Send request with query parameters
    auto *reply = manager.get(QNetworkRequest(QUrl(QStringLiteral("%1?1=a&2=b").arg(m_server.getUrl()))));

    QSignalSpy spy(&m_server, &CallbackServer::serverClosed);
    spy.wait();

    const auto params = m_server.getParameters();
    EXPECT_EQ(params[QStringLiteral("1")], QStringLiteral("a"));
    EXPECT_EQ(params[QStringLiteral("2")], QStringLiteral("b"));
    EXPECT_FALSE(m_server.isRunning());
    reply->deleteLater();

    // Send requests without parameters
    m_server.start(PORT, 1);
    expectWarning();

    auto *reply2 = manager.get(QNetworkRequest(QUrl(m_server.getUrl())));
    auto *reply3 = manager.get(QNetworkRequest(QUrl(m_server.getUrl())));

    spy.wait();
    EXPECT_TRUE(m_server.getParameters().isEmpty());

    reply2->deleteLater();
    reply3->deleteLater();
}
