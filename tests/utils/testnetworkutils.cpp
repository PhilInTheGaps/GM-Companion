#include <QtTest>
#include <QObject>
#include "../../src/utils/networkutils.h"

class TestNetworkUtils : public QObject
{
    Q_OBJECT
public:
    TestNetworkUtils() = default;

private slots:
    void basicAuthHeader_data();
    void basicAuthHeader();
};

void TestNetworkUtils::basicAuthHeader_data()
{
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("password");
    QTest::addColumn<QString>("header");

    QTest::newRow("default") << "rophil" << "hunter2" << "Basic cm9waGlsOmh1bnRlcjI=";
}

void TestNetworkUtils::basicAuthHeader()
{
    QFETCH(QString, username);
    QFETCH(QString, password);
    QFETCH(QString, header);

    QCOMPARE(QString(NetworkUtils::basicAuthHeader(username, password)), header);
}

QTEST_APPLESS_MAIN(TestNetworkUtils)
#include "testnetworkutils.moc"
