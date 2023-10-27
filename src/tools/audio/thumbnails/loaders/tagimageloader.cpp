#include "tagimageloader.h"
#include "../audiothumbnailcache.h"
#include "common/settings/settingsmanager.h"
#include "common/utils/fileutils.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include <QLoggingCategory>
#include <QTemporaryFile>
#include <QUuid>
#include <QtConcurrent/QtConcurrent>
#include <taglib/attachedpictureframe.h>
#include <taglib/taglib.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioTagImageLoader, "gm.audio.thumbnails.loaders.tag")

using namespace TagLib;

auto TagImageLoader::loadImageAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>
{
    // Paranoid pointer check
    if (!audioFile) return QtFuture::makeReadyFuture(QPixmap());

    if (audioFile->source() != AudioFile::Source::File)
    {
        qCCritical(gmAudioTagImageLoader())
            << "Can not read image from tags, as audio file" << audioFile->url() << "is not a local file";
        return QtFuture::makeReadyFuture(QPixmap());
    }

    // Resolve file path
    auto path = SettingsManager::getPath(AudioElement::typeToSettings(element->type()));
    path = FileUtils::fileInDir(audioFile->url(), path);

    // Try to retrieve image from cache
    QPixmap pixmap;
    if (AudioThumbnailCache::tryGet(path, &pixmap))
    {
        return QtFuture::makeReadyFuture(pixmap);
    }

    const auto isLocalFile = SettingsManager::instance()->get(u"cloudMode"_s, u"local"_s) == "local"_L1;
    return QtConcurrent::run(loadFromFile, path, isLocalFile).unwrap();
}

auto TagImageLoader::loadFromFile(const QString &path, bool isLocalFile) -> QFuture<QPixmap>
{
    if (!isLocalFile) return loadViaTempFile(path);

    return loadFromLocalFile(path);
}

auto TagImageLoader::loadFromData(const QString &path, const QByteArray &data) -> QFuture<QPixmap>
{
    const ByteVector bvector(data.data(), data.length());
    auto bvstream = std::make_unique<ByteVectorStream>(bvector);

    return loadFromData(path, std::move(bvstream));
}

auto TagImageLoader::loadFromData(const QString &path, std::unique_ptr<ByteVectorStream> data) -> QFuture<QPixmap>
{
    const auto mimeType = FileUtils::getMimeType(path);

    switch (mimeType)
    {
    case FileUtils::MimeType::MPEG: {
        auto mpeg = std::make_unique<MPEG::File>(data.get(), ID3v2::FrameFactory::instance());
        return loadFromMpeg(std::move(mpeg), path);
    }
    case FileUtils::MimeType::OGA: {
        Ogg::FLAC::File flac(data.get());
        if (flac.isValid())
        {
            return loadFromFlac(flac, path);
        }

        Ogg::Vorbis::File vorbis(data.get());
        return loadFromVorbis(vorbis, path);
    }
    case FileUtils::MimeType::Vorbis: {
        Ogg::Vorbis::File vorbis(data.get());
        return loadFromVorbis(vorbis, path);
    }
    case FileUtils::MimeType::FLAC: {
        FLAC::File flac(data.get(), ID3v2::FrameFactory::instance());
        if (flac.isValid()) return loadFromFlac(flac, path);

        Ogg::FLAC::File oggflac(data.get());
        return loadFromFlac(oggflac, path);
    }
    case FileUtils::MimeType::WAV: {
        RIFF::WAV::File wav(data.get());
        return loadFromWav(wav, path);
    }
    default:
        qCDebug(gmAudioTagImageLoader()) << "Could not load image from" << path << "mime type is not supported yet";
        return QtFuture::makeReadyFuture(QPixmap());
    }
}

auto TagImageLoader::loadViaTempFile(const QString &path) -> QFuture<QPixmap>
{
    auto future = Files::File::getDataAsync(path);

    return future
        .then([path](const Files::FileDataResult &result) {
            auto fileName = FileUtils::fileName(path);

#ifdef Q_OS_WIN
            fileName.push_front(QUuid::createUuid().toString(QUuid::Id128));
            QFile tempFile(FileUtils::fileInDir(fileName, QDir::tempPath()));
            tempFile.open(QIODevice::WriteOnly);
#else
            auto tempFile = QTemporaryFile(u"%1/XXXXXX.%2"_s.arg(QDir::tempPath(), fileName));
            tempFile.setAutoRemove(false);
            tempFile.open();
#endif
            tempFile.write(result.data());
            tempFile.close();

            return loadFromLocalFile(tempFile.fileName());
        })
        .unwrap();
}

auto TagImageLoader::loadFromLocalFile(const QString &path) -> QFuture<QPixmap>
{
    const auto mimeType = FileUtils::getMimeType(path);

    switch (mimeType)
    {
    case FileUtils::MimeType::MPEG:
        return loadFromMpeg(path);
    case FileUtils::MimeType::OGA:
        return loadFromOga(path);
    case FileUtils::MimeType::Vorbis:
        return loadFromVorbis(path);
    case FileUtils::MimeType::FLAC:
        return loadFromFlac(path);
    case FileUtils::MimeType::WAV:
        return loadFromWav(path);
    default:
        qCDebug(gmAudioTagImageLoader()) << "Could not load image from" << path << "mime type is not supported yet";
        return QtFuture::makeReadyFuture(QPixmap());
    }
}

auto TagImageLoader::loadFromMpeg(const QString &path) -> QFuture<QPixmap>
{
    auto mpeg = std::make_unique<MPEG::File>(QFile::encodeName(path).constData());
    return loadFromMpeg(std::move(mpeg), path);
}

auto TagImageLoader::loadFromMpeg(std::unique_ptr<MPEG::File> mpeg, const QString &path) -> QFuture<QPixmap>
{
    if (!mpeg->isValid())
    {
        qCCritical(gmAudioTagImageLoader) << "File could not be opened by TagLib!" << path;
        return QtFuture::makeReadyFuture(QPixmap());
    }

    if (mpeg->hasID3v2Tag())
    {
        return loadFromId3v2(mpeg->ID3v2Tag(), path);
    }

    qCDebug(gmAudioTagImageLoader) << "File does not contain supported tags" << path;
    return QtFuture::makeReadyFuture(QPixmap());
}

auto TagImageLoader::loadFromId3v2(const ID3v2::Tag *tag, const QString &path) -> QFuture<QPixmap>
{
    if (!tag)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read id3v2 meta data tags from file" << path;
        return QtFuture::makeReadyFuture(QPixmap());
    }

    // Get frames from tag for image
    const auto frames = tag->frameList("APIC");
    if (frames.isEmpty())
    {
        qCDebug(gmAudioTagImageLoader) << "Meta data tags (id3v2) do not contain images" << path;
        return QtFuture::makeReadyFuture(QPixmap());
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
        qCWarning(gmAudioTagImageLoader())
            << "Image exists in metadata, but could it could not be read correctly:" << path;
    }

    return QtFuture::makeReadyFuture(image);
}

auto TagImageLoader::pixmapFromId3v2Frames(const ID3v2::FrameList &frames) -> QPixmap
{
    QPixmap result;

    for (auto *frame : frames)
    {
        const auto *pictureFrame = dynamic_cast<ID3v2::AttachedPictureFrame *>(frame);

        if (!pictureFrame)
        {
            qCCritical(gmAudioTagImageLoader()) << "Could not cast id3v2 APIC frame into ID3v2::AttachedPictureFrame";
            continue;
        }

        if (pictureFrame->type() == ID3v2::AttachedPictureFrame::FrontCover ||
            pictureFrame->type() == ID3v2::AttachedPictureFrame::Other)
        {
            const auto format = pictureFrame->mimeType().toCString(true);
            result.loadFromData(QByteArray::fromRawData(pictureFrame->picture().data(),
                                                        static_cast<int>(pictureFrame->picture().size())),
                                format);

            if (!result.isNull()) break;
        }
    }

    return result;
}

auto TagImageLoader::loadFromOga(const QString &path) -> QFuture<QPixmap>
{
    // Can be of type FLAC or Vorbis, check FLAC first
    auto flac = Ogg::FLAC::File(QFile::encodeName(path).constData());
    if (flac.isValid())
    {
        return loadFromFlac(flac, path);
    }

    // File is of type vorbis
    return loadFromVorbis(path);
}

auto TagImageLoader::loadFromVorbis(const QString &path) -> QFuture<QPixmap>
{
    const auto file = Ogg::Vorbis::File(QFile::encodeName(path).constData());
    return loadFromVorbis(file, path);
}

auto TagImageLoader::loadFromVorbis(const Ogg::Vorbis::File &file, const QString &path) -> QFuture<QPixmap>
{
    if (!file.isValid())
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read tags from vorbis (ogg) file" << path;
        return QtFuture::makeReadyFuture(QPixmap());
    }

    return loadFromXiphComment(file.tag(), path);
}

auto TagImageLoader::loadFromFlac(const QString &path) -> QFuture<QPixmap>
{
    auto flac = FLAC::File(QFile::encodeName(path).constData());
    if (flac.isValid())
    {
        return loadFromFlac(flac, path);
    }

    auto oggflac = Ogg::FLAC::File(QFile::encodeName(path).constData());
    if (oggflac.isValid())
    {
        return loadFromFlac(oggflac, path);
    }

    return QtFuture::makeReadyFuture(QPixmap());
}

auto TagImageLoader::loadFromFlac(const Ogg::FLAC::File &file, const QString &path) -> QFuture<QPixmap>
{
    if (file.hasXiphComment())
    {
        return loadFromXiphComment(file.tag(), path);
    }

    qCDebug(gmAudioTagImageLoader) << "File does not contain supported tags" << path;
    return QtFuture::makeReadyFuture(QPixmap());
}

auto TagImageLoader::loadFromFlac(FLAC::File &file, const QString &path) -> QFuture<QPixmap>
{
    if (file.hasID3v2Tag())
    {
        return loadFromId3v2(file.ID3v2Tag(), path);
    }

    if (file.hasXiphComment())
    {
        return loadFromXiphComment(file.xiphComment(), path);
    }

    qCDebug(gmAudioTagImageLoader) << "File does not contain supported tags" << path;
    return QtFuture::makeReadyFuture(QPixmap());
}

auto TagImageLoader::loadFromXiphComment(Ogg::XiphComment *tag, const QString &path) -> QFuture<QPixmap>
{
    if (!tag)
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read XiphComment meta data tags from file" << path;
        return QtFuture::makeReadyFuture(QPixmap());
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
                return QtFuture::makeReadyFuture(result);
            }
            else
            {
                qCWarning(gmAudioTagImageLoader())
                    << "Image exists in XiphComment metadata, but could it could not be read correctly:" << path;
            }
        }
    }

    qCDebug(gmAudioTagImageLoader) << "Could not find image in XiphComment meta data" << tag->pictureList().size()
                                   << path;
    return QtFuture::makeReadyFuture(QPixmap());
}

auto TagImageLoader::loadFromWav(const QString &path) -> QFuture<QPixmap>
{
    const auto file = RIFF::WAV::File(QFile::encodeName(path).constData());
    return loadFromWav(file, path);
}

auto TagImageLoader::loadFromWav(const TagLib::RIFF::WAV::File &file, const QString &path) -> QFuture<QPixmap>
{
    if (!file.isValid())
    {
        qCWarning(gmAudioTagImageLoader) << "Could not read tags from wav file" << path;
        return QtFuture::makeReadyFuture(QPixmap());
    }

    if (file.hasID3v2Tag())
    {
        return loadFromId3v2(file.ID3v2Tag(), path);
    }

    return QtFuture::makeReadyFuture(QPixmap());
}
