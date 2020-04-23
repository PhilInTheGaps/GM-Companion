#include "metadatareader.h"
#include "utils/utils.h"
#include <QMediaMetaData>
#include <QImage>

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
                qWarning() << "Could not save cover art in temp file.";
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

void MetaDataReader::updateDuration(const qint64& duration)
{
    m_metaData.length = duration;
    emit metaDataUpdated(m_metaData);
}

void MetaDataReader::clearMetaData()
{
    m_metaData.artist      = "-";
    m_metaData.album       = "-";
    m_metaData.title       = "-";
    m_metaData.cover       = "";
    m_metaData.coverUrl    = "";
    m_metaData.elementIcon = "";
    m_metaData.length      = 0;

    emit metaDataUpdated(m_metaData);
}
