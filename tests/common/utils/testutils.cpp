#include <QtTest>
#include <QObject>
#include "utils/utils.h"

class TestUtils : public QObject
{
    Q_OBJECT
public:
    TestUtils() = default;

private slots:
    void rot13_data();
    void rot13();

    void hasWildcardMatch_data();
    void hasWildcardMatch();

    void isInBounds();
};


void TestUtils::rot13_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("alphabet lower")
            << "abcdefghijklmnopqrstuvwxyz"
            << "nopqrstuvwxyzabcdefghijklm";

    QTest::newRow("alphabet upper")
            << "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            << "NOPQRSTUVWXYZABCDEFGHIJKLM";

    QTest::newRow("random string 1")
            << "DzFmnYqGVmsHcNnqDWqO"
            << "QmSzaLdTIzfUpAadQJdB";

    QTest::newRow("random string 2")
            << "bqiqxJFmqwrIkhFmFEvT"
            << "odvdkWSzdjeVxuSzSRiG";

    QTest::newRow("random string 3")
            << "qiFJglgZCVgiXLUcFmGF"
            << "dvSWtytMPItvKYHpSzTS";
}

void TestUtils::rot13()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(Utils::rot13(input), output);
}

void TestUtils::hasWildcardMatch_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("wildcard");
    QTest::addColumn<bool>("output");

    QTest::newRow("mp3 file match") << "file.mp3" << "*.mp3" << true;
    QTest::newRow("mp3 file no match") << "file.mp3_" << "*.mp3" << false;
    QTest::newRow("doc file match") << "file.doc" << "*.doc" << true;
    QTest::newRow("doc file no match") << "file.not_a_doc" << "*.doc" << false;
}

void TestUtils::hasWildcardMatch()
{
    QFETCH(QString, string);
    QFETCH(QString, wildcard);
    QFETCH(bool, output);

    QCOMPARE(Utils::hasWildcardMatch(string, wildcard), output);
}

void TestUtils::isInBounds()
{
    QList<QObject*> list;

    QVERIFY(!Utils::isInBounds(list, 0));
    QVERIFY(!Utils::isInBounds(list, 1));
    QVERIFY(!Utils::isInBounds(list, -1));
    QVERIFY(!Utils::isInBounds(list, -5));

    list.push_back(new QObject(this));
    list.push_back(new QObject(this));

    QVERIFY(Utils::isInBounds(list, 0));
    QVERIFY(Utils::isInBounds(list, 1));
    QVERIFY(!Utils::isInBounds(list, 2));
    QVERIFY(!Utils::isInBounds(list, -1));
}

QTEST_APPLESS_MAIN(TestUtils)
#include "testutils.moc"
