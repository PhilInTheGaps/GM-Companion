#pragma once

#include "filecacheentry.h"
#include <QByteArray>
#include <QHash>
#include <QString>
#include <memory>

class FileCache
{
public:
    explicit FileCache(int expirationTimeMs = DEFAULT_EXPIRATION_TIME_MS);

    auto tryGetData(const QString &path, QByteArray &data) -> bool;
    auto createOrUpdateEntry(const QString &path, const QByteArray &data) -> bool;
    auto removeEntry(const QString &path) -> bool;
    auto moveEntry(const QString &oldPath, const QString &newPath) -> bool;
    auto copyEntry(const QString &path, const QString &copy) -> bool;
    auto checkEntry(const QString &path) -> bool;

    void printEntries() const;

private:
    QHash<QString, std::shared_ptr<FileCacheEntry>> m_entries;
    int m_expirationTimeMs;

    static constexpr int DEFAULT_EXPIRATION_TIME_MS = 30000;
};
