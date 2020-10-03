#include "metadatareader.h"
#include "utils/utils.h"
#include "logging.h"
#include <QMediaMetaData>
#include <QImage>

#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>
#include <taglib/fileref.h>

MetaDataReader::MetaDataReader(QObject *parent) : QObject(parent)
{
    m_metaData = new AudioMetaData(this);
}

void MetaDataReader::setMetaData(AudioMetaData *metaData)
{
    if (m_metaData) m_metaData->deleteLater();

    m_metaData = metaData;
    m_metaData->setParent(this);
    emit metaDataChanged();
}

/**
 * @brief Read MetaData from media
 * @param mediaPlayer Pointer to QMediaPlayer with the media object
 */
void MetaDataReader::updateMetaData(QMediaPlayer *mediaPlayer)
{
    if (!m_metaData) m_metaData = new AudioMetaData(this);

    updateDuration(mediaPlayer->duration() * 1000);
}

void MetaDataReader::updateMetaData(const QString& key, const QVariant& value)
{
    qCDebug(gmAudioMetaData()) << "Updating meta data:" << key;

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    if (key == "Title")
    {
        m_metaData->setTitle(value.toString());
    }
    else if ((key == "AlbumArtist") || (key == "Artist") || (key == "Composer"))
    {
        m_metaData->setArtist(value.toString());
    }
    else if (key == "AlbumTitle")
    {
        m_metaData->setAlbum(value.toString());
    }
    else if ((key == "CoverArtImage") || (key == "ThumbnailImage"))
    {
        if (m_coverFile) m_coverFile->deleteLater();

        m_coverFile = new QTemporaryFile(this);

        if (m_coverFile->open())
        {
            if (!value.value<QImage>().save(m_coverFile, "JPG"))
            {
                qWarning(gmAudioMetaData()) << "Could not save cover art in temp file.";
            }

            m_metaData->setCover(QUrl::fromLocalFile(m_coverFile->fileName()).toEncoded());
            m_coverFile->close();
        }
    }
    else if (key == "Type")
    {
        m_metaData->setType(value.toString());
    }
    else
    {
        return;
    }

    emit metaDataChanged();
}

/**
 * @brief Use taglib to read basic meta data. Works only with taglib 1.12-beta1 or greater.
 * This is intended to be a fallback implementation, as qmediaplayer sometimes does not recognize the tags.
 * Also this unfortunately can not read the cover art, but it's better than nothing.
 */
void MetaDataReader::updateMetaData(const QByteArray &data)
{
#if TAGLIB_MAJOR_VERSION >= 1 && TAGLIB_MINOR_VERSION >= 12

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    TagLib::ByteVector bvector(data.data(), data.length());
    TagLib::ByteVectorStream bvstream(bvector);
    TagLib::FileRef ref(&bvstream);
    auto *tag = ref.tag();

    if (!tag || tag->isEmpty()) return;

    qCDebug(gmAudioMetaData()) << "Updating meta data from data ...";

    auto title = tag->title();
    auto artist = tag->artist();
    auto album = tag->album();

    if (!title.isEmpty())  m_metaData->setTitle(QString::fromStdString(title.to8Bit(true)));
    if (!artist.isEmpty()) m_metaData->setArtist(QString::fromStdString(artist.to8Bit(true)));
    if (!album.isEmpty())  m_metaData->setAlbum(QString::fromStdString(album.to8Bit(true)));

    emit metaDataChanged();

#endif
}

void MetaDataReader::updateDuration(const qint64& duration)
{
    if (!m_metaData) m_metaData = new AudioMetaData(this);

    m_metaData->setLength(duration);
    emit metaDataChanged();
}

void MetaDataReader::clearMetaData()
{
    qCDebug(gmAudioMetaData()) << "Clearing meta data ...";

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    m_metaData->setArtist(QStringLiteral("-"));
    m_metaData->setAlbum(QStringLiteral("-"));
    m_metaData->setTitle(QStringLiteral("-"));
    m_metaData->setCover(QStringLiteral(""));
    m_metaData->setLength(0);

    emit metaDataChanged();
}
