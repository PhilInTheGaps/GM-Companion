#include "abstracttest.h"
#include "file.h"
#include "fileaccesslocal.h"
#include <QtTest>

using namespace Files;

AbstractTest::AbstractTest() {
    Q_INIT_RESOURCE(tests);
    tempDir.setAutoRemove(true);
}

void AbstractTest::verifyFileContent(const QString &path, const QByteArray &content, bool cached)
{
    checkOrCreateFileAccess();

    const auto future = File::getDataAsync(path, cached, fileAccess);
    testFuture(future, QStringLiteral("File::getDataAsync"), [future, content](){
        auto *result = future.result();
        QVERIFY2(result->success(), "File::getDataAsync did not return a valid result.");
        QCOMPARE(result->data(), content);
        result->deleteLater();
    }, cached);
}

void AbstractTest::verifyThatFileExists(const QString &path, bool shouldExist)
{
    checkOrCreateFileAccess();

    auto future = File::checkAsync(path, false, fileAccess);
    testFuture(future, QStringLiteral("File::checkAsync"), [future, path, shouldExist](){
        QVERIFY2(shouldExist == future.result()->exists(),
                 QString("Apparently the file %1 does%2 exist when it should%3.")
                 .arg(path, shouldExist ? " not" : "", shouldExist ? "" : " not").toUtf8());
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
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*"));
}
