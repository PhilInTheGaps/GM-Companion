#include "src/tools/audio/project/audioelement.h"
#include "src/tools/audio/thumbnails/audiothumbnail.h"
#include "tests/testhelper/abstracttest.h"
#include <gtest/gtest.h>

class AudioThumbnailTest : public AbstractTest
{
public:
    explicit AudioThumbnailTest() : AbstractTest()
    {
        m_element = QSharedPointer<AudioElement>(
            new AudioElement(QStringLiteral("test"), AudioElement::Type::Music, QLatin1String(), nullptr));

        image0 = loadImage(":/resources/images/test.png");
        image1 = loadImage(":/resources/images/test.jpg");
        image2 = loadImage(":/resources/images/test.tif");
        image3 = loadImage(":/resources/images/test.gif");
    }

protected:
    QSharedPointer<AudioElement> m_element = nullptr;
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;

    static auto loadImage(const QString &resource) -> QPixmap;
};

auto AudioThumbnailTest::loadImage(const QString &resource) -> QPixmap
{
    QFile file(resource);
    file.open(QFile::ReadOnly);

    QPixmap pixmap;
    pixmap.loadFromData(file.readAll());

    file.close();
    return pixmap;
}

TEST_F(AudioThumbnailTest, TestAudioThumbnail)
{
    auto *thumbnail = m_element->thumbnail();

    EXPECT_TRUE(thumbnail);
    EXPECT_EQ(thumbnail->imageId(), QStringLiteral("/Music/test"));
    EXPECT_EQ(thumbnail->collageImages().length(), 0);

    const auto id = thumbnail->imageIdForReload();
    EXPECT_TRUE(thumbnail->imageIdForReload() != id);

    // Can add images, but not the same image twice
    EXPECT_TRUE(thumbnail->addCollageImage({"file0", image0}));
    EXPECT_FALSE(thumbnail->addCollageImage({"file0", image0}));
    EXPECT_FALSE(thumbnail->addCollageImage({"fileX", image0}));
    EXPECT_TRUE(thumbnail->addCollageImage({"file1", image1}));
    EXPECT_TRUE(thumbnail->addCollageImage({"file2", image2}));
    EXPECT_TRUE(thumbnail->addCollageImage({"file3", image3}));
    EXPECT_EQ(thumbnail->collageImages().length(), 4);

    // Set explicit image instead of collage
    const auto url = QStringLiteral("https://sipi.usc.edu/database/preview/textures/1.1.01.png");
    thumbnail->setRelativeUrl(url);
    EXPECT_EQ(thumbnail->relativeUrl(), url);
    EXPECT_EQ(thumbnail->absoluteUrl(), url); // for web resources relative and absolute should be the same

    const auto path = QStringLiteral("some/image.png");
    thumbnail->setRelativeUrl(path);
    EXPECT_EQ(thumbnail->relativeUrl(), path);
    EXPECT_NE(thumbnail->absoluteUrl(), path);
    EXPECT_FALSE(thumbnail->absoluteUrl().isEmpty());
}
