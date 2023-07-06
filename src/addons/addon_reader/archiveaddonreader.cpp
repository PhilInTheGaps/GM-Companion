#include "archiveaddonreader.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <quazipdir.h>
#include <quazipfile.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAddonArchiveReader, "gm.addons.reader.archive")

ArchiveAddonReader::ArchiveAddonReader(const Addon &addon) : m_addon(addon)
{
    m_zip = std::make_unique<QuaZip>(addon.path());

    if (!m_zip->open(QuaZip::mdUnzip))
    {
        qCWarning(gmAddonArchiveReader) << "Could not open addon zip file" << m_addon.path();
    }

    setRootDir();
}

auto ArchiveAddonReader::findAllFiles(const QString &path, const QStringList &filter) -> QStringList
{
    QuaZipDir const dir(m_zip.get(), FileUtils::fileInDir(path, m_root));
    return dir.entryList(filter, QDir::Files);
}

auto ArchiveAddonReader::readFile(const QString &path) -> QByteArray
{
    m_zip->setCurrentFile(FileUtils::fileInDir(path, m_root));

    QuaZipFile zipFile(m_zip.get());
    if (!zipFile.open(QIODevice::ReadOnly))
    {
        qCWarning(gmAddonArchiveReader) << "Could not open file" << path << "of addon" << m_addon.id();
        return {};
    }

    auto data = zipFile.readAll();
    zipFile.close();

    return data;
}

auto ArchiveAddonReader::checkFile(const QString &path) -> bool
{
    qCDebug(gmAddonArchiveReader) << path;

    QuaZipDir const dir(m_zip.get());
    return dir.exists(FileUtils::fileInDir(path, m_root));
}

void ArchiveAddonReader::setRootDir()
{
    auto jsonPath = findAddonJson();
    m_root = FileUtils::dirFromPath(jsonPath) + "/";
}

auto ArchiveAddonReader::findAddonJson() const -> QString
{
    const auto entries = m_zip->getFileNameList();

    for (const auto &entry : entries)
    {
        if (entry.endsWith("addon.json"_L1))
        {
            return entry;
        }
    }

    qCWarning(gmAddonArchiveReader()) << "Error: addon.json could not be found in addon" << m_addon.id();
    return u""_s;
}
