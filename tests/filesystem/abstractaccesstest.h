#pragma once

#include <QtTest>
#include <QObject>
#include <functional>

#include "fileaccess.h"

class AbstractAccessTest : public QObject
{
    Q_OBJECT

protected:
    virtual void createTestDir() = 0;
    virtual void removeTestDir() = 0;

    virtual void createTestFile(const QString &path, const QByteArray &data) = 0;
    void createTestFiles();

    virtual QString getFilePath(const QString& filename = "") const = 0;

    void ignoreWarning();
    void runAllTests();

    template<typename T>
    void testFuture(const QFuture<T>& future, const QString& funcName, const std::function<void ()>& testFunc, bool cached = false);

    void verifyFileContent(const QString& path, const QByteArray& content, bool cached);
    void verifyThatFileExists(const QString& path, bool shouldExist = true);
    void createDirAndVerify(const QString& path);
    void removeFileAndVerify(const QString& path);
    void saveFileAndVerify(const QString& path, const QByteArray& data);
    void copyFileAndVerify(const QString& path, const QString& copy);

    Files::FileAccess* fileAccess = nullptr;

    static constexpr int WAIT_TIME_IN_MS = 10000;
    static constexpr int WAIT_TIME_IN_MS_CACHED = 10;

private slots:
    void getDataAsync();

    void saveAsync_data();
    void saveAsync();

    void moveAsync();
    void deleteAsync();
    void copyAsync();
    void listAsync();
    void createDirAsync();
    void checkAsync();
};
