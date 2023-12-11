#pragma once

#include <QHash>
#include <QMediaFormat>
#include <QString>
#include <QStringList>

class FileUtils
{
public:
    static auto dirFromFolders(const QStringList &folders) -> QString;
    static auto dirFromPath(const QString &path) -> QString;
    static auto foldersFromDir(const QString &dir) -> QStringList;
    static auto suffix(const QString &fileName) -> QString;
    static auto fileName(const QString &path) -> QString;
    static auto fileName(QStringView path) -> QString;
    static auto incrementFileName(const QString &fileName) -> QString;
    static auto incrementName(const QString &name) -> QString;
    static auto fileInDir(const QString &fileName, const QString &dir) -> QString;

    enum class MimeType
    {
        Unknown,
        MPEG,
        Vorbis,
        OGA,
        FLAC,
        MPC,
        WavPack,
        SPX,
        OPUS,
        TrueAudio,
        MP4,
        ASF,
        AIFF,
        WAV,
        APE,
        MOD,
        S3M,
        IT,
        XM
    };

    [[nodiscard]] static auto getMimeType(const QString &filename) -> MimeType;
    [[nodiscard]] static auto mimeTypeStringToFileFormat(const QString &string) -> QMediaFormat::FileFormat;
    [[nodiscard]] static auto audioCodecNameToEnum(const QString &string) -> QMediaFormat::AudioCodec;

private:
    static auto splitFileNameAndSuffix(const QString &fileName) -> std::pair<QString, QString>;
    static auto incrementNumString(const QString &num) -> int;

    inline static const auto extensionToMimeType = QHash<QString, MimeType>{
        {QStringLiteral("mp3"), MimeType::MPEG},      {QStringLiteral("ogg"), MimeType::Vorbis},
        {QStringLiteral("oga"), MimeType::OGA},       {QStringLiteral("flac"), MimeType::FLAC},
        {QStringLiteral("mpc"), MimeType::MPC},       {QStringLiteral("wv"), MimeType::WavPack},
        {QStringLiteral("spx"), MimeType::SPX},       {QStringLiteral("opus"), MimeType::OPUS},
        {QStringLiteral("tta"), MimeType::TrueAudio}, {QStringLiteral("m4a"), MimeType::MP4},
        {QStringLiteral("m4r"), MimeType::MP4},       {QStringLiteral("m4b"), MimeType::MP4},
        {QStringLiteral("m4p"), MimeType::MP4},       {QStringLiteral("mp4"), MimeType::MP4},
        {QStringLiteral("3g2"), MimeType::MP4},       {QStringLiteral("m4v"), MimeType::MP4},
        {QStringLiteral("wma"), MimeType::ASF},       {QStringLiteral("asf"), MimeType::ASF},
        {QStringLiteral("aif"), MimeType::AIFF},      {QStringLiteral("aiff"), MimeType::AIFF},
        {QStringLiteral("wav"), MimeType::WAV},       {QStringLiteral("ape"), MimeType::APE},
        {QStringLiteral("mod"), MimeType::MOD},       {QStringLiteral("module"), MimeType::MOD},
        {QStringLiteral("nst"), MimeType::MOD},       {QStringLiteral("wow"), MimeType::MOD},
        {QStringLiteral("s3m"), MimeType::S3M},       {QStringLiteral("it"), MimeType::IT},
        {QStringLiteral("xm"), MimeType::XM},
    };

    inline static const auto mimeTypeToFileQtFormat = QHash<QString, QMediaFormat::FileFormat>{
        {QStringLiteral("audio/aac"), QMediaFormat::FileFormat::AAC},
        {QStringLiteral("audio/mpeg"), QMediaFormat::FileFormat::MP3},
        {QStringLiteral("audio/ogg"), QMediaFormat::FileFormat::Ogg},
        {QStringLiteral("audio/wav"), QMediaFormat::FileFormat::Wave},
        {QStringLiteral("audio/webm"), QMediaFormat::FileFormat::WebM},
        {QStringLiteral("audio/mp4"), QMediaFormat::FileFormat::Mpeg4Audio},
    };

    inline static const auto audioCodecToQtCodec = QHash<QString, QMediaFormat::AudioCodec>{
        {QStringLiteral("opus"), QMediaFormat::AudioCodec::Opus},
        {QStringLiteral("mp4a.40.1"), QMediaFormat::AudioCodec::AAC},
        {QStringLiteral("mp4a.40.2"), QMediaFormat::AudioCodec::AAC},
        {QStringLiteral("mp4a.40.17"), QMediaFormat::AudioCodec::AAC},
        {QStringLiteral("vorbis"), QMediaFormat::AudioCodec::Vorbis},
        {QStringLiteral("wma"), QMediaFormat::AudioCodec::WMA},
        {QStringLiteral("ac3"), QMediaFormat::AudioCodec::AC3},
        {QStringLiteral("aac"), QMediaFormat::AudioCodec::AAC},
        {QStringLiteral("alac"), QMediaFormat::AudioCodec::ALAC},
        {QStringLiteral("eac3"), QMediaFormat::AudioCodec::EAC3},
        {QStringLiteral("mp3"), QMediaFormat::AudioCodec::MP3},
        {QStringLiteral("wav"), QMediaFormat::AudioCodec::Wave},
        {QStringLiteral("wave"), QMediaFormat::AudioCodec::Wave},
        {QStringLiteral("flac"), QMediaFormat::AudioCodec::FLAC},
    };
};
