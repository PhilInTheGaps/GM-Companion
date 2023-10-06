#pragma once

#include "audioplaylist.h"
#include <memory>

class AudioPlaylistFactory
{
public:
    AudioPlaylistFactory() = delete;

    static auto build(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>;
    static auto m3u(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>;
    static auto pls(const QByteArray &data, QObject *parent) -> std::unique_ptr<AudioPlaylist>;

private:
    [[nodiscard]] static auto getType(const QByteArray &data) -> AudioPlaylist::Type;
};
