#pragma once

#include <QByteArray>
#include <QTemporaryFile>
#include <QTime>

class FileCacheEntry
{
public:
    explicit FileCacheEntry(const QByteArray &data);

    auto tryGetData(QByteArray &data) -> bool;
    auto update(const QByteArray &data) -> bool;
    [[nodiscard]] auto isFresh(int expirationTimeMs) const -> bool;

private:
    QTime m_lastModified;
    QTemporaryFile m_tempFile;
};
