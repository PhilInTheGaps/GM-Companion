#include "filecacheentry.h"
#include "filecache.h"

FileCacheEntry::FileCacheEntry(const QByteArray &data, FileCache *parent)
    : QObject(parent)
{
    update(data);
}

auto FileCacheEntry::tryGetData(QByteArray& data) -> bool
{
    if (m_tempFile.open())
    {
        data = m_tempFile.readAll();
        m_tempFile.close();
        return true;
    }

    return false;
}

auto FileCacheEntry::update(const QByteArray &data) -> bool
{
    m_lastModified = QTime::currentTime();

    if (m_tempFile.open())
    {
        m_tempFile.write(data);
        m_tempFile.close();
        return true;
    }

    return false;
}

auto FileCacheEntry::isFresh() const -> bool
{
    auto now = QTime::currentTime();
    return now > m_lastModified && m_lastModified.msecsTo(now) < EXPIRATION_TIME_MS;
}

