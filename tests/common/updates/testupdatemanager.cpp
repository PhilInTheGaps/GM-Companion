#include <QtTest>
#include <QObject>
#include <QFutureWatcher>
#include "updates/updatemanager.h"

class TestUpdateManager : public UpdateManager
{
    Q_OBJECT

private:
    static constexpr int WAIT_TIME_IN_MS = 10000;

private slots:
    static void compareVersions_data();
    static void compareVersions();

    static void canFindNewestVersion();

    void canFetchNewestVersion();
};

void TestUpdateManager::compareVersions_data()
{
    QTest::addColumn<QString>("version1");
    QTest::addColumn<QString>("version2");
    QTest::addColumn<bool>("isNewer");

    QTest::newRow("newer") << "2.0.1" << "1.2.0" << true;
    QTest::newRow("newer") << "1.2.1" << "1.2.0" << true;
    QTest::newRow("newer") << "1.3.0" << "1.2.0" << true;
    QTest::newRow("newer") << "1.1"   << "1.0.2" << true;
    QTest::newRow("newer") << "2"     << "1.3.4" << true;
    QTest::newRow("newer") << "1.1.0" << "0.3.1.1" << true;

    QTest::newRow("older") << "1.2.1" << "1.2.2" << false;
    QTest::newRow("older") << "1.1.1" << "1.2.1" << false;
    QTest::newRow("older") << "1.0.1" << "1.0.2" << false;
    QTest::newRow("older") << "0"     << "1"     << false;
    QTest::newRow("older") << ""      << "1.0.0" << false;
    QTest::newRow("older") << "1.0"   << "1.0.1" << false;
    QTest::newRow("older") << "0.3.1.1" << "1.1.0" << false;

    QTest::newRow("newer dev") << "1.0.0-rc1"    << "1.0.0-beta3"  << true;
    QTest::newRow("newer dev") << "1.0.0-alpha1" << "0.9.3"        << true;
    QTest::newRow("newer dev") << "1.0.0-alpha2" << "1.0.0-alpha1" << true;
    QTest::newRow("newer dev") << "1.0.0-beta1"  << "1.0.0-alpha1" << true;
    QTest::newRow("newer dev") << "1.0.0-beta1"  << "1.0.0-alpha4" << true;
    QTest::newRow("newer dev") << "1.0.0"        << "1.0.0-alpha4" << true;
    QTest::newRow("newer dev") << "1.2-alpha1"   << "1.1.3"        << true;

    QTest::newRow("older dev") << "1.0.0-alpha1" << "1.0.0"        << false;
    QTest::newRow("older dev") << "1.0.0-alpha1" << "1.0.0-alpha2" << false;
    QTest::newRow("older dev") << "1.0.0-alpha1" << "1.0.0-beta1"  << false;
}

void TestUpdateManager::compareVersions()
{
    QFETCH(QString, version1);
    QFETCH(QString, version2);
    QFETCH(bool, isNewer);

    QCOMPARE(UpdateManager::compareVersions(version1, version2), isNewer);
    QCOMPARE(UpdateManager::compareVersions(version2, version1), !isNewer);
}

void TestUpdateManager::canFindNewestVersion()
{
    QStringList versions = { "1.1.0", "1.0.2", "1.0.1.0", "1.0.0.0", "0.3.3.0", "0.3.2.0", "0.3.1.1", "0.3.1.0", "0.3.00", "0.3.0.1" };

    QCOMPARE(findNewestVersion(versions), "1.1.0");
}

void TestUpdateManager::canFetchNewestVersion()
{
    QFutureWatcher<QString> watcher;
    QSignalSpy spy(&watcher, &QFutureWatcher<QString>::finished);

    auto future = fetchNewestVersion();

    watcher.setFuture(future);
    QVERIFY2(!future.isCanceled(), QStringLiteral("The QFuture object returned by fetchNewestVersion() has been canceled.").toUtf8());

    auto waitTime = WAIT_TIME_IN_MS;
    auto isReady = spy.wait(waitTime);

    QVERIFY2(isReady, QStringLiteral("fetchNewestVersion() took longer than %1 ms to respond.").arg(QString::number(waitTime)).toUtf8());
    QVERIFY(!future.result().isEmpty());
}

QTEST_MAIN(TestUpdateManager)
#include "testupdatemanager.moc"
