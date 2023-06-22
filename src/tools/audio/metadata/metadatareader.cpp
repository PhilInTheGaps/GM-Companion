#include "metadatareader.h"
#include <QImage>
#include <QLoggingCategory>
#include <QMediaMetaData>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>

Q_LOGGING_CATEGORY(gmAudioMetaData, "gm.audio.metadata")

MetaDataReader::MetaDataReader(QObject *parent) : QObject(parent), m_metaData(new AudioMetaData(this))
{
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

void MetaDataReader::updateMetaData(const QString &key, const QVariant &value)
{
    qCDebug(gmAudioMetaData()) << "Updating meta data:" << key;

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    if (key == QLatin1String("Title"))
    {
        m_metaData->title(value.toString());
    }
    else if ((key == QLatin1String("AlbumArtist")) || (key == QLatin1String("Artist")) ||
             (key == QLatin1String("Composer")))
    {
        m_metaData->artist(value.toString());
    }
    else if (key == QLatin1String("AlbumTitle"))
    {
        m_metaData->album(value.toString());
    }
    else if ((key == QLatin1String("CoverArtImage")) || (key == QLatin1String("ThumbnailImage")))
    {
        if (m_coverFile) m_coverFile->deleteLater();

        m_coverFile = new QTemporaryFile(this);

        if (m_coverFile->open())
        {
            if (!value.value<QImage>().save(m_coverFile, "JPG"))
            {
                qWarning(gmAudioMetaData()) << "Could not save cover art in temp file.";
            }

            m_metaData->cover(QUrl::fromLocalFile(m_coverFile->fileName()).toEncoded());
            m_coverFile->close();
        }
    }
    else if (key == QLatin1String("Type"))
    {
        m_metaData->type(value.toString());
    }
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

    const TagLib::ByteVector bvector(data.data(), data.length());
    TagLib::ByteVectorStream bvstream(bvector);
    const TagLib::FileRef ref(&bvstream);
    auto *tag = ref.tag();

    if (!tag || tag->isEmpty()) return;

    qCDebug(gmAudioMetaData()) << "Updating meta data from data ...";

    auto title = tag->title();
    auto artist = tag->artist();
    auto album = tag->album();

    if (!title.isEmpty()) m_metaData->title(QString::fromStdString(title.to8Bit(true)));
    if (!artist.isEmpty()) m_metaData->artist(QString::fromStdString(artist.to8Bit(true)));
    if (!album.isEmpty()) m_metaData->album(QString::fromStdString(album.to8Bit(true)));

    emit metaDataChanged();

#endif
}

void MetaDataReader::updateDuration(qint64 duration)
{
    if (!m_metaData) m_metaData = new AudioMetaData(this);

    m_metaData->length(duration);
    emit metaDataChanged();
}

void MetaDataReader::clearMetaData()
{
    qCDebug(gmAudioMetaData()) << "Clearing meta data ...";

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    m_metaData->artist(QStringLiteral("-"));
    m_metaData->album(QStringLiteral("-"));
    m_metaData->title(QStringLiteral("-"));
    m_metaData->cover(QLatin1String(""));
    m_metaData->length(0);

    emit metaDataChanged();
}
