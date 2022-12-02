#include "../../src/services/rest/callbackserver.h"
#include "abstracttest.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <QtTest>

class TestCallbackServer : public AbstractTest
{
    Q_OBJECT
public:
    TestCallbackServer() = default;

private:
    CallbackServer m_server;

    static constexpr quint16 PORT = 9999;

private slots:
    void initTestCase();
    void handleConnections();
};

void TestCallbackServer::initTestCase()
{
    QVERIFY(!m_server.isRunning());

    m_server.start(PORT, 0);

    QVERIFY(m_server.isRunning());
    QCOMPARE(m_server.getUrl(), QStringLiteral("http://localhost:%1").arg(PORT));
}

void TestCallbackServer::handleConnections()
{
    QVERIFY(m_server.isRunning());
    QNetworkAccessManager manager;

    // Send request with query parameters
    auto *reply = manager.get(QNetworkRequest(QUrl(QStringLiteral("%1?1=a&2=b").arg(m_server.getUrl()))));

    QSignalSpy spy(&m_server, &CallbackServer::serverClosed);
    spy.wait();

    const auto params = m_server.getParameters();
    QCOMPARE(params[QStringLiteral("1")], QStringLiteral("a"));
    QCOMPARE(params[QStringLiteral("2")], QStringLiteral("b"));
    QVERIFY(!m_server.isRunning());
    reply->deleteLater();

    // Send requests without parameters
    m_server.start(PORT, 1);
    expectWarning();

    auto *reply2 = manager.get(QNetworkRequest(QUrl(m_server.getUrl())));
    auto *reply3 = manager.get(QNetworkRequest(QUrl(m_server.getUrl())));

    spy.wait();
    QVERIFY(m_server.getParameters().isEmpty());

    reply2->deleteLater();
    reply3->deleteLater();
}

QTEST_GUILESS_MAIN(TestCallbackServer)
#include "testcallbackserver.moc"
