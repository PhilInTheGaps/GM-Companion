#include "audiofile.h"
#include <utils/fileutils.h>

using namespace Qt::Literals::StringLiterals;

AudioFile::AudioFile(const QString &url, Source source, const QString &title, QObject *parent)
    : QObject(parent), a_url(url), a_title(title), a_source(source)
{
    connect(this, &AudioFile::missingChanged, this, [this](bool missing) {
        if (missing) hadError(true);
    });
}

AudioFile::AudioFile(const QJsonObject &object, QObject *parent)
    : AudioFile(object["url"_L1].toString(), static_cast<Source>(object["source"_L1].toInt()), u""_s, parent)
{
}

AudioFile::AudioFile(const AudioFile &other) : AudioFile(other.url(), other.source(), other.title(), other.parent())
{
}

auto AudioFile::printableUrl() const -> QString
{
    return title().isEmpty() ? url() : u"%1 | %2"_s.arg(title(), url());
}

auto AudioFile::toJson() const -> QJsonObject
{
    return QJsonObject{{"url", url()}, {"source", static_cast<int>(source())}};
}

auto AudioFile::displayName() const -> QString
{
    if (!title().isEmpty()) return title();

    if (source() == Source::File)
    {
        return FileUtils::fileName(url());
    }

    return url();
}
