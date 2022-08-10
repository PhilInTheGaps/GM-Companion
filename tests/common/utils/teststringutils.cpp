#include <QtTest>
#include <QObject>
#include "utils/stringutils.h"

class TestStringUtils : public QObject
{
    Q_OBJECT
public:
    TestStringUtils() = default;

private slots:
    void rot13_data();
    void rot13();

    void hasWildcardMatch_data();
    void hasWildcardMatch();

    void canConstructCompileTimeString();
};

void TestStringUtils::rot13_data()
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

void TestStringUtils::rot13()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(StringUtils::rot13(input), output);
}

void TestStringUtils::hasWildcardMatch_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("wildcard");
    QTest::addColumn<bool>("output");

    QTest::newRow("mp3 file match") << "file.mp3" << "*.mp3" << true;
    QTest::newRow("mp3 file no match") << "file.mp3_" << "*.mp3" << false;
    QTest::newRow("doc file match") << "file.doc" << "*.doc" << true;
    QTest::newRow("doc file no match") << "file.not_a_doc" << "*.doc" << false;
}

void TestStringUtils::hasWildcardMatch()
{
    QFETCH(QString, string);
    QFETCH(QString, wildcard);
    QFETCH(bool, output);

    QCOMPARE(StringUtils::hasWildcardMatch(string, wildcard), output);
}

constexpr ConstQString constString = "Test String";

void TestStringUtils::canConstructCompileTimeString()
{
    QCOMPARE(constString.data(), "Test String");
}

QTEST_APPLESS_MAIN(TestStringUtils)
#include "teststringutils.moc"
