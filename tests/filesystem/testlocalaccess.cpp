#include <QtTest>
#include <QObject>

#include "abstractaccesstest.h"
#include "fileaccesslocal.h"

using namespace Files;

class TestLocalAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    void createTestDir() override;
    void removeTestDir() override {} // Temp dir is deleted automatically

private slots:
    void initTestCase();
};

void TestLocalAccess::createTestDir()
{
    // Temp file should have been created automatically
    verifyThatFileExists(getFilePath());
}

void TestLocalAccess::initTestCase()
{
    fileAccess = new FileAccessLocal(this);
    createTestFiles();
}

QTEST_GUILESS_MAIN(TestLocalAccess)
#include "testlocalaccess.moc"
