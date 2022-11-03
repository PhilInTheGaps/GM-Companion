#include "addonreader.h"
#include "archiveaddonreader.h"
#include "folderaddonreader.h"

AddonReader::AddonReader(const Addon &addon)
{
    switch (addon.type())
    {
    case Addon::Type::Folder:
        m_reader = std::make_unique<FolderAddonReader>(addon);
        break;
    case Addon::Type::Archive:
        m_reader = std::make_unique<ArchiveAddonReader>(addon);
        break;
    }
}

auto AddonReader::findAllFiles(const QString &path, const QStringList &filter) -> QStringList
{
    return m_reader->findAllFiles(path, filter);
}

auto AddonReader::readFile(const QString &path) -> QByteArray
{
    return m_reader->readFile(path);
}
