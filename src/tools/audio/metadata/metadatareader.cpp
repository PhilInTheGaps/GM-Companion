#include "metadatareader.h"
#include "../thumbnails/loaders/tagimageloader.h"
#include "utils/stringutils.h"
#include <QImage>
#include <QLoggingCategory>
#include <QMediaMetaData>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>
#include <taglib/tbytevectorstream.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioMetaData, "gm.audio.metadata")

void MetaDataReader::setMetaData(const AudioMetaData &metaData)
{
    m_metaData.apply(metaData);
}

// void MetaDataReader::updateMetaData(QMediaPlayer *mediaPlayer)
//{
//     if (!m_metaData) m_metaData = new AudioMetaData(this);

//    setDuration(mediaPlayer->duration() * 1000);
//}

void MetaDataReader::setMetaData(QMediaMetaData::Key key, const QVariant &value)
{
    if (!value.isValid() || value.isNull()) return;

    switch (key)
    {
    case QMediaMetaData::Title:
        m_metaData.title(value.toString());
        break;
    case QMediaMetaData::Author:
    case QMediaMetaData::AlbumArtist:
    case QMediaMetaData::Composer:
    case QMediaMetaData::LeadPerformer:
        m_metaData.artist(value.toStringList());
        break;
    case QMediaMetaData::AlbumTitle:
        m_metaData.album(value.toString());
    case QMediaMetaData::ThumbnailImage:
    case QMediaMetaData::CoverArtImage: {
        if (m_coverFile) m_coverFile->deleteLater();

        m_coverFile = std::make_unique<QTemporaryFile>();

        if (m_coverFile->open())
        {
            if (!value.value<QImage>().save(m_coverFile.get(), "JPG"))
            {
                qWarning(gmAudioMetaData()) << "Could not save cover art in temp file.";
            }

            m_metaData.cover(QUrl::fromLocalFile(m_coverFile->fileName()).toEncoded());
            m_coverFile->close();
        }
        break;
    }
    default:
        break;
    }
}

void MetaDataReader::setMetaData(const QMediaMetaData &data)
{
    setMetaData(QMediaMetaData::Title, data.value(QMediaMetaData::Title));
    setMetaData(QMediaMetaData::AlbumTitle, data.value(QMediaMetaData::AlbumTitle));

    auto artist = data.value(QMediaMetaData::Author);
    if (artist.isNull()) artist = data.value(QMediaMetaData::AlbumArtist);
    if (artist.isNull()) artist = data.value(QMediaMetaData::Composer);
    if (artist.isNull()) artist = data.value(QMediaMetaData::LeadPerformer);
    setMetaData(QMediaMetaData::Author, artist);

    auto image = data.value(QMediaMetaData::CoverArtImage);
    if (image.isNull()) image = data.value(QMediaMetaData::ThumbnailImage);
    setMetaData(QMediaMetaData::CoverArtImage, image);
}

void MetaDataReader::setMetaData(const QString &key, const QVariant &value)
{
    qCDebug(gmAudioMetaData()) << "Updating meta data:" << key;

    if (key == "Title"_L1)
    {
        setMetaData(QMediaMetaData::Title, value);
        return;
    }

    if (key == "AlbumArtist"_L1)
    {
        setMetaData(QMediaMetaData::AlbumArtist, value);
        return;
    }

    if (key == "Artist"_L1)
    {
        setMetaData(QMediaMetaData::Author, value);
        return;
    }

    if (key == "Composer"_L1)
    {
        setMetaData(QMediaMetaData::Composer, value);
        return;
    }

    if (key == "AlbumTitle"_L1)
    {
        setMetaData(QMediaMetaData::AlbumTitle, value);
        return;
    }

    if (key == "ThumbnailImage"_L1)
    {
        setMetaData(QMediaMetaData::ThumbnailImage, value);
        return;
    }

    if (key == "CoverArtImage"_L1)
    {
        setMetaData(QMediaMetaData::CoverArtImage, value);
        return;
    }

    if (key == "Type"_L1)
    {
        setMetaData(QMediaMetaData::MediaType, value);
        return;
    }
}

void MetaDataReader::loadMetaData(const QString &path, const QByteArray &data)
{
    const TagLib::ByteVector bvector(data.data(), data.length());
    auto bvstream = std::make_unique<TagLib::ByteVectorStream>(bvector);
    const TagLib::FileRef ref(bvstream.get());
    auto *tag = ref.tag();

    if (!tag || tag->isEmpty()) return;

    qCDebug(gmAudioMetaData()) << "Updating meta data from data ...";

    auto title = tag->title();
    if (!title.isEmpty()) m_metaData.title(QString::fromStdString(title.to8Bit(true)));

    auto artist = tag->artist();
    if (!artist.isEmpty()) m_metaData.artist(QString::fromStdString(artist.to8Bit(true)).split(", "));

    auto album = tag->album();
    if (!album.isEmpty()) m_metaData.album(QString::fromStdString(album.to8Bit(true)));

    auto pixmap = TagImageLoader::loadFromData(path, std::move(bvstream));
    if (!pixmap.isNull())
    {
        m_metaData.cover(StringUtils::stringFromImage(pixmap));
    }
}

void MetaDataReader::setDuration(qint64 duration)
{
    m_metaData.length(duration);
}

void MetaDataReader::clearMetaData()
{
    m_metaData.clear();
}
