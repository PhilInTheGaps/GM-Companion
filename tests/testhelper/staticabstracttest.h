#pragma once

#include <QSignalSpy>
#include <gtest/gtest.h>

class Addon;

class StaticAbstractTest : public ::testing::Test
{
public:
    StaticAbstractTest();

    template <typename T, typename F>
    static void testFutureNoAuth(const QFuture<T> &future, const char *funcName, F &&testFunc, bool cached = false)
    {
        QFutureWatcher<T> watcher;
        QSignalSpy spy(&watcher, &QFutureWatcher<T>::finished);
        ASSERT_TRUE(spy.isValid());

        watcher.setFuture(future);
        EXPECT_FALSE(future.isCanceled()) << "The QFuture object returned by " << funcName << " has been canceled.";

        auto waitTime = cached ? WAIT_TIME_IN_MS_CACHED : WAIT_TIME_IN_MS;
        auto isReady = spy.wait(waitTime);
        EXPECT_TRUE(isReady) << funcName << " took longer than " << waitTime << " ms to respond.";

        testFunc();
    }

    [[nodiscard]] static auto getFilePathInTempDir(const QString &filename, const QTemporaryDir &tempDir) -> QString;

protected:
    static void expectWarning();

    static void copyResourceToFile(const QString &resource, const QString &destination);
    [[nodiscard]] static auto copyResourceToTempFile(const QString &resource) -> QFile *;
    [[nodiscard]] static auto readResource(const QString &path) -> QByteArray;

    [[nodiscard]] static auto backupUserFolder(const QString &userFolder, QTemporaryDir &tempDir) -> QString;
    static void restoreUserFolder(const QString &backupFolder, const QString &destination);

    static constexpr int WAIT_TIME_IN_MS = 10000;
    static constexpr int WAIT_TIME_IN_MS_CACHED = 10;
    static constexpr int WAIT_TIME_IN_MS_WITH_AUTH = 120000;

    [[nodiscard]] static auto enableTestAddons() -> QList<Addon *>;
    static void disableTestAddons(const QList<Addon *> &testAddons);
    inline static const QStringList testAddonIds = {"local.test.0", "local.test.1"};

private:
    static void loadResources();
    static void moveFolderRecursively(const QString &userFolder, const QString &backupFolder);
};
