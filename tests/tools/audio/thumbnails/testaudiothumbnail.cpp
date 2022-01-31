#include <QtTest>
#include <QObject>
#include "src/tools/audio/project/audioelement.h"
#include "src/tools/audio/thumbnails/audiothumbnail.h"
#include "src/common/utils/fileutils.h"
#include "tests/testhelper/abstracttest.h"

class TestAudioThumbnail : public AbstractTest
{
    Q_OBJECT

private:
    AudioElement *m_element = nullptr;
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;

    static auto loadImage(const QString &resource) -> QPixmap;

private slots:
    void initTestCase();

    void testAudioThumbnail();
};

auto TestAudioThumbnail::loadImage(const QString &resource) -> QPixmap
{
    QFile file(resource);
    file.open(QFile::ReadOnly);

    QPixmap pixmap;
    pixmap.loadFromData(file.readAll());

    file.close();
    return pixmap;
}

void TestAudioThumbnail::initTestCase()
{
    m_element = new AudioElement(QStringLiteral("test"), AudioElement::Music, QLatin1String(), this);

    image0 = loadImage(":/resources/images/test.png");
    image1 = loadImage(":/resources/images/test.jpg");
    image2 = loadImage(":/resources/images/test.tif");
    image3 = loadImage(":/resources/images/test.gif");
}

void TestAudioThumbnail::testAudioThumbnail()
{
    auto *thumbnail = m_element->thumbnail();

    QVERIFY(thumbnail);
    QCOMPARE(thumbnail->imageId(), QStringLiteral("/Music/test"));
    QCOMPARE(thumbnail->collageImages().length(), 0);

    const auto id = thumbnail->imageIdForReload();
    QVERIFY(thumbnail->imageIdForReload() != id);

    // Can add images, but not the same image twice
    QVERIFY(thumbnail->addCollageImage({ "file0", image0 }));
    QVERIFY(!thumbnail->addCollageImage({ "file0", image0 }));
    QVERIFY(!thumbnail->addCollageImage({ "fileX", image0 }));
    QVERIFY(thumbnail->addCollageImage({ "file1", image1 }));
    QVERIFY(thumbnail->addCollageImage({ "file2", image2 }));
    QVERIFY(thumbnail->addCollageImage({ "file3", image3 }));
    QCOMPARE(thumbnail->collageImages().length(), 4);

    // Set explicit image instead of collage
    const auto url = QStringLiteral("https://sipi.usc.edu/database/preview/textures/1.1.01.png");
    thumbnail->setRelativeUrl(url);
    QCOMPARE(thumbnail->relativeUrl(), url);
    QCOMPARE(thumbnail->absoluteUrl(), url); // for web resources relative and absolute should be the same

    const auto path = QStringLiteral("some/image.png");
    thumbnail->setRelativeUrl(path);
    QCOMPARE(thumbnail->relativeUrl(), path);
    QVERIFY(thumbnail->absoluteUrl() != path);
    QVERIFY(!thumbnail->absoluteUrl().isEmpty());
}

QTEST_MAIN(TestAudioThumbnail)
#include "testaudiothumbnail.moc"
