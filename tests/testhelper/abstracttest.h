#pragma once

#include "src/filesystem/fileaccess.h"
#include "staticabstracttest.h"
#include <QByteArray>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QNetworkAccessManager>
#include <QSignalSpy>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <gtest/gtest.h>
#include <memory>

namespace Files
{
class FileAccess;
}

class AbstractTest : public StaticAbstractTest
{
public:
    explicit AbstractTest();
    virtual ~AbstractTest();

protected:
    std::unique_ptr<QNetworkAccessManager> networkManager = nullptr;
    std::shared_ptr<Files::FileAccess> fileAccess = nullptr;
    QTemporaryDir tempDir;

    bool waitForAuthentication = false;

    template <typename T, typename F>
    void testFuture(const QFuture<T> &future, const char *funcName, F &&testFunc, bool cached = false)
    {
        QFutureWatcher<T> watcher;
        QSignalSpy spy(&watcher, &QFutureWatcher<T>::finished);
        ASSERT_TRUE(spy.isValid());

        watcher.setFuture(future);
        EXPECT_FALSE(future.isCanceled()) << "The QFuture object returned by " << funcName << " has been canceled.";

        auto waitTime = cached                  ? WAIT_TIME_IN_MS_CACHED
                        : waitForAuthentication ? WAIT_TIME_IN_MS_WITH_AUTH
                                                : WAIT_TIME_IN_MS;
        auto isReady = spy.wait(waitTime);
        EXPECT_TRUE(isReady) << funcName << " took longer than " << waitTime << " ms to respond.";

        waitForAuthentication = false;

        testFunc();
    }

    void verifyFileContent(const QString &path, const QByteArray &content,
                           Files::Options options = Files::Option::None);
    void verifyThatFileExists(const QString &path, bool shouldExist = true);

    [[nodiscard]] virtual auto getFilePath(const QString &filename = QLatin1String()) const -> QString;

    [[nodiscard]] auto backupUserFolder(const QString &userFolder) -> QString;

private:
    void checkOrCreateFileAccess();

    QString cloudMode;
};
