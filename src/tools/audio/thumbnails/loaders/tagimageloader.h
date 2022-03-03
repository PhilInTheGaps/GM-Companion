#pragma once

#include <QPixmap>
#include <QFuture>
#include "../../project/audioelement.h"
#include "../../project/audiofile.h"

#include <taglib/id3v2tag.h>
#include <taglib/xiphcomment.h>
#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>

class TagImageLoader
{
public:
    static auto loadImageAsync(AudioElement *element, AudioFile *audioFile) -> QFuture<QPixmap>;
    static auto loadFromFile(const QString &path, bool isLocalFile) -> QFuture<QPixmap>;

private:
    static auto loadFromLocalFile(const QString &path) -> QFuture<QPixmap>;
    static auto loadViaTempFile(const QString &path) -> QFuture<QPixmap>;

    static auto loadFromLocalMpeg(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromId3v2(const TagLib::ID3v2::Tag *tag, const QString &path) -> QFuture<QPixmap>;
    static auto pixmapFromId3v2Frames(const TagLib::ID3v2::FrameList &frames) -> QPixmap;

    static auto loadFromOga(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromVorbis(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(const TagLib::Ogg::FLAC::File &file, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromFlac(TagLib::FLAC::File &file, const QString &path) -> QFuture<QPixmap>;
    static auto loadFromXiphComment(TagLib::Ogg::XiphComment *tag, const QString &path) -> QFuture<QPixmap>;

    static auto loadFromWav(const QString &path) -> QFuture<QPixmap>;
};
