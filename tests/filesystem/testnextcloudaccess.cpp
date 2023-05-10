#include <QDesktopServices>
#include <QObject>
#include <QUuid>
#include <QtTest>

#include "abstractaccesstest.h"
#include "fileaccessnextcloud.h"
#include "mocknextcloud.h"
#include "settings/settingsmanager.h"

#define QT_GUI_LIB

using namespace Files;

constexpr auto MOCK_SERVICE = "NextCloud_Test";
constexpr auto MOCK_DOMAIN = "nextcloud.mock";

class TestNextcloudAccess : public AbstractAccessTest
{
    Q_OBJECT

protected:
    void createTestFile(const QString &path, const QByteArray &data) override;

    [[nodiscard]] auto getFilePath(const QString &filename = QLatin1String("")) const -> QString override;

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    NextCloud *nc = nullptr;
    QString testPath;
};

void TestNextcloudAccess::createTestFile(const QString &path, const QByteArray &data)
{
    saveFileAndVerify(getFilePath(path), data);
}

auto TestNextcloudAccess::getFilePath(const QString &filename) const -> QString
{
    if (filename.isEmpty()) return testPath;

    return testPath + "/" + filename;
}

void TestNextcloudAccess::initTestCase()
{
    networkManager = new MockNextCloud(MOCK_DOMAIN, this);
    QDesktopServices::setUrlHandler(QStringLiteral("http"), networkManager, "simulateBrowser");
    QDesktopServices::setUrlHandler(QStringLiteral("https"), networkManager, "simulateBrowser");

    nc = new NextCloud(MOCK_SERVICE, *networkManager, this);
    nc->disconnectService();
    QVERIFY(!nc->connected());

    // empty server url, connecting should fail
    SettingsManager::setServerUrl(QLatin1String(), MOCK_SERVICE);
    nc->connectService();
    QVERIFY(!nc->connected());

    // actual url, should connect during first usage
    SettingsManager::setServerUrl(QStringLiteral("https://%1").arg(MOCK_DOMAIN), MOCK_SERVICE);

    fileAccess = new FileAccessNextcloud(*nc, this);
    testPath = QStringLiteral("gm-companion-test_") + QUuid::createUuid().toString();

    waitForAuthentication = true;

    createTestDir();
    createTestFiles();
}

void TestNextcloudAccess::cleanupTestCase()
{
    removeTestDir();
    nc->disconnectService();

    SettingsManager::instance()->forceSync();
}

QTEST_GUILESS_MAIN(TestNextcloudAccess)
#include "testnextcloudaccess.moc"
