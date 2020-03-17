#include <QtTest>

class Test : public QObject {
    Q_OBJECT

public:

    Test();
    ~Test();

private slots:

    void test_case1();
};

Test::Test()
{
}

Test::~Test()
{
}

void Test::test_case1()
{
    QString str = "Hello";

    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
