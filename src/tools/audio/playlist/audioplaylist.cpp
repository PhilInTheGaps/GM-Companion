#include "audioplaylist.h"
#include "utils/utils.h"
#include <QRandomGenerator>

using namespace Qt::Literals::StringLiterals;

AudioPlaylist::AudioPlaylist(Type type) : m_type(type)
{
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

auto AudioPlaylist::filesQml(QObject *parent) -> QQmlListProperty<AudioFile>
{
    return QQmlListProperty(parent, &m_files);
}

auto AudioPlaylist::type() const -> AudioPlaylist::Type
{
    return m_type;
}

auto AudioPlaylist::at(qsizetype i) const -> AudioFile *const &
{
    return m_files.at(i);
}

auto AudioPlaylist::constFirst() const -> AudioFile *const &
{
    return m_files.constFirst();
}

void AudioPlaylist::setFiles(const QList<AudioFile *> &files)
{
    m_files = files;
}

void AudioPlaylist::append(AudioFile *file)
{
    if (!file) return;
    m_files.append(file);
}

void AudioPlaylist::insert(qsizetype index, AudioFile *file)
{
    if (!file) return;
    m_files.insert(index, file);
}

void AudioPlaylist::replace(qsizetype index, const AudioPlaylist &other)
{
    replace(index, other.m_files);
}

void AudioPlaylist::replace(qsizetype index, const QList<AudioFile *> &files)
{
    if (!Utils::isInBounds(m_files, index)) return;

    m_files.removeAt(index);

    foreach (auto *file, files)
    {
        m_files.insert(index, file);
        index++;
    }
}

void AudioPlaylist::shuffle()
{
    std::shuffle(m_files.begin(), m_files.end(), *QRandomGenerator::system());
}

void AudioPlaylist::clear()
{
    m_files.clear();
}

auto AudioPlaylist::hasElementsWithoutErrors() const -> bool
{
    foreach (const auto *file, m_files)
    {
        if (!file->hadError()) return true;
    }

    return false;
}
