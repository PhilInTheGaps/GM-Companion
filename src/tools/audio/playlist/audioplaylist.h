#pragma once

#include "../project/audiofile.h"
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QQmlListProperty>

class AudioPlaylist
{
public:
    enum class Type
    {
        Undefined,
        m3u,
        pls
    };

    AudioPlaylist() = default;
    explicit AudioPlaylist(Type type);

    [[nodiscard]] auto isEmpty() const -> bool;
    [[nodiscard]] auto length() const -> qsizetype;
    [[nodiscard]] auto type() const -> AudioPlaylist::Type;
    [[nodiscard]] auto at(qsizetype i) const -> AudioFile *const &;
    [[nodiscard]] auto constFirst() const -> AudioFile *const &;

    [[nodiscard]] auto files() const -> QList<AudioFile *>;
    [[nodiscard]] auto filesQml(QObject *parent) -> QQmlListProperty<AudioFile>;
    void setFiles(const QList<AudioFile *> &files);
    void append(AudioFile *file);
    void insert(qsizetype index, AudioFile *file);

    void replace(qsizetype index, const AudioPlaylist &other);
    void replace(qsizetype index, const QList<AudioFile *> &files);

    void shuffle();
    void clear();

    [[nodiscard]] auto hasElementsWithoutErrors() const -> bool;

private:
    QList<AudioFile *> m_files;
    Type m_type = Type::Undefined;
};
