#include "abstracttest.h"
#include "file.h"
#include "fileaccesslocal.h"
#include "src/addons/addonmanager.h"
#include "utils/fileutils.h"
#include <QDebug>
#include <QtTest>

using namespace Files;

AbstractTest::AbstractTest(QObject *parent) : QObject(parent)
{
    loadResources();

    tempDir.setAutoRemove(true);
}

void AbstractTest::verifyFileContent(const QString &path, const QByteArray &content, bool cached)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying content of file" << path << "(cached:" << cached << ")";

    const auto future = File::getDataAsync(path, cached, fileAccess);
    testFuture(
        future, QStringLiteral("File::getDataAsync"),
        [future, content]() {
            QVERIFY2(!future.isCanceled(), "QFuture is canceled!");

            auto *result = future.result();
            QVERIFY2(result->success(), "File::getDataAsync did not return a valid result.");
            QCOMPARE(result->data(), content);
            result->deleteLater();
        },
        cached);
}

void AbstractTest::verifyThatFileExists(const QString &path, bool shouldExist)
{
    checkOrCreateFileAccess();

    qDebug() << "Verifying that file" << path << (shouldExist ? "exists" : "does not exist") << "...";

    auto future = File::checkAsync(path, false, fileAccess);
    testFuture(future, QStringLiteral("File::checkAsync"), [future, path, shouldExist]() {
        QVERIFY2(!future.isCanceled(), "QFuture is canceled!");
        QVERIFY2(shouldExist == future.result()->exists(),
                 QStringLiteral("Apparently the file %1 does%2 exist when it should%3.")
                     .arg(path, shouldExist ? " not" : "", shouldExist ? "" : " not")
                     .toUtf8());
        future.result()->deleteLater();
    });
}

auto AbstractTest::getFilePath(const QString &filename) const -> QString
{
    return filename.isEmpty() ? tempDir.path() : tempDir.filePath(filename);
}

void AbstractTest::checkOrCreateFileAccess()
{
    if (!fileAccess)
    {
        fileAccess = new FileAccessLocal(this);
    }

    QVERIFY(fileAccess);
}

void AbstractTest::expectWarning()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(QStringLiteral(".*")));
}

void AbstractTest::loadResources()
{
    Q_INIT_RESOURCE(tests);
    Q_INIT_RESOURCE(libresources);
}

auto AbstractTest::copyResourceToTempFile(const QString &resource) -> QFile *
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

auto AbstractTest::readResource(const QString &path) -> QByteArray
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto content = file.readAll();
    file.close();

    return content;
}

/// Moves content of a folder into a temp dir.
/// Returns new path.
auto AbstractTest::backupUserFolder(const QString &userFolder) -> QString
{
    const QDir dir(userFolder);
    if (!dir.exists()) return QLatin1String();

    const auto folderName = FileUtils::fileName(userFolder);
    auto tempFolder = getFilePath(folderName);

    while (QFileInfo::exists(tempFolder))
    {
        tempFolder = FileUtils::incrementName(tempFolder);
    }
    dir.mkpath(tempFolder);

    moveFolderRecursively(userFolder, tempFolder);

    return tempFolder;
}

void AbstractTest::moveFolderRecursively(const QString &userFolder, const QString &backupFolder)
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

void AbstractTest::restoreUserFolder(const QString &backupFolder, const QString &destination)
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

void AbstractTest::enableTestAddons()
{
    auto manager = AddonManager::instance();
    if (manager->repositoryManager().isLoading())
    {
        QSignalSpy spy(&manager->repositoryManager(), &AddonRepositoryManager::isLoadingChanged);
        QVERIFY(spy.wait());
    }

    const auto addons = AddonManager::instance()->addons();

    for (auto *addon : addons)
    {
        if (addon && m_testAddonIds.contains(addon->id()))
        {
            m_addons << addon;

            if (!addon->isInstalled())
            {
                testFuture(manager->installAsync(addon), QStringLiteral("installAsync"), [manager, addon]() {
                    QVERIFY(addon->isInstalled());
                    QVERIFY(manager->setAddonEnabled(addon, true));
                });
            }
            else
            {
                QVERIFY(manager->setAddonEnabled(addon, true));
            }
        }
    }

    QCOMPARE(m_addons.length(), m_testAddonIds.length());
}

void AbstractTest::disableTestAddons()
{
    for (auto *addon : qAsConst(m_addons))
    {
        if (!addon->isLocal())
        {
            QVERIFY(AddonManager::instance()->uninstall(addon));
        }

        QVERIFY(AddonManager::instance()->setAddonEnabled(addon, false));
    }
}

auto AbstractTest::testAddons() const -> QList<Addon *>
{
    return m_addons;
}
