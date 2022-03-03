#include <QtTest>
#include <QObject>
#include "src/tools/audio/thumbnails/loaders/tagimageloader.h"
#include "src/common/utils/fileutils.h"
#include "tests/testhelper/abstracttest.h"

class TestTagImageLoader : public AbstractTest
{
    Q_OBJECT

private slots:
    void loadFromFile_data();
    void loadFromFile();
};

void TestTagImageLoader::loadFromFile_data()
{
    QTest::addColumn<QString>("file");

    QTest::newRow("MP3") << "test.mp3";
    QTest::newRow("OGG") << "test.ogg";
    QTest::newRow("WAV") << "test.wav";
}

void TestTagImageLoader::loadFromFile()
{
    QFETCH(QString, file);

    const auto resource = FileUtils::fileInDir(file, ":/resources/audiofiles");
    auto *tempFile = copyResourceToTempFile(resource);

    const auto future = TagImageLoader::loadFromFile(tempFile->fileName(), true);
    testFuture(future, "loadFromFile", [future, tempFile]() {
        tempFile->remove();
        tempFile->deleteLater();
        const auto pixmap = future.result();
        QVERIFY(!pixmap.isNull());
    });
}

QTEST_MAIN(TestTagImageLoader)
#include "testtagimageloader.moc"
