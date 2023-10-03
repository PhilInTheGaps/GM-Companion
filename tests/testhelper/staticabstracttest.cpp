#include "staticabstracttest.h"
#include "src/addons/addon.h"
#include "src/addons/addonmanager.h"
#include "utils/fileutils.h"
#include <QDebug>
#include <QtTest>

StaticAbstractTest::StaticAbstractTest()
{
    loadResources();
}

void StaticAbstractTest::loadResources()
{
    Q_INIT_RESOURCE(test_resources);
    Q_INIT_RESOURCE(test_resources_addons);
    Q_INIT_RESOURCE(libresources);
    Q_INIT_RESOURCE(services_resources);
}

void StaticAbstractTest::expectWarning()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(QStringLiteral(".*")));
}

auto StaticAbstractTest::getFilePathInTempDir(const QString &filename, const QTemporaryDir &tempDir) -> QString
{
    return filename.isEmpty() ? tempDir.path() : tempDir.filePath(filename);
}

void StaticAbstractTest::waitForSpy(QSignalSpy &spy, int count)
{
    ASSERT_TRUE(spy.isValid());

    if (count < 0)
    {
        ASSERT_TRUE(spy.wait());
        return;
    }

    while (spy.count() < count)
    {
        ASSERT_TRUE(spy.wait()) << "current count: " << spy.count() << ", expected count: " << count;
    }

    ASSERT_EQ(spy.count(), count);
}

auto StaticAbstractTest::copyResourceToTempFile(const QString &resource) -> QFile *
{
    QFile resourceFile(resource);

    if (!resourceFile.open(QIODevice::ReadOnly)) return nullptr;

    auto fileName = FileUtils::fileName(resource);

#ifdef Q_OS_WIN
    fileName.push_front(QUuid::createUuid().toString(QUuid::Id128));
    auto *tempFile = new QFile(FileUtils::fileInDir(fileName, QDir::tempPath()));
    tempFile->open(QIODevice::WriteOnly);
#else
    auto *tempFile = new QTemporaryFile(QStringLiteral("%1/XXXXXX.%2").arg(QDir::tempPath(), fileName));
    tempFile->open();
#endif

    tempFile->write(resourceFile.readAll());
    tempFile->close();

    resourceFile.close();
    return tempFile;
}

void StaticAbstractTest::copyResourceToFile(const QString &resource, const QString &destination)
{
    const auto data = readResource(resource);
    EXPECT_FALSE(data.isEmpty());

    QFile destinationFile(destination);
    auto dir = FileUtils::dirFromPath(destination);

    const QDir qdir(dir);
    if (!qdir.exists())
    {
        EXPECT_TRUE(qdir.mkpath(dir)) << "Could not create path: " << dir.toStdString();
    }

    EXPECT_TRUE(destinationFile.open(QIODevice::WriteOnly)) << "Could not open file " << destination.toStdString();
    destinationFile.write(data);
    destinationFile.close();
}

auto StaticAbstractTest::readResource(const QString &path) -> QByteArray
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto content = file.readAll();
    file.close();

    return content;
}

auto StaticAbstractTest::backupUserFolder(const QString &userFolder, QTemporaryDir &tempDir) -> QString
{
    const QDir dir(userFolder);
    if (!dir.exists()) return QLatin1String();

    const auto folderName = FileUtils::fileName(userFolder);
    auto tempFolder = getFilePathInTempDir(folderName, tempDir);

    while (QFileInfo::exists(tempFolder))
    {
        tempFolder = FileUtils::incrementName(tempFolder);
    }
    dir.mkpath(tempFolder);

    moveFolderRecursively(userFolder, tempFolder);

    return tempFolder;
}

void StaticAbstractTest::restoreUserFolder(const QString &backupFolder, const QString &destination)
{
    if (destination.isEmpty()) return;

    const QDir sourceDir(backupFolder);
    if (!sourceDir.exists())
    {
        qWarning() << "Could not restore user folder: path does not exist" << backupFolder << destination;
        return;
    }

    // remove any data from the user directory that the tests might have placed there
    QDir dest(destination);
    if (dest.exists()) dest.removeRecursively();

    // if nothing was backed up, don't restore anything
    if (backupFolder.isEmpty()) return;

    moveFolderRecursively(backupFolder, destination);
}

void StaticAbstractTest::moveFolderRecursively(const QString &userFolder, const QString &backupFolder)
{
    QDir dir(userFolder);
    const auto entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << "Moving" << userFolder << entries << "to" << backupFolder;

    const QDir backupDir(backupFolder);
    if (!backupDir.exists()) backupDir.mkpath(backupFolder);

    for (const auto &entry : entries)
    {
        const auto source = FileUtils::fileInDir(entry, userFolder);
        const auto destination = FileUtils::fileInDir(entry, backupFolder);

        if (!dir.rename(entry, destination))
        {
            // move recursively if simple rename failed
            moveFolderRecursively(source, destination);

            dir.removeRecursively();
        }
    }

    const QDir dir2(backupFolder);
    qDebug() << backupFolder << dir2.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
}

auto StaticAbstractTest::enableTestAddons() -> QList<Addon *>
{
    auto manager = AddonManager::instance();
    if (manager->repositoryManager().isLoading())
    {
        QSignalSpy spy(&manager->repositoryManager(), &AddonRepositoryManager::isLoadingChanged);
        EXPECT_TRUE(spy.wait());
    }

    const auto addons = AddonManager::instance()->addons();
    QList<Addon *> testAddons;
    testAddons.reserve(testAddonIds.length());

    foreach (auto *addon, addons)
    {
        if (addon && testAddonIds.contains(addon->id()))
        {
            testAddons << addon;

            if (!addon->isInstalled())
            {
                testFutureNoAuth(manager->installAsync(addon), "installAsync", [manager, addon]() {
                    EXPECT_TRUE(addon->isInstalled());
                    EXPECT_TRUE(manager->setAddonEnabled(addon, true));
                });
            }
            else
            {
                EXPECT_TRUE(manager->setAddonEnabled(addon, true));
            }
        }
    }

    EXPECT_EQ(testAddons.length(), testAddonIds.length());
    return testAddons;
}

void StaticAbstractTest::disableTestAddons(const QList<Addon *> &testAddons)
{
    foreach (auto *addon, testAddons)
    {
        if (!addon->isLocal())
        {
            EXPECT_TRUE(AddonManager::instance()->uninstall(addon));
        }

        EXPECT_TRUE(AddonManager::instance()->setAddonEnabled(addon, false));
    }
}
