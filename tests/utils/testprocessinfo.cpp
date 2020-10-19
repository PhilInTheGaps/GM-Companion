#include <QtTest>
#include <QObject>
#include "../../src/utils/processinfo.h"

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
}

QTEST_APPLESS_MAIN(TestProcessInfo)
#include "testprocessinfo.moc"
