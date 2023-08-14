#include "audiothumbnail.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"

using namespace Qt::Literals::StringLiterals;

auto AudioThumbnail::imageIdForReload() -> QString
{
    m_imageIdCounter = !m_imageIdCounter;
    return u"%1?r=%2"_s.arg(m_imageId, QString::number(m_imageIdCounter));
}

auto AudioThumbnail::absoluteUrl() const -> QString
{
    if (m_relativeUrl.isEmpty()) return u""_s;

    // Is web url?
    if (NetworkUtils::isHttpUrl(m_relativeUrl))
    {
        return m_relativeUrl;
    }

    return SettingsManager::getPath(u"resources"_s) + m_relativeUrl;
}

void AudioThumbnail::setRelativeUrl(const QString &url)
{
    m_relativeUrl = url;
    emit urlChanged();
}

auto AudioThumbnail::addCollageImage(const std::pair<QString, QPixmap> &icon) -> bool
{
    // Check if icon already exists
    for (const auto &entry : m_collageIcons)
    {
        if (entry.first == icon.first) return false;

        if (entry.second.cacheKey() == icon.second.cacheKey()) return false;
    }

    m_collageIcons.append(icon);
    return true;
}
