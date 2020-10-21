#include "audioelementimageprovider.h"
#include "audioicongenerator.h"

#include <QRegularExpression>

#define IMAGE_ID_REGEX "(.+\\/(.+)\\/.+)(\\?r=)"

AudioElementImageProvider::AudioElementImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

auto AudioElementImageProvider::requestPixmap(const QString& id, QSize *size, const QSize& requestedSize) -> QPixmap
{
    auto _id = id;
    auto type = QStringLiteral("Music");
    auto regex = QRegularExpression(IMAGE_ID_REGEX);
    auto match = regex.match(id);

    if (match.hasMatch())
    {
        _id = match.captured(1);
        type = match.captured(2);
    }

    QPixmap pixmap;

    if (AudioIconGenerator::cacheContains(_id))
    {
        pixmap = AudioIconGenerator::readFromCache(_id);
    }

    if (pixmap.isNull())
    {
        pixmap = getDefaultImage(type);
    }

    if (size) *size = pixmap.size();

    if ((requestedSize.width() > 0) && (requestedSize.height() > 0))
    {
        pixmap = pixmap.scaled(requestedSize.width(), requestedSize.height());
    }

    return pixmap;
}

auto AudioElementImageProvider::getDefaultImage(const QString &type) const -> QPixmap
{
    if (type == "Sound")
    {
        return QPixmap(":/icons/media/sound_image.png");
    }

    if (type == "Radio")
    {
        return QPixmap(":/icons/media/radio_image.png");
    }

    return QPixmap(":/icons/media/music_image.png");
}
