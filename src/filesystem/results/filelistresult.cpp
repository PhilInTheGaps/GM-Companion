#include "filelistresult.h"
#include "utils/fileutils.h"
#include "utils/stringutils.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileListResult::filesFull(const QString &wildcard) const -> QStringList
{
    const auto &list = getMatchingEntries(files(), wildcard);
    return getFullyQualified(list);
}

auto FileListResult::foldersFull(const QString &wildcard) const -> QStringList
{
    const auto &list = getMatchingEntries(folders(), wildcard);
    return getFullyQualified(list);
}

auto FileListResult::getFullyQualified(const QStringList &list) const -> QStringList
{
    QStringList results;

    for (const auto &file : list)
    {
        results << FileUtils::fileInDir(file, path());
    }

    return results;
}

auto FileListResult::getMatchingEntries(const QStringList &list, const QString &wildcard) -> QStringList
{
    if (wildcard.isEmpty() || wildcard == "*"_L1) return list;

    QStringList results;

    for (const auto &entry : list)
    {
        if (StringUtils::hasWildcardMatch(entry, wildcard))
        {
            results << entry;
        }
    }

    return results;
}
