#include <QtTest>
#include "testnetworkutils.h"
#include "../../src/utils/networkutils.h"

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

    QCOMPARE(NetworkUtils::basicAuthHeader(username, password), header);
}

QTEST_APPLESS_MAIN(TestNetworkUtils)
