#include <QtTest>
#include <QObject>
#include "utils/processinfo.h"

class TestProcessInfo : public QObject
{
    Q_OBJECT
public:
    TestProcessInfo() = default;

private slots:
    void isProcessRunning();
};

void TestProcessInfo::isProcessRunning()
{
    QVERIFY(ProcessInfo::isProcessRunning("test_processinfo"));
    QVERIFY(!ProcessInfo::isProcessRunning("this_is_not_running"));
}

QTEST_APPLESS_MAIN(TestProcessInfo)
#include "testprocessinfo.moc"
