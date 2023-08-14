#include "utils/networkutils.h"
#include <QObject>
#include <QtTest>

class TestNetworkUtils : public QObject
{
    Q_OBJECT
public:
    TestNetworkUtils() = default;

private slots:
    void basicAuthHeader_data();
    void basicAuthHeader();
    void isHttpUrl_data();
    void isHttpUrl();
};

void TestNetworkUtils::basicAuthHeader_data()
{
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("password");
    QTest::addColumn<QString>("header");

    QTest::newRow("default") << "rophil"
                             << "hunter2"
                             << "Basic cm9waGlsOmh1bnRlcjI=";
}

void TestNetworkUtils::basicAuthHeader()
{
    QFETCH(QString, username);
    QFETCH(QString, password);
    QFETCH(QString, header);

    QCOMPARE(QString(NetworkUtils::basicAuthHeader(username, password)), header);
}

void TestNetworkUtils::isHttpUrl_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<bool>("isHttp");

    QTest::newRow("http") << "http://example.com" << true;
    QTest::newRow("https") << "https://example.com" << true;
    QTest::newRow("ftp") << "ftp://example.com" << false;
    QTest::newRow("file") << "file://test.mp3" << false;
}

void TestNetworkUtils::isHttpUrl()
{
    QFETCH(QString, url);
    QFETCH(bool, isHttp);

    QCOMPARE(NetworkUtils::isHttpUrl(url), isHttp);
}

QTEST_APPLESS_MAIN(TestNetworkUtils)
#include "testnetworkutils.moc"
