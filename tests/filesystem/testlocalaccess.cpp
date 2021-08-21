#include <QtTest>
#include <QObject>
#include <QTemporaryDir>

#include "abstractaccesstest.h"
#include "fileaccesslocal.h"

using namespace Files;

class TestLocalAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    void createTestDir() override;
    void removeTestDir() override {} // Temp dir is deleted automatically

    void createTestFile(const QString &path, const QByteArray &data) override;

private slots:
    void initTestCase();
};

void TestLocalAccess::createTestDir()
{
    // Temp file should have been created automatically
    verifyThatFileExists(getFilePath());
}

void TestLocalAccess::createTestFile(const QString& path, const QByteArray& data)
{
    QFile f1(getFilePath(path));
    QVERIFY(f1.open(QIODevice::WriteOnly));
    f1.write(data);
    f1.close();
}

void TestLocalAccess::initTestCase()
{
    fileAccess = new FileAccessLocal(this);
    createTestFiles();
}

QTEST_GUILESS_MAIN(TestLocalAccess)
#include "testlocalaccess.moc"
