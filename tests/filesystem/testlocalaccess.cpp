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

    auto getFilePath(const QString& filename = "") const -> QString override;

private:
    QTemporaryDir dir;

private slots:
    void initTestCase();
    void cleanupTestCase();
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

auto TestLocalAccess::getFilePath(const QString& filename) const -> QString
{
    return filename.isEmpty() ? dir.path() : dir.filePath(filename);
}

void TestLocalAccess::initTestCase()
{
    fileAccess = new FileAccessLocal(this);
    createTestFiles();
}

void TestLocalAccess::cleanupTestCase()
{
    dir.remove();
}

QTEST_GUILESS_MAIN(TestLocalAccess)
#include "testlocalaccess.moc"
