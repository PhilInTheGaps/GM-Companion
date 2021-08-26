#include "audioicon.h"
#include "audioicon.h"
#include "logging.h"
#include "settings/settingsmanager.h"

auto AudioIcon::imageIdForReload() -> QString
{
    m_imageIdCounter = !m_imageIdCounter;
    return QString("%1?r=%2").arg(m_imageId, QString::number(m_imageIdCounter));
}

auto AudioIcon::absoluteUrl() const -> QString
{
    if (m_relativeUrl.isEmpty()) return QStringLiteral("");

    // Is web url?
    if (m_relativeUrl.startsWith("http://") ||
        m_relativeUrl.startsWith("https://"))
    {
        return m_relativeUrl;
    }

    return QUrl::fromLocalFile(SettingsManager::getPath("resources") + m_relativeUrl).toString(QUrl::None);
}

void AudioIcon::setRelativeUrl(const QString &url)
{
    m_relativeUrl = url;
    emit urlChanged();
}

auto AudioIcon::addCollageIcon(const QPair<QString, QPixmap> &icon) -> bool
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

void AudioIcon::setCollageIcon(const QPair<QString, QPixmap>&icon, int index)
{
    qCDebug(gmAudioTool) << "Setting collage icon to index" << index;
    qCDebug(gmAudioTool) << "   Icon count:" << m_collageIcons.length();

    if (m_collageIcons.length() < index)
    {
        qCWarning(gmAudioTool) << "Could not set collage icon, index out of range";
        return;
    }

    m_collageIcons.replace(index, icon);
}