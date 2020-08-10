#include <QtTest>
#include <QObject>
#include "../../src/managers/updatemanager.h"

class TestUpdateManager : public UpdateManager
{
    Q_OBJECT

private slots:
    void compareVersions_data();
    void compareVersions();
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

    QTest::newRow("older") << "1.2.1" << "1.2.2" << false;
    QTest::newRow("older") << "1.1.1" << "1.2.1" << false;
    QTest::newRow("older") << "1.0.1" << "1.0.2" << false;
    QTest::newRow("older") << "0"     << "1"     << false;
    QTest::newRow("older") << ""      << "1.0.0" << false;
    QTest::newRow("older") << "1.0"   << "1.0.1" << false;

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
}

QTEST_APPLESS_MAIN(TestUpdateManager)
#include "testupdatemanager.moc"
