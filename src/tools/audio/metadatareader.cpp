#include "metadatareader.h"
#include "utils/utils.h"
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
        return Utils::stringFromImage(mediaPlayer->metaData(QMediaMetaData::CoverArtImage).value<QPixmap>());
    }

    // Windows somehow uses ThumbnailImage instead,
    // the documentation says this is a photo metadata attribute but whatever.
    // It works somehow.
    if (mediaPlayer->availableMetaData().contains("ThumbnailImage"))
    {
        return Utils::stringFromImage(mediaPlayer->metaData(QMediaMetaData::ThumbnailImage).value<QPixmap>());
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
void MetaDataReader::updateMetaData(QMediaPlayer *mediaPlayer, QPixmap elementIcon)
{
    MetaData m;

    m.title  = mediaPlayer->metaData(QMediaMetaData::Title).toString();
    m.album  = mediaPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
    m.artist = mediaPlayer->metaData(QMediaMetaData::Author).toString();
    m.type   = mediaPlayer->objectName();
    m.length = mediaPlayer->duration() * 1000; // Milliseconds to Microseconds

    qDebug() << "MEDIA DURATION:" << mediaPlayer->duration();

    if (m.artist.isEmpty()) m.artist = mediaPlayer->metaData(QMediaMetaData::AlbumArtist).toString();

    if (m.artist.isEmpty()) m.artist = mediaPlayer->metaData(QMediaMetaData::Composer).toString();

    m.cover       = convertCoverImage(mediaPlayer);
    m.elementIcon = Utils::stringFromImage(elementIcon);
    m_currentImageUrl.clear();

    // Fill empty fields with "-"
    if (m.title.isEmpty()) m.title = "-";

    if (m.artist.isEmpty()) m.artist = "-";

    if (m.album.isEmpty()) m.album = "-";

    emit metaDataUpdated(m);
}
