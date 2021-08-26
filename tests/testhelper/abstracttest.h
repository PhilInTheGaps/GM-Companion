#pragma once

#include <QObject>
#include <QFuture>
#include <QtTest>
#include <QTemporaryDir>
#include "fileaccess.h"

class AbstractTest : public QObject
{
    Q_OBJECT
public:
    AbstractTest();
    virtual ~AbstractTest() {}

protected:
    Files::FileAccess* fileAccess = nullptr;
    QTemporaryDir tempDir;

    static constexpr int WAIT_TIME_IN_MS = 10000;
    static constexpr int WAIT_TIME_IN_MS_CACHED = 10;

    template<typename T, typename F>
    void testFuture(const QFuture<T>& future, const QString& funcName, F&& testFunc, bool cached = false)
    {
        QFutureWatcher<T> watcher;
        QSignalSpy spy(&watcher, &QFutureWatcher<T>::finished);

        watcher.setFuture(future);
        QVERIFY2(!future.isCanceled(), QString("The QFuture object returned by %1 has been canceled.").arg(funcName).toUtf8());

        auto waitTime = cached ? WAIT_TIME_IN_MS_CACHED : WAIT_TIME_IN_MS;
        auto isReady = spy.wait(waitTime);
        QVERIFY2(isReady, QString("%1 took longer than %2 ms to respond.").arg(funcName, QString::number(waitTime)).toUtf8());

        testFunc();
    }

    void verifyFileContent(const QString& path, const QByteArray& content, bool cached = false);
    void verifyThatFileExists(const QString& path, bool shouldExist = true);

    virtual QString getFilePath(const QString& filename = "") const;

    static void expectWarning();

private:
    void checkOrCreateFileAccess();
};