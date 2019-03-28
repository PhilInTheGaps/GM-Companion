#include "metadatareader.h"
#include <QDebug>
#include <QMediaMetaData>
#include <QBuffer>

MetaDataReader::MetaDataReader(QObject *parent) : QObject(parent)
{
    qDebug() << "Starting MetaDataReader ...";
}

void MetaDataReader::setMetaData(MetaData m)
{
    emit metaDataUpdated(m);
}

/**
 * @brief Get the cover image from meta data
 * @param mediaPlayer Pointer to QMediaPlayer with the media object
 * @return Url of album cover
 */
QString MetaDataReader::convertCoverImage(QMediaPlayer *mediaPlayer)
{
    if (mediaPlayer->availableMetaData().contains("CoverArtImage"))
    {
        QImage myImage = mediaPlayer->metaData(QMediaMetaData::CoverArtImage).value<QImage>();
        QByteArray bArray;
        QBuffer    buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        myImage.save(&buffer, "JPEG");

        QString image("data:image/jpg;base64,");
        image.append(QString::fromLatin1(bArray.toBase64().data()));
        return image;
    }
    // Windows somehow uses ThumbnailImage instead,
    // the documentation says this is a photo metadata attribute but whatever. It works somehow.
    else if (mediaPlayer->availableMetaData().contains("ThumbnailImage"))
    {
        QImage myImage = mediaPlayer->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
        QByteArray bArray;
        QBuffer    buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        myImage.save(&buffer, "JPEG");

        QString image("data:image/jpg;base64,");
        image.append(QString::fromLatin1(bArray.toBase64().data()));
        return image;
    }
    else
    {
        return "";
    }
}

/**
 * @brief Read MetaData from media
 * @param mediaPlayer Pointer to QMediaPlayer with the media object
 */
void MetaDataReader::updateMetaData(QMediaPlayer *mediaPlayer)
{
    MetaData m;

    m.title  = mediaPlayer->metaData(QMediaMetaData::Title).toString();
    m.album  = mediaPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
    m.artist = mediaPlayer->metaData(QMediaMetaData::Author).toString();
    m.type   = mediaPlayer->objectName();

    if (m.artist.isEmpty()) m.artist = mediaPlayer->metaData(QMediaMetaData::AlbumArtist).toString();

    if (m.artist.isEmpty()) m.artist = mediaPlayer->metaData(QMediaMetaData::Composer).toString();

    m.cover = convertCoverImage(mediaPlayer);

    // Fill empty fields with "-"
    if (m.title.isEmpty()) m.title = "-";

    if (m.artist.isEmpty()) m.artist = "-";

    if (m.album.isEmpty()) m.album = "-";

    emit metaDataUpdated(m);
}
