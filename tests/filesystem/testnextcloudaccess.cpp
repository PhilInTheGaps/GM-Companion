#include <QtTest>
#include <QObject>
#include <QTemporaryDir>
#include <QUuid>

#include "file.h"
#include "abstractaccesstest.h"
#include "fileaccessnextcloud.h"

using namespace Files;

class TestNextcloudAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    void createTestDir() override;
    void removeTestDir() override;

    void createTestFile(const QString &path, const QByteArray &data) override;

    [[nodiscard]] auto getFilePath(const QString& filename = "") const -> QString override;

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    QString testPath;
};

void TestNextcloudAccess::createTestDir()
{
    createDirAndVerify(getFilePath());
}

void TestNextcloudAccess::removeTestDir()
{
    expectWarning();
    removeFileAndVerify(getFilePath());
}

void TestNextcloudAccess::createTestFile(const QString& path, const QByteArray& data)
{
    saveFileAndVerify(getFilePath(path), data);
}

auto TestNextcloudAccess::getFilePath(const QString& filename) const -> QString
{
    if (filename.isEmpty()) return testPath;

    return testPath + "/" + filename;
}

void TestNextcloudAccess::initTestCase()
{
    fileAccess = new FileAccessNextcloud(this);
    testPath = QStringLiteral("gm-companion-test_") + QUuid::createUuid().toString();
    createTestDir();
    createTestFiles();
}

void TestNextcloudAccess::cleanupTestCase()
{
    removeTestDir();
}

QTEST_GUILESS_MAIN(TestNextcloudAccess)
#include "testnextcloudaccess.moc"