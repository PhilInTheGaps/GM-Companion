#include "abstractaccesstest.h"
#include "mocknextcloud.h"
#include "settings/settingsmanager.h"
#include "src/filesystem/fileaccessnextcloud.h"
#include "src/services/nextcloud/nextcloud.h"
#include <QDesktopServices>
#include <QObject>
#include <QUuid>

#ifndef QT_GUI_LIB
#define QT_GUI_LIB
#endif

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Services;

class NextcloudAccessTest : public AbstractAccessTest
{
public:
    NextcloudAccessTest()
    {
        networkManager = std::make_unique<MockNextCloud>(MOCK_DOMAIN, nullptr);
        QDesktopServices::setUrlHandler(u"http"_s, networkManager.get(), "simulateBrowser");
        QDesktopServices::setUrlHandler(u"https"_s, networkManager.get(), "simulateBrowser");

        mock = networkManager.get();
        nc = new NextCloud(MOCK_SERVICE, *mock, nullptr);
        nc->disconnectService();
        EXPECT_FALSE(nc->connected());

        // empty server url, connecting should fail
        SettingsManager::setServerUrl(QLatin1String(), MOCK_SERVICE);
        nc->connectService();
        EXPECT_FALSE(nc->connected());

        // actual url, should connect during first usage
        SettingsManager::setServerUrl(u"https://%1"_s.arg(MOCK_DOMAIN), MOCK_SERVICE);

        fileAccess = std::make_shared<FileAccessNextcloud>(*nc);
        testPath = u"gm-companion-test_"_s + QUuid::createUuid().toString();

        waitForAuthentication = true;

        createTestDir();
        createTestFiles();
    }

    ~NextcloudAccessTest() override
    {
        QDesktopServices::unsetUrlHandler(u"http"_s);
        QDesktopServices::unsetUrlHandler(u"https"_s);

        removeTestDir();
        nc->disconnectService();

        SettingsManager::instance()->forceSync();

        nc->deleteLater();
    }

protected:
    void createTestFile(const QString &path, const QByteArray &data) override
    {
        saveFileAndVerify(getFilePath(path), data);
    }

    [[nodiscard]] auto getFilePath(const QString &filename = QLatin1String("")) const -> QString override
    {
        if (filename.isEmpty()) return testPath;

        return testPath + "/" + filename;
    }

private:
    NextCloud *nc = nullptr;
    QNetworkAccessManager *mock = nullptr;
    QString testPath;

    static constexpr auto MOCK_SERVICE = "NextCloud_Test";
    static constexpr auto MOCK_DOMAIN = "nextcloud.mock";
};

TEST_F(NextcloudAccessTest, TestFileAccess)
{
    runAllTests();
}
