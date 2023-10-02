#include "filecache.h"
#include "filecacheentry.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmFileCache, "gm.filesystem.cache")

FileCache::FileCache(int expirationTimeMs) : m_expirationTimeMs(expirationTimeMs)
{
}

auto FileCache::tryGetData(const QString &path, QByteArray &data) -> bool
{
    if (!m_entries.contains(path))
    {
        return false;
    }

    auto entry = m_entries.value(path);
    if (!entry->isFresh(m_expirationTimeMs))
    {
        removeEntry(path);
        return false;
    }

    return entry->tryGetData(data);
}

auto FileCache::createOrUpdateEntry(const QString &path, const QByteArray &data) -> bool
{
    if (m_entries.contains(path))
    {
        auto entry = m_entries.value(path);
        return entry->update(data);
    }

    m_entries[path] = std::make_shared<FileCacheEntry>(data);
    return true;
}

auto FileCache::removeEntry(const QString &path) -> bool
{
    if (m_entries.contains(path))
    {
        auto entry = m_entries.take(path);
        entry.reset();
        return true;
    }

    return false;
}

auto FileCache::moveEntry(const QString &oldPath, const QString &newPath) -> bool
{
    if (!m_entries.contains(oldPath)) return false;

    if (auto entry = m_entries.value(oldPath); !entry->isFresh(m_expirationTimeMs))
    {
        removeEntry(oldPath);
        return false;
    }

    if (m_entries.contains(newPath))
    {
        removeEntry(newPath);
    }

    auto entry = m_entries.take(oldPath);
    m_entries[newPath] = std::move(entry);
    return true;
}

auto FileCache::copyEntry(const QString &path, const QString &copy) -> bool
{
    if (!m_entries.contains(path)) return false;

    if (auto entry = m_entries.value(path); entry->isFresh(m_expirationTimeMs))
    {
        if (QByteArray data; entry->tryGetData(data))
        {
            return createOrUpdateEntry(copy, data);
        }
    }
    else
    {
        removeEntry(path);
    }

    return false;
}

auto FileCache::checkEntry(const QString &path) -> bool
{
    return m_entries.contains(path) && m_entries.value(path)->isFresh(m_expirationTimeMs);
}
