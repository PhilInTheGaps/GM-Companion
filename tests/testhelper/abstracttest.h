#pragma once

#include "fileaccess.h"
#include "src/addons/addon.h"
#include <QFuture>
#include <QObject>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QtTest>

class AbstractTest : public QObject
{
    Q_OBJECT
public:
    AbstractTest();
    ~AbstractTest() override = default;

protected:
    Files::FileAccess *fileAccess = nullptr;
    QTemporaryDir tempDir;

    bool waitForAuthentication = false;

    static constexpr int WAIT_TIME_IN_MS = 10000;
    static constexpr int WAIT_TIME_IN_MS_CACHED = 10;
    static constexpr int WAIT_TIME_IN_MS_WITH_AUTH = 120000;

    template <typename T, typename F>
    void testFuture(const QFuture<T> &future, const QString &funcName, F &&testFunc, bool cached = false)
    {
        QFutureWatcher<T> watcher;
        QSignalSpy spy(&watcher, &QFutureWatcher<T>::finished);

        watcher.setFuture(future);
        QVERIFY2(!future.isCanceled(),
                 QStringLiteral("The QFuture object returned by %1 has been canceled.").arg(funcName).toUtf8());

        auto waitTime = cached                  ? WAIT_TIME_IN_MS_CACHED
                        : waitForAuthentication ? WAIT_TIME_IN_MS_WITH_AUTH
                                                : WAIT_TIME_IN_MS;
        auto isReady = spy.wait(waitTime);
        QVERIFY2(
            isReady,
            QStringLiteral("%1 took longer than %2 ms to respond.").arg(funcName, QString::number(waitTime)).toUtf8());

        waitForAuthentication = false;

        testFunc();
    }

    void verifyFileContent(const QString &path, const QByteArray &content, bool cached = false);
    void verifyThatFileExists(const QString &path, bool shouldExist = true);

    [[nodiscard]] virtual auto getFilePath(const QString &filename = QLatin1String()) const -> QString;

    static void expectWarning();

    static void loadResources();
    static auto copyResourceToTempFile(const QString &resource) -> QFile *;

    void enableTestAddons();
    void disableTestAddons();
    [[nodiscard]] auto testAddons() const -> QList<Addon *>;

private:
    void checkOrCreateFileAccess();

    QList<Addon *> m_addons;
    const QStringList m_testAddonIds = {"local.test.0", "local.test.1"};
};
