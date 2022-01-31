#include "audiothumbnailcache.h"
#include <QTimer>

auto AudioThumbnailCache::instance() -> AudioThumbnailCache *
{
    if (!m_instance)
    {
        m_instance = new AudioThumbnailCache;
    }

    return m_instance;
}

/// Try to get an entry from the cache.
/// Returns true if operation was successful.
auto AudioThumbnailCache::tryGet(const QString &key, QPixmap *pixmap) -> bool
{
    instance()->lock.lockForRead();
    auto success = instance()->m_cache.contains(key);

    if (success)
    {
        *pixmap = instance()->m_cache[key];
    }

    instance()->lock.unlock();
    return success;
}

/// Check if the cache has an entry for <key>
auto AudioThumbnailCache::contains(const QString &key) -> bool
{
    instance()->lock.lockForRead();
    bool result = instance()->m_cache.contains(key);
    instance()->lock.unlock();

    return result;
}

void AudioThumbnailCache::insertImage(const QString &key, const QPixmap &image)
{
    if (lock.tryLockForWrite(LOCK_TIMEOUT))
    {
        m_cache[key] = image;
        lock.unlock();
    }
    else
    {
        QTimer::singleShot(LOCK_TIMEOUT, [key, image, this]() {
            insertImage(key, image);
        });
    }
}
