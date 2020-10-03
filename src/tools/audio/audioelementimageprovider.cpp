#include "audioelementimageprovider.h"
#include "audioicongenerator.h"

AudioElementImageProvider::AudioElementImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap AudioElementImageProvider::requestPixmap(const QString& id, QSize *size, const QSize& requestedSize)
{
    QString tempId = id.contains("?r=") ? id.left(id.lastIndexOf("?r=")) : id;

    QPixmap pixmap;

    if (AudioIconGenerator::cacheContains(tempId))
    {
        pixmap = AudioIconGenerator::readFromCache(tempId);
    }

    if (pixmap.isNull())
    {
//        pixmap = AudioIconGenerator::getPlaceholderImage();
    }

    if (size) *size = pixmap.size();

    if ((requestedSize.width() > 0) && (requestedSize.height() > 0))
    {
        pixmap = pixmap.scaled(requestedSize.width(), requestedSize.height());
    }

    return pixmap;
}

//int AudioElementImageProvider::getElementType(const QString& id)
//{
//    auto path = id.split("/");

//    // [0] = project, [1] = category, [2] = scenario, [3] = type or scenario,
//    // [4] = type again if [3] was scenario or element name if not
//    if (path.length() > 3)
//    {
//        auto type = path[3];
//        if (type == "Project")

//        bool ok   = false;
//        int  type = path[3].toInt(&ok);

//        if (ok)
//        {
//            return type;
//        }
//        else
//        {
//            type = path[4].toInt(&ok);

//            if (ok) return type;
//        }
//    }

//    return -1;
//}
