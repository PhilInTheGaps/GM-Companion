#pragma once

#include <QObject>

#include "abstracttest.h"

class AbstractAccessTest : public AbstractTest
{
    Q_OBJECT

protected:
    virtual void createTestDir();
    virtual void removeTestDir();

    virtual void createTestFile(const QString &path, const QByteArray &data);
    void createTestFiles();

    void runAllTests();

    void createDirAndVerify(const QString &path);
    void removeFileAndVerify(const QString &path);
    void saveFileAndVerify(const QString &path, const QByteArray &data);
    void copyFileAndVerify(const QString &path, const QString &copy);

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
