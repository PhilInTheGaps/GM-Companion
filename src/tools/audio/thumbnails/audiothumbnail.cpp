#include "audiothumbnail.h"
#include "logging.h"
#include "settings/settingsmanager.h"

auto AudioThumbnail::imageIdForReload() -> QString
{
    m_imageIdCounter = !m_imageIdCounter;
    return QString("%1?r=%2").arg(m_imageId, QString::number(m_imageIdCounter));
}

auto AudioThumbnail::absoluteUrl() const -> QString
{
    if (m_relativeUrl.isEmpty()) return QLatin1String();

    // Is web url?
    if (m_relativeUrl.startsWith("http://") ||
        m_relativeUrl.startsWith("https://"))
    {
        return m_relativeUrl;
    }

    return SettingsManager::getPath("resources") + m_relativeUrl;
}

void AudioThumbnail::setRelativeUrl(const QString &url)
{
    m_relativeUrl = url;
    emit urlChanged();
}

auto AudioThumbnail::addCollageImage(const QPair<QString, QPixmap> &icon) -> bool
{
    // Check if icon already exists
    for (const auto& entry : m_collageIcons)
    {
        if (entry.first == icon.first) return false;

        if (entry.second.cacheKey() == icon.second.cacheKey()) return false;
    }

    m_collageIcons.append(icon);
    return true;
}
