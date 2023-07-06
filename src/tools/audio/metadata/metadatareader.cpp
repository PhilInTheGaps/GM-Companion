#include "metadatareader.h"
#include <QImage>
#include <QLoggingCategory>
#include <QMediaMetaData>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>

using namespace Qt::Literals::StringLiterals;

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

void MetaDataReader::updateMetaData(const QMediaMetaData &metaData)
{
    if (!m_metaData) m_metaData = new AudioMetaData(this);

    foreach (auto key, metaData.keys())
    {
        updateMetaData(key, metaData.value(key));
    }
}

void MetaDataReader::updateMetaData(QMediaMetaData::Key key, const QVariant &value)
{
    if (!value.isValid() || value.isNull()) return;

    switch (key)
    {
    case QMediaMetaData::Title:
        m_metaData->title(value.toString());
        break;
    case QMediaMetaData::Author:
    case QMediaMetaData::AlbumArtist:
    case QMediaMetaData::Composer:
    case QMediaMetaData::LeadPerformer:
        m_metaData->artist(value.toString());
        break;
    case QMediaMetaData::AlbumTitle:
        m_metaData->album(value.toString());
    case QMediaMetaData::MediaType:
        m_metaData->type(value.toString());
        break;
    case QMediaMetaData::ThumbnailImage:
    case QMediaMetaData::CoverArtImage: {
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
        break;
    }
    default:
        break;
    }
}

void MetaDataReader::updateMetaData(const QString &key, const QVariant &value)
{
    qCDebug(gmAudioMetaData()) << "Updating meta data:" << key;

    if (!m_metaData) m_metaData = new AudioMetaData(this);

    if (key == "Title"_L1)
    {
        updateMetaData(QMediaMetaData::Title, value);
        return;
    }

    if (key == "AlbumArtist"_L1)
    {
        updateMetaData(QMediaMetaData::AlbumArtist, value);
        return;
    }

    if (key == "Artist"_L1)
    {
        updateMetaData(QMediaMetaData::Author, value);
        return;
    }

    if (key == "Composer"_L1)
    {
        updateMetaData(QMediaMetaData::Composer, value);
        return;
    }

    if (key == "AlbumTitle"_L1)
    {
        updateMetaData(QMediaMetaData::AlbumTitle, value);
        return;
    }

    if (key == "ThumbnailImage"_L1)
    {
        updateMetaData(QMediaMetaData::ThumbnailImage, value);
        return;
    }

    if (key == "CoverArtImage"_L1)
    {
        updateMetaData(QMediaMetaData::CoverArtImage, value);
        return;
    }

    if (key == "Type"_L1)
    {
        updateMetaData(QMediaMetaData::MediaType, value);
        return;
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

    m_metaData->artist(u"-"_s);
    m_metaData->album(u"-"_s);
    m_metaData->title(u"-"_s);
    m_metaData->cover(u""_s);
    m_metaData->length(0);

    emit metaDataChanged();
}
