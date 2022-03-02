#include "tagimageloader.h"
#include "common/settings/settingsmanager.h"
#include "common/utils/fileutils.h"
#include "filesystem/file.h"
#include "../audiothumbnailcache.h"

#include <QtConcurrent/QtConcurrent>
#include <QTemporaryFile>
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/wavfile.h>

#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(gmAudioTagImageLoader, "gm.audio.thumbnails.loaders.tag")

using namespace TagLib;
using namespace AsyncFuture;

auto TagImageLoader::loadImageAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>
{
    // Paranoid pointer check
    if (!audioFile) return completed(QPixmap());

    if (audioFile->source() != 0)
    {
        qCCritical(gmAudioTagImageLoader()) << "Can not read image from tags, as audio file" << audioFile->url() << "is not a local file";
        return completed(QPixmap());
    }

    // Resolve file path
    auto path = SettingsManager::getPath(AudioElement::typeToSettings(element->type()));
    path = FileUtils::fileInDir(audioFile->url(), path);

    // Try to retrieve image from cache
    QPixmap pixmap;
    if (AudioThumbnailCache::tryGet(path, &pixmap))
    {
        return completed(pixmap);
    }

    const auto isLocalFile = SettingsManager::getSetting("cloudMode", "local") == "local";
    return QtConcurrent::run(loadFromFile, path, isLocalFile);
}

auto TagImageLoader::loadFromFile(const QString &path, bool isLocalFile) -> QFuture<QPixmap>
{
    if (!isLocalFile) return loadViaTempFile(path);

    return loadFromLocalFile(path);
}

auto TagImageLoader::loadViaTempFile(const QString &path) -> QFuture<QPixmap>
{
    const auto future = Files::File::getDataAsync(path);

    return observe(future).subscribe([](Files::FileDataResult *result) {
        auto tempFile = QTemporaryFile();
        tempFile.setAutoRemove(false);
        tempFile.open();
        tempFile.write(result->data());
        tempFile.close();

        result->deleteLater();

        return loadFromLocalFile(tempFile.fileName());
    }).future();
}

auto TagImageLoader::loadFromLocalFile(const QString &path) -> QFuture<QPixmap>
{
    const auto mimeType = FileUtils::getMimeType(path);

    switch (mimeType)
    {
    case FileUtils::MPEG:
        return loadFromLocalMpeg(path);
    case FileUtils::OGA:
        return loadFromOga(path);
    case FileUtils::Vorbis:
        return loadFromVorbis(path);
    case FileUtils::FLAC:
        return loadFromFlac(path);
    case FileUtils::WAV:
        return loadFromWav(path);
    default:
        qCDebug(gmAudioTagImageLoader()) << "Could not load image from" << path << "mime type is not supported yet";
        return completed(QPixmap());
    }
}

auto TagImageLoader::loadFromLocalMpeg(const QString &path) -> QFuture<QPixmap>
{
    FileRef f(path.toUtf8().constData());
    auto *mpeg = dynamic_cast<TagLib::MPEG::File *>(f.file());

    if (!mpeg)
    {
        qCCritical(gmAudioTagImageLoader) << "Could not cast TagLib FileRef.file() to TagLib MPEG file" << path;
        return completed(QPixmap());
    }

    if (mpeg->hasID3v2Tag())
    {
        return loadFromId3v2(mpeg->ID3v2Tag(), path);
    }

    qCDebug(gmAudioTagImageLoader) << "File does not contain supported tags" << path;
    return completed(QPixmap());
}

auto TagImageLoader::loadFromId3v2(const TagLib::ID3v2::Tag *tag, const QString &path) -> QFuture<QPixmap>
{
    if (!tag)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read id3v2 meta data tags from file" << path;
        return completed(QPixmap());
    }

    // Get frames from tag for image
    const auto frames = tag->frameList("APIC");
    if (frames.isEmpty())
    {
        qCDebug(gmAudioTagImageLoader) << "Meta data tags (id3v2) do not contain images" << path;
        return completed(QPixmap());
    }

    // Convert image to Pixmap
    auto image = pixmapFromId3v2Frames(frames);
    if (!image.isNull())
    {
        qCDebug(gmAudioTagImageLoader) << "Successfully loaded image from audio file" << path;
        AudioThumbnailCache::instance()->insertImage(path, image);
    }
    else
    {
        qCWarning(gmAudioTagImageLoader()) << "Image exists in metadata, but could it could not be read correctly:" << path;
    }

    return completed(image);
}

auto TagImageLoader::pixmapFromId3v2Frames(const TagLib::ID3v2::FrameList &frames) -> QPixmap
{
    QPixmap result;

    for (auto *frame : frames)
    {
        const auto *pictureFrame = dynamic_cast<ID3v2::AttachedPictureFrame*>(frame);

        if (!pictureFrame)
        {
            qCCritical(gmAudioTagImageLoader()) << "Could not cast id3v2 APIC frame into ID3v2::AttachedPictureFrame";
            continue;
        }

        if (pictureFrame->type() == ID3v2::AttachedPictureFrame::FrontCover ||
            pictureFrame->type() == ID3v2::AttachedPictureFrame::Other)
        {
            const auto format = pictureFrame->mimeType().toCString(true);
            result.loadFromData(QByteArray::fromRawData(pictureFrame->picture().data(), static_cast<int>(pictureFrame->picture().size())), format);

            if (!result.isNull()) break;
        }
    }

    return result;
}

auto TagImageLoader::loadFromOga(const QString &path) -> QFuture<QPixmap>
{
    // Can be of type FLAC or Vorbis, check FLAC first
    auto *flac = new TagLib::Ogg::FLAC::File(path.toUtf8().constData());
    if (flac && flac->isValid())
    {
        return loadFromFlac(flac, path);
    }

    delete flac;

    // File is of type vorbis
    return loadFromVorbis(path);
}

auto TagImageLoader::loadFromVorbis(const QString &path) -> QFuture<QPixmap>
{
    auto *file = new TagLib::Ogg::Vorbis::File(path.toUtf8().constData());
    if (!file)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read tags from vorbis (ogg) file" << path;
        delete file;
        return completed(QPixmap());
    }

    return observe(loadFromXiphComment(file->tag(), path))
            .subscribe([file](const QPixmap &pixmap) {
        delete file;
        return pixmap;
    }).future();
}

auto TagImageLoader::loadFromFlac(const QString &path) -> QFuture<QPixmap>
{
    auto *flac = new TagLib::FLAC::File(path.toUtf8().constData());
    if (flac && flac->isValid())
    {
        return loadFromFlac(flac, path);
    }
    delete flac;

    auto *oggflac = new TagLib::Ogg::FLAC::File(path.toUtf8().constData());
    if (oggflac && oggflac->isValid())
    {
        return loadFromFlac(oggflac, path);
    }
    delete oggflac;

    return completed(QPixmap());
}

auto TagImageLoader::loadFromFlac(TagLib::Ogg::FLAC::File *file, const QString &path) -> QFuture<QPixmap>
{
    return observe(loadFromXiphComment(file->tag(), path))
            .subscribe([file](const QPixmap &pixmap) {
        delete file;
        return pixmap;
    }).future();
}

auto TagImageLoader::loadFromFlac(TagLib::FLAC::File *file, const QString &path) -> QFuture<QPixmap>
{
    if (file->hasID3v2Tag())
    {
        return observe(loadFromId3v2(file->ID3v2Tag(), path))
                .subscribe([file](const QPixmap &pixmap) {
            delete file;
            return pixmap;
        }).future();
    }

    if (file->hasXiphComment())
    {
        return observe(loadFromXiphComment(file->xiphComment(), path))
                .subscribe([file](const QPixmap &pixmap) {
            delete file;
            return pixmap;
        }).future();
    }

    qCDebug(gmAudioTagImageLoader) << "File does not contain supported tags" << path;
    return completed(QPixmap());
}

auto TagImageLoader::loadFromXiphComment(TagLib::Ogg::XiphComment *tag, const QString &path) -> QFuture<QPixmap>
{
    if (!tag)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read XiphComment meta data tags from file" << path;
        return completed(QPixmap());
    }

    for (const auto *pic : tag->pictureList())
    {
        if (pic && (pic->type() == FLAC::Picture::FrontCover || pic->type() == FLAC::Picture::Other))
        {
            QPixmap result;
            const auto format = pic->mimeType().toCString(true);
            result.loadFromData(QByteArray::fromRawData(pic->data().data(), static_cast<int>(pic->data().size())),
                                format);

            if (!result.isNull())
            {
                qCDebug(gmAudioTagImageLoader) << "Successfully loaded image from audio file" << path;
                AudioThumbnailCache::instance()->insertImage(path, result);
                return completed(result);
            }
            else
            {
                qCWarning(gmAudioTagImageLoader()) << "Image exists in XiphComment metadata, but could it could not be read correctly:" << path;
            }
        }
    }

    qCDebug(gmAudioTagImageLoader) << "Could not find image in XiphComment meta data" << tag->pictureList().size() << path;
    return completed(QPixmap());
}

auto TagImageLoader::loadFromWav(const QString &path) -> QFuture<QPixmap>
{
    auto *file = new TagLib::RIFF::WAV::File(path.toUtf8().constData());

    if (!file)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read tags from wav file" << path;
        delete file;
        return completed(QPixmap());
    }

    if (file->hasID3v2Tag())
    {
        return observe(loadFromId3v2(file->ID3v2Tag(), path))
                .subscribe([file](const QPixmap &pixmap) {
            delete file;
            return pixmap;
        }).future();
    }

    delete file;
    return completed(QPixmap());
}
