#include <QNetworkAccessManager>
#include <QObject>
#include <QUuid>
#include <QtTest>

#include "abstractaccesstest.h"
#include "fileaccessgoogledrive.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"

using namespace Files;

constexpr auto MOCK_SERVICE = "GoogleDrive_Test";

class TestGoogleDriveAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    [[nodiscard]] auto getFilePath(const QString &filename = QLatin1String()) const -> QString override;

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    GoogleDrive *gd = nullptr;
    QString testPath;
};

auto TestGoogleDriveAccess::getFilePath(const QString &filename) const -> QString
{
    if (filename.isEmpty()) return testPath;

    return FileUtils::fileInDir(filename, testPath);
}

void TestGoogleDriveAccess::initTestCase()
{
    QCoreApplication::setApplicationName("gm-companion-test_googledrive");
    QGuiApplication::setOrganizationName("GM-Companion");
    QGuiApplication::setOrganizationDomain("gm-companion.github.io");

    networkManager = new QNetworkAccessManager(this);
    gd = new GoogleDrive(*networkManager, this);

    waitForAuthentication = true;

    fileAccess = new FileAccessGoogleDrive(*gd, this);
    testPath = QStringLiteral("gm-companion-test_") + QUuid::createUuid().toString();
    createTestDir();
    createTestFiles();
}

void TestGoogleDriveAccess::cleanupTestCase()
{
    removeTestDir();

    SettingsManager::instance()->forceSync();
}

QTEST_MAIN(TestGoogleDriveAccess)
#include "testgoogledriveaccess.moc"
