#include <QtTest>
#include <QObject>
#include <QUuid>

#include "utils/fileutils.h"
#include "file.h"
#include "abstractaccesstest.h"
#include "fileaccessgoogledrive.h"

using namespace Files;

class TestGoogleDriveAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    [[nodiscard]] auto getFilePath(const QString& filename = "") const -> QString override;

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    QString testPath;
};

auto TestGoogleDriveAccess::getFilePath(const QString& filename) const -> QString
{
    if (filename.isEmpty()) return testPath;

    return FileUtils::fileInDir(filename, testPath);
}

void TestGoogleDriveAccess::initTestCase()
{
    QCoreApplication::setApplicationName("gm-companion-test_googledrive");
    QGuiApplication::setOrganizationName("GM-Companion");
    QGuiApplication::setOrganizationDomain("gm-companion.github.io");
    waitForAuthentication = true;

    fileAccess = new FileAccessGoogleDrive(this);
    testPath = QStringLiteral("gm-companion-test_") + QUuid::createUuid().toString();
    createTestDir();
    createTestFiles();
}

void TestGoogleDriveAccess::cleanupTestCase()
{
    removeTestDir();
}

QTEST_GUILESS_MAIN(TestGoogleDriveAccess)
#include "testgoogledriveaccess.moc"
