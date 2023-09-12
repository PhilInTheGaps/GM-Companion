#include "audioelement.h"
#include "../thumbnails/audiothumbnail.h"
#include "audioscenario.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>
#include <utility>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioElement, "gm.audio.project.element")

AudioElement::AudioElement(const QString &name, Type type, const QString &path, AudioScenario *parent)
    : TreeItem(name, path.split(u"/"_s).length() - 1, false, parent), a_type(type)
{
    this->name(name);
    m_path = path + "/" + typeToString(type) + "/" + name;
    a_thumbnail = new AudioThumbnail(m_path, this);
}

AudioElement::AudioElement(const QJsonObject &object, Type type, const QString &path, AudioScenario *parent)
    : TreeItem(u""_s, path.split(u"/"_s).length() - 1, false, parent), a_type(type)
{
    name(object["name"_L1].toString());
    mode(static_cast<Mode>(object["mode"_L1].toInt()));
    m_path = path + "/" + typeToString(type) + "/" + name();

    a_thumbnail = new AudioThumbnail(m_path, this);
    a_thumbnail->setRelativeUrl(object["icon"_L1].toString());

    foreach (const auto &file, object.value("files"_L1).toArray())
    {
        m_files.append(new AudioFile(file.toObject(), this));
    }
}

AudioElement::AudioElement(const AudioElement &other)
    : AudioElement(other.name(), other.type(), FileUtils::dirFromPath(other.path()),
                   qobject_cast<AudioScenario *>(other.parent()))
{
    mode(other.mode());
    m_files = Utils::copyList(other.files());
    thumbnail(other.thumbnail());
}

/**
 * @brief Serialize AudioElement to json
 */
auto AudioElement::toJson() const -> QJsonObject
{
    QJsonObject object({{"name", name()}, {"icon", thumbnail()->relativeUrl()}, {"mode", static_cast<int>(mode())}});

    // Files
    QJsonArray files;
    foreach (const auto *file, m_files)
    {
        files.append(file->toJson());
    }
    object.insert(u"files"_s, files);
    return object;
}

/**
 * @brief Replace the file list of the element.
 * All previous files are deleted!
 */
void AudioElement::setFiles(QList<AudioFile *> files)
{
    foreach (auto *file, m_files)
    {
        if (file) file->deleteLater();
    }

    m_files = std::move(files);
    emit filesChanged();
}

/**
 * @brief Add an AudioFile to the filelist
 */
auto AudioElement::addFile(AudioFile *file) -> bool
{
    if (!file) return false;

    file->setParent(this);
    m_files.append(file);
    emit filesChanged();
    return true;
}

/**
 * @brief Try to remove the file at index
 */
auto AudioElement::removeFile(int index) -> bool
{
    if (Utils::isInBounds(m_files, index))
    {
        auto *file = m_files.takeAt(index);
        file->deleteLater();
        emit filesChanged();
        return true;
    }

    return false;
}

/**
 * @brief Move a file to a new position in the list
 */
auto AudioElement::moveFile(int from, int steps) -> bool
{
    const int to = from + steps;

    if (Utils::isInBounds(m_files, to))
    {
        m_files.move(from, to);
        emit filesChanged();
        return true;
    }

    return false;
}

auto AudioElement::typeToString(AudioElement::Type type) -> QString
{
    return QVariant::fromValue(type).toString();
}

auto AudioElement::typeToSettings(AudioElement::Type type) -> QString
{
    switch (type)
    {
    case AudioElement::Type::Music:
        return u"music"_s;
    case AudioElement::Type::Sound:
        return u"sounds"_s;
    case AudioElement::Type::Radio:
        return u"radio"_s;
    default:
        qCWarning(gmAudioElement()) << "Error: getPath() was called with illegal element type:"
                                    << AudioElement::typeToString(type);
        return u""_s;
    }
}

/**
 * @brief Compare two audio elements, true if e1 comes alphabetically before e2.
 */
auto AudioElement::compare(const AudioElement *e1, const AudioElement *e2) -> bool
{
    if (e1 && e2) return e1->name() < e2->name();
    return true;
}
