#include "fileutils.h"

/**
 * @brief Get a path string from a list of folders.
 * Example: { "some", "folder" } becomes "/some/folder"
 */
auto FileUtils::dirFromFolders(const QStringList& folders) -> QString
{
    auto dirString = folders.join("/");

    if (dirString.isEmpty()) dirString = "/";

    return dirString;
}

/**
 * @brief Get the path to a folder a file is located in.
 * Example: "some/file.mp3" becomes "some"
 */
auto FileUtils::dirFromPath(const QString &path) -> QString
{
    auto index = path.lastIndexOf('/');

    if (index < 0) return "";

    if (index == 0) return "/";

    return path.left(index);
}

/**
 * @brief Get a list of folders from a path string.
 * Example: "/some/folder" becomes { "some", "folder" }
 */
auto FileUtils::foldersFromDir(const QString& dir) -> QStringList
{
    return dir.split('/');
}

/**
 * @brief Get the suffix from a filename (or path)
 * Example: "/some/file.mp3" becomes "mp3"
 */
auto FileUtils::suffix(const QString &fileName) -> QString
{
    auto index = fileName.lastIndexOf('.');

    return fileName.right(fileName.length() - index -1);
}

/**
 * @brief Get the filename (with suffix) from a path.
 * Example: path "/some/file.mp3" becomes "file.mp3"
 */
auto FileUtils::fileName(const QString &path) -> QString
{
    auto index = path.lastIndexOf('/');

    if (index > -1)
    {
        return path.right(path.length() - index - 1);
    }

    return path;
}

/**
 * @brief Increment a filename by adding (or incrementing) a number.
 * Example: "file.mp3" becomes "file_0.mp3", which would become "file_1.mp3"
 */
auto FileUtils::incrementFileName(const QString &fileName) -> QString
{
    if (fileName.isEmpty()) return "";

    auto suffix = FileUtils::suffix(fileName);

    auto pathWithoutSuffix = fileName;
    pathWithoutSuffix.replace(suffix, "").chop(1);

    auto name = FileUtils::fileName(pathWithoutSuffix);
    auto list = name.split('_');

    auto number = 0;

    if (list.length() > 1)
    {
        bool ok = 0;
        auto temp = list.takeLast().toInt(&ok);

        if (ok) number = temp + 1;
    }

    auto dir = FileUtils::dirFromPath(fileName);
    if (!dir.isEmpty() && !dir.endsWith('/')) dir.append('/');

    auto joined = list.join('_');
    if (!joined.endsWith('_')) joined.append('_');

    return dir + joined + QString::number(number) + '.' + suffix;
}
