#include "filecacheentry.h"

FileCacheEntry::FileCacheEntry(const QByteArray &data)
{
    update(data);
}

auto FileCacheEntry::tryGetData(QByteArray &data) -> bool
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

auto FileCacheEntry::isFresh(int expirationTimeMs) const -> bool
{
    const auto now = QTime::currentTime();
    return now >= m_lastModified && m_lastModified.msecsTo(now) < expirationTimeMs;
}
