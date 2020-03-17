#include <QtTest>

// #include "../src/managers/updatemanager.h"

class TestUpdateManager : public QObject {
    Q_OBJECT

public:

    TestUpdateManager();
    ~TestUpdateManager();

private:

private slots:

    void test_case1();
};

TestUpdateManager::TestUpdateManager()
{
}

TestUpdateManager::~TestUpdateManager()
{
}

void TestUpdateManager::test_case1()
{
    QString str = "Hello";

    QCOMPARE(str.toUpper(), QString("HELLO"));
}

// QTEST_APPLESS_MAIN(TestUpdateManager)

#include "tst_updatemanager.moc"
