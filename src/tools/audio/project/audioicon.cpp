#include "audioicon.h"
#include "logging.h"

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
