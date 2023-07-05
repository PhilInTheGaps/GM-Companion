#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QHash>
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

    static auto getMimeType(const QString &filename) -> MimeType;

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
};

#endif // FILEUTILS_H
