#include "audioplaylistfactory.h"
#include "utils/networkutils.h"
#include <QRegularExpression>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

auto AudioPlaylistFactory::build(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>
{
    switch (getType(data))
    {
    case AudioPlaylist::Type::m3u:
        return m3u(data, parent);
    case AudioPlaylist::Type::pls:
        return pls(data, parent);
    default:
        return std::unique_ptr<AudioPlaylist>();
    }
}

auto AudioPlaylistFactory::m3u(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>
{
    auto playlist = std::make_unique<AudioPlaylist>(AudioPlaylist::Type::m3u);
    QTextStream stream(data, QIODeviceBase::ReadOnly);

    QString line;
    while (!(line = stream.readLine().trimmed()).isNull())
    {
        // header type line, ignore for now
        if (line.startsWith("#"_L1)) continue;

        const auto isFromWeb = NetworkUtils::isHttpUrl(line);

        playlist->append(
            new AudioFile(line, isFromWeb ? AudioFile::Source::Web : AudioFile::Source::File, u""_s, parent));
    }

    return playlist;
}

auto AudioPlaylistFactory::pls(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>
{
    auto playlist = std::make_unique<AudioPlaylist>(AudioPlaylist::Type::pls);
    QTextStream stream(data, QIODeviceBase::ReadOnly);

    QString line;
    while (!(line = stream.readLine().trimmed()).isNull())
    {
        static QRegularExpression const regex(uR"([fF]ile(?<index>\d+)=(?<url>.+))"_s);
        auto match = regex.match(line);

        if (match.isValid() && match.hasMatch())
        {
            const auto index = match.captured("index"_L1).toInt() - 1;
            const auto url = match.captured("url"_L1);
            const auto isFromWeb = NetworkUtils::isHttpUrl(url);

            playlist->insert(
                index, new AudioFile(url, isFromWeb ? AudioFile::Source::Web : AudioFile::Source::File, u""_s, parent));
        }
    }
    return playlist;
}

auto AudioPlaylistFactory::getType(const QByteArray &data) -> AudioPlaylist::Type
{
    QTextStream stream(data, QIODeviceBase::ReadOnly);
    const auto line = stream.readLine();

    if (line.isNull() || line.isEmpty()) return AudioPlaylist::Type::Undefined;

    if (line.trimmed() == "[playlist]"_L1) return AudioPlaylist::Type::pls;

    return AudioPlaylist::Type::m3u;
}
