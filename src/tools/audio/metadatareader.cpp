#include "metadatareader.h"
#include "utils/utils.h"
#include "logging.h"
#include <QMediaMetaData>
#include <QImage>

#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>
#include <taglib/fileref.h>

MetaDataReader::MetaDataReader(QObject *parent) : QObject(parent) {}

void MetaDataReader::setMetaData(MetaData m)
{
    if (m.type.isEmpty()) m.type = m_metaData.type;

    m_metaData = m;
    emit metaDataUpdated(m);
}

/**
 * @brief Read MetaData from media
 * @param mediaPlayer Pointer to QMediaPlayer with the media object
 */
void MetaDataReader::updateMetaData(QMediaPlayer *mediaPlayer)
{
    m_metaData.type   = mediaPlayer->objectName().isEmpty() ? m_metaData.type : mediaPlayer->objectName();
    m_metaData.length = mediaPlayer->duration() * 1000;

    emit metaDataUpdated(m_metaData);
}

void MetaDataReader::updateMetaData(const QString& key, const QVariant& value)
{
    qCDebug(gmAudioMetaData()) << "Updating meta data:" << key;

    if (key == "Title")
    {
        m_metaData.title = value.toString();
    }
    else if ((key == "AlbumArtist") || (key == "Artist") || (key == "Composer"))
    {
        m_metaData.artist = value.toString();
    }
    else if (key == "AlbumTitle")
    {
        m_metaData.album = value.toString();
    }
    else if ((key == "CoverArtImage") || (key == "ThumbnailImage"))
    {
        if (m_coverFile.open())
        {
            if (!value.value<QImage>().save(&m_coverFile, "JPG"))
            {
                qWarning(gmAudioMetaData()) << "Could not save cover art in temp file.";
            }

            m_coverFile.close();
            m_metaData.cover    = Utils::stringFromImage(QPixmap::fromImage(value.value<QImage>()));
            m_metaData.coverUrl = QUrl::fromLocalFile(m_coverFile.fileName()).toEncoded();
        }
    }
    else
    {
        return;
    }

    emit metaDataUpdated(m_metaData);
}

/**
 * @brief Use taglib to read basic meta data. Works only with taglib 1.12-beta1 or greater.
 * This is intended to be a fallback implementation, as qmediaplayer sometimes does not recognize the tags.
 * Also this unfortunately can not read the cover art, but it's better than nothing.
 */
void MetaDataReader::updateMetaData(const QByteArray &data)
{
    TagLib::ByteVector bvector(data.data(), data.length());
    TagLib::ByteVectorStream bvstream(bvector);
    TagLib::FileRef ref(&bvstream);
    auto *tag = ref.tag();

    if (!tag || tag->isEmpty()) return;

    qCDebug(gmAudioMetaData()) << "Updating meta data from data ...";

    auto title = tag->title();
    auto artist = tag->artist();
    auto album = tag->album();

    if (!title.isEmpty())  m_metaData.title  = QString::fromStdString(title.to8Bit(true));
    if (!artist.isEmpty()) m_metaData.artist = QString::fromStdString(artist.to8Bit(true));
    if (!album.isEmpty())  m_metaData.album  = QString::fromStdString(album.to8Bit(true));

    emit metaDataUpdated(m_metaData);
}

void MetaDataReader::updateDuration(const qint64& duration)
{
    m_metaData.length = duration;
    emit metaDataUpdated(m_metaData);
}

void MetaDataReader::clearMetaData()
{
    qCDebug(gmAudioMetaData()) << "Clearing meta data ...";

    m_metaData.artist      = "-";
    m_metaData.album       = "-";
    m_metaData.title       = "-";
    m_metaData.cover       = "";
    m_metaData.coverUrl    = "";
    m_metaData.elementIcon = "";
    m_metaData.length      = 0;

    emit metaDataUpdated(m_metaData);
}
