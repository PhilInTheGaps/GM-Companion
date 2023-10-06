#pragma once

#include "../../project/audioelement.h"
#include "../../project/audiofile.h"
#include <QFuture>
#include <QPixmap>
#include <memory>
#include <taglib/flacfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/tbytevectorstream.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavfile.h>
#include <taglib/xiphcomment.h>

class TagImageLoader
{
public:
    static auto loadImageAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>;
    static auto loadFromFile(const QString &path, bool isLocalFile) -> QFuture<QPixmap>;
    static auto loadFromData(const QString &path, const QByteArray &data) -> QFuture<QPixmap>;
    static auto loadFromData(const QString &path, std::unique_ptr<TagLib::ByteVectorStream> data) -> QFuture<QPixmap>;

private:
    static auto loadFromLocalFile(const QString &path) -> QFuture<QPixmap>;
    static auto loadViaTempFile(const QString &path) -> QFuture<QPixmap>;

    static auto loadFromMpeg(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromMpeg(std::unique_ptr<TagLib::MPEG::File> mpeg, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromId3v2(const TagLib::ID3v2::Tag *tag, const QString &path) -> QFuture<QPixmap>;
    static auto pixmapFromId3v2Frames(const TagLib::ID3v2::FrameList &frames) -> QPixmap;

    static auto loadFromOga(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromVorbis(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromVorbis(const TagLib::Ogg::Vorbis::File &file, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(const TagLib::Ogg::FLAC::File &file, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(TagLib::FLAC::File &file, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromXiphComment(TagLib::Ogg::XiphComment *tag, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromWav(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromWav(const TagLib::RIFF::WAV::File &file, const QString &path) -> QFuture<QPixmap>;
};
