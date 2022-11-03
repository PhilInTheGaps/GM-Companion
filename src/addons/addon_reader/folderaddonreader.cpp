#include "folderaddonreader.h"
#include "utils/fileutils.h"
#include <QDir>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAddonFolderReader, "gm.addons.reader.folder")

FolderAddonReader::FolderAddonReader(const Addon &addon) : m_addon(addon)
{
}

auto FolderAddonReader::findAllFiles(const QString &path, const QStringList &filter) -> QStringList
{
    QDir dir(FileUtils::fileInDir(path, m_addon.path()));

    if (!dir.exists()) return {};

    return dir.entryList(filter, QDir::Files);
}

auto FolderAddonReader::readFile(const QString &path) -> QByteArray
{
    const auto absolutePath = FileUtils::fileInDir(path, m_addon.path());

    if (QFile file(absolutePath); file.open(QIODevice::ReadOnly))
    {
        auto data = file.readAll();
        file.close();

        return data;
    }

    qCWarning(gmAddonFolderReader()) << "Could not read file" << path << "as it does not exist";
    return {};
}
