#pragma once

#include "../project/audiofile.h"
#include <QByteArray>
#include <QList>
#include <QObject>

class AudioPlaylist
{
public:
    enum class Type
    {
        Undefined,
        m3u,
        pls
    };

    AudioPlaylist(const QByteArray &data, QObject *parent = nullptr);

    [[nodiscard]] auto isEmpty() const -> bool;
    [[nodiscard]] auto length() const -> qsizetype;
    [[nodiscard]] auto files() const -> QList<AudioFile *>;
    [[nodiscard]] auto type() const -> AudioPlaylist::Type;

private:
    QList<AudioFile *> m_files;
    Type m_type = Type::Undefined;

    [[nodiscard]] static auto getType(const QByteArray &data) -> Type;

    void parseM3u(const QByteArray &data, QObject *parent);
    void parsePls(const QByteArray &data, QObject *parent);
};
