#include "audiofile.h"
#include <utils/fileutils.h>

AudioFile::AudioFile(const QString &url, AudioFileSource source, const QString &title, QObject *parent)
    : QObject(parent), a_url(url), a_title(title), a_source(source), a_missing(false) {}

AudioFile::AudioFile(const QJsonObject &object, QObject *parent)
    : QObject(parent), a_missing(false)
{
    url(object["url"].toString());
    a_source = static_cast<AudioFileSource>(object["source"].toInt());
}

auto AudioFile::printableUrl() const -> QString
{
    return title().isEmpty() ? url() : QString("%1 | %2").arg(title(), url());
}

auto AudioFile::toJson() const -> QJsonObject
{
    return QJsonObject {
        { "url", url() },
        { "source", source() }
    };
}

auto AudioFile::displayName() const -> QString
{
    if (!title().isEmpty()) return title();

    switch (source())
    {
    case AudioFile::File:
        return FileUtils::fileName(url());
    default:
        return url();
    }
}
