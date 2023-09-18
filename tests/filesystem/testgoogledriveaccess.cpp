#include "abstractaccesstest.h"
#include "settings/settingsmanager.h"
#include "src/filesystem/fileaccessgoogledrive.h"
#include "src/services/google/googledrive.h"
#include "utils/fileutils.h"
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QString>
#include <QUuid>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

class GoogleDriveAccessTest : public AbstractAccessTest
{
public:
    GoogleDriveAccessTest()
    {
        QCoreApplication::setApplicationName(u"gm-companion-test_googledrive"_s);
        QGuiApplication::setOrganizationName(u"GM-Companion"_s);
        QGuiApplication::setOrganizationDomain(u"gm-companion.github.io"_s);

        networkManager = std::make_unique<QNetworkAccessManager>(nullptr);
        gd = new GoogleDrive(*networkManager.get(), nullptr);

        waitForAuthentication = true;

        fileAccess = std::make_unique<FileAccessGoogleDrive>(*gd, nullptr);
        testPath = u"gm-companion-test_"_s + QUuid::createUuid().toString();
        createTestDir();
        createTestFiles();
    }

    ~GoogleDriveAccessTest() override
    {
        removeTestDir();

        SettingsManager::instance()->forceSync();

        gd->deleteLater();
    }

protected:
    [[nodiscard]] auto getFilePath(const QString &filename = QLatin1String()) const -> QString override
    {
        if (filename.isEmpty()) return testPath;

        return FileUtils::fileInDir(filename, testPath);
    }

private:
    GoogleDrive *gd = nullptr;
    QString testPath;

    static constexpr auto MOCK_SERVICE = "GoogleDrive_Test";
};

TEST_F(GoogleDriveAccessTest, TestFileAccess)
{
    runAllTests();
}
