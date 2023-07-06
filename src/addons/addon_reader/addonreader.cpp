#include "addonreader.h"
#include "archiveaddonreader.h"
#include "folderaddonreader.h"

using namespace Qt::Literals::StringLiterals;

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

    if (!findAllFiles(u"/names"_s, {u"*.json"_s}).isEmpty())
    {
        features.setFlag(Feature::Names);
    }

    if (!findAllFiles(u"/audio"_s, {u"*.json"_s, u"*.audio"_s}).isEmpty())
    {
        features.setFlag(Feature::Audio);
    }

    if (!findAllFiles(u"/units"_s, {u"*.json"_s}).isEmpty())
    {
        features.setFlag(Feature::Units);
    }

    return features;
}
