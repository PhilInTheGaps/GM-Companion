#include "audiofile.h"
#include <utils/fileutils.h>

AudioFile::AudioFile(const QString &url, Source source, const QString &title, QObject *parent)
    : QObject(parent), a_url(url), a_title(title), a_source(source)
{
}

AudioFile::AudioFile(const QJsonObject &object, QObject *parent) : QObject(parent)
{
    url(object[QStringLiteral("url")].toString());
    a_source = static_cast<Source>(object[QStringLiteral("source")].toInt());
}

AudioFile::AudioFile(const AudioFile &other) : AudioFile(other.url(), other.source(), other.title(), other.parent())
{
}

auto AudioFile::printableUrl() const -> QString
{
    return title().isEmpty() ? url() : QStringLiteral("%1 | %2").arg(title(), url());
}

auto AudioFile::toJson() const -> QJsonObject
{
    return QJsonObject{{"url", url()}, {"source", static_cast<int>(source())}};
}

auto AudioFile::displayName() const -> QString
{
    if (!title().isEmpty()) return title();

    switch (source())
    {
    case Source::File:
        return FileUtils::fileName(url());
    default:
        return url();
    }
}
