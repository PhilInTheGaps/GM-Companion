#include "audioplaylist.h"
#include "utils/networkutils.h"
#include <QRegularExpression>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

AudioPlaylist::AudioPlaylist(const QByteArray &data, QObject *parent) : m_type(getType(data))
{
    switch (m_type)
    {
    case Type::m3u:
        parseM3u(data, parent);
        break;
    case Type::pls:
        parsePls(data, parent);
        break;
    default:
        break;
    }
}

auto AudioPlaylist::isEmpty() const -> bool
{
    return m_files.isEmpty();
}

auto AudioPlaylist::length() const -> qsizetype
{
    return m_files.length();
}

auto AudioPlaylist::files() const -> QList<AudioFile *>
{
    return m_files;
}

auto AudioPlaylist::type() const -> AudioPlaylist::Type
{
    return m_type;
}

auto AudioPlaylist::getType(const QByteArray &data) -> AudioPlaylist::Type
{
    QTextStream stream(data, QIODeviceBase::ReadOnly);
    const auto line = stream.readLine();

    if (line.isNull() || line.isEmpty()) return Type::Undefined;

    if (line.trimmed() == "[playlist]"_L1) return Type::pls;

    return Type::m3u;
}

void AudioPlaylist::parseM3u(const QByteArray &data, QObject *parent)
{
    QTextStream stream(data, QIODeviceBase::ReadOnly);

    QString line;
    while (!(line = stream.readLine().trimmed()).isNull())
    {
        // header type line, ignore for now
        if (line.startsWith("#"_L1)) continue;

        const auto isFromWeb = NetworkUtils::isHttpUrl(line);

        m_files << new AudioFile(line, isFromWeb ? AudioFile::Source::Web : AudioFile::Source::File, u""_s, parent);
    }
}

void AudioPlaylist::parsePls(const QByteArray &data, QObject *parent)
{
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

            m_files.insert(
                index, new AudioFile(url, isFromWeb ? AudioFile::Source::Web : AudioFile::Source::File, u""_s, parent));
        }
    }
}
