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

auto AddonReader::checkFile(const QString &path) -> bool
{
    return m_reader->checkFile(path);
}

auto AddonReader::getFeatures() -> AddonReader::Features
{
    Features features;

    features.setFlag(Feature::None);

    if (!findAllFiles(QStringLiteral("/names"), {QStringLiteral("*.json")}).isEmpty())
    {
        features.setFlag(Feature::Names);
    }

    if (!findAllFiles(QStringLiteral("/audio"), {QStringLiteral("*.json"), QStringLiteral("*.audio")}).isEmpty())
    {
        features.setFlag(Feature::Audio);
    }

    if (!findAllFiles(QStringLiteral("/units"), {QStringLiteral("*.json")}).isEmpty())
    {
        features.setFlag(Feature::Units);
    }

    return features;
}
