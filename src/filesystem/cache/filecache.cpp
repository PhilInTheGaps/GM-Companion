#include "filecache.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmFileCache, "gm.filesystem.cache")

auto FileCache::tryGetData(const QString &path, QByteArray &data) -> bool
{
    if (!m_entries.contains(path))
    {
        return false;
    }

    auto *entry = m_entries[path];
    if (!entry->isFresh())
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
        auto *entry = m_entries[path];
        return entry->update(data);
    }

    auto *entry = new FileCacheEntry(data, this);
    m_entries[path] = entry;
    return true;
}

auto FileCache::removeEntry(const QString &path) -> bool
{
    if (m_entries.contains(path))
    {
        auto *entry = m_entries.take(path);
        entry->deleteLater();
        return true;
    }

    return false;
}

auto FileCache::moveEntry(const QString &oldPath, const QString &newPath) -> bool
{
    if (!m_entries.contains(oldPath)) return false;

    if (m_entries.contains(newPath))
    {
        removeEntry(newPath);
    }

    auto *entry = m_entries.take(oldPath);
    m_entries[newPath] = entry;
    return true;
}

auto FileCache::copyEntry(const QString &path, const QString &copy) -> bool
{
    if (!m_entries.contains(path)) return false;

    if (auto *entry = m_entries[path]; entry->isFresh())
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
    return m_entries.contains(path) && m_entries[path]->isFresh();
}

void FileCache::printEntries() const
{
    qCDebug(gmFileCache()) << "Cache entries:";

    foreach (const auto &key, m_entries.keys())
    {
        auto *value = m_entries[key];

        QByteArray data;
        value->tryGetData(data);

        qCDebug(gmFileCache()) << "\t"
                               << "key:" << key << "\tvalue:" << data << "\tisFresh:" << value->isFresh();
    }
}
