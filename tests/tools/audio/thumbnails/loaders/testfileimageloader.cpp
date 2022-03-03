#include <QtTest>
#include <QObject>
#include "src/tools/audio/thumbnails/loaders/fileimageloader.h"
#include "src/common/utils/fileutils.h"
#include "src/filesystem/fileaccesslocal.h"
#include "tests/testhelper/abstracttest.h"

class TestFileImageLoader : public AbstractTest
{
    Q_OBJECT

private slots:
    void initTestCase();

    void loadImageAsync_data();
    void loadImageAsync();
};

void TestFileImageLoader::initTestCase()
{
    fileAccess = new Files::FileAccessLocal(this);
    Files::FileAccess::setInstance(fileAccess);
}

void TestFileImageLoader::loadImageAsync_data()
{
    QTest::addColumn<QString>("file");

    QTest::newRow("png") << "test.png";
    QTest::newRow("jpg") << "test.jpg";
    QTest::newRow("tif") << "test.tif";
    QTest::newRow("gif") << "test.gif";
}

void TestFileImageLoader::loadImageAsync()
{
    QFETCH(QString, file);

    const auto resource = FileUtils::fileInDir(file, ":/resources/images");
    auto *tempFile = copyResourceToTempFile(resource);

    const auto future = FileImageLoader::loadImageAsync(tempFile->fileName());
    testFuture(future, "loadImageAsync", [future, tempFile]() {
        tempFile->remove();
        tempFile->deleteLater();

        const auto pixmap = future.result();
        QVERIFY(!pixmap.isNull());
    });
}

QTEST_MAIN(TestFileImageLoader)
#include "testfileimageloader.moc"
