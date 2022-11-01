#include "abstracttest.h"
#include "file.h"
#include "fileaccesslocal.h"
#include "src/addons/addonmanager.h"
#include "utils/fileutils.h"
#include <QDebug>
#include <QtTest>

using namespace Files;

AbstractTest::AbstractTest()
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
