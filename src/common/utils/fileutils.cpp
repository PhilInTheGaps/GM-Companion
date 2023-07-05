#include "fileutils.h"

/**
 * @brief Get a path string from a list of folders.
 * Example: { "some", "folder" } becomes "/some/folder"
 */
auto FileUtils::dirFromFolders(const QStringList &folders) -> QString
{
    auto dirString = folders.join('/');

    if (dirString.isEmpty()) dirString = '/';

    return dirString;
}

/**
 * @brief Get the path to a folder a file is located in.
 * Example: "some/file.mp3" becomes "some"
 */
auto FileUtils::dirFromPath(const QString &path) -> QString
{
    auto index = path.lastIndexOf('/');

    if (index < 0) return QLatin1String();

    if (index == 0) return QStringLiteral("/");

    return path.left(index);
}

/**
 * @brief Get a list of folders from a path string.
 * Example: "/some/folder" becomes { "some", "folder" }
 */
auto FileUtils::foldersFromDir(const QString &dir) -> QStringList
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

    return fileName.right(fileName.length() - index - 1);
}

/**
 * @brief Get the filename (with suffix) from a path.
 * Example: path "/some/file.mp3" becomes "file.mp3"
 */
auto FileUtils::fileName(const QString &path) -> QString
{
    auto endsWithSlash = path.endsWith('/');
    auto from = endsWithSlash ? -2 : -1;

    if (auto index = path.lastIndexOf('/', from); index > -1)
    {
        auto result = path.right(path.length() - index - 1);
        return endsWithSlash ? result.left(result.length() - 1) : result;
    }

    return endsWithSlash ? path.left(path.length() - 1) : path;
}

/**
 * @brief Get the filename (with suffix) from a path.
 * Example: path "/some/file.mp3" becomes "file.mp3"
 */
auto FileUtils::fileName(QStringView path) -> QString
{
    auto endsWithSlash = path.endsWith('/');
    auto from = endsWithSlash ? -2 : -1;

    if (auto index = path.lastIndexOf('/', from); index > -1)
    {
        auto result = path.right(path.length() - index - 1).toString();
        return endsWithSlash ? result.left(result.length() - 1) : result;
    }

    return endsWithSlash ? path.left(path.length() - 1).toString() : path.toString();
}

/**
 * @brief Increment a filename by adding (or incrementing) a number.
 * Example: "file.mp3" becomes "file_0.mp3", which would become "file_1.mp3"
 */
auto FileUtils::incrementFileName(const QString &fileName) -> QString
{
    if (fileName.isEmpty()) return QLatin1String();

    auto nameAndSuffix = splitFileNameAndSuffix(fileName);
    auto incrementedName = incrementName(nameAndSuffix.first);

    return incrementedName + '.' + nameAndSuffix.second;
}

auto FileUtils::incrementName(const QString &name) -> QString
{
    if (name.isEmpty()) return QLatin1String();

    auto list = name.split('_');
    auto number = list.length() == 1 ? 0 : incrementNumString(list.takeLast());

    auto joined = list.join('_');
    if (!joined.endsWith('_')) joined.append('_');

    return joined + QString::number(number);
}

auto FileUtils::fileInDir(const QString &fileName, const QString &dir) -> QString
{
    if (dir.isEmpty()) return fileName;

    if (dir.endsWith('/') || dir.endsWith('\\'))
    {
        if (fileName.startsWith('/'))
        {
            return dir + fileName.right(fileName.length() - 1);
        }

        return dir + fileName;
    }

    if (dir.lastIndexOf('\\') > -1)
    {
        return dir + '\\' + fileName;
    }

    if (fileName.startsWith('/'))
    {
        return dir + fileName;
    }

    return dir + '/' + fileName;
}

/// Get the mimetype of a file, based on the file extension
auto FileUtils::getMimeType(const QString &filename) -> FileUtils::MimeType
{
    if (const auto extension = FileUtils::suffix(filename); extensionToMimeType.contains(extension))
    {
        return extensionToMimeType[extension];
    }

    return MimeType::Unknown;
}

auto FileUtils::splitFileNameAndSuffix(const QString &fileName) -> std::pair<QString, QString>
{
    auto suffix = FileUtils::suffix(fileName);
    auto pathWithoutSuffix = fileName;
    pathWithoutSuffix.replace(suffix, QLatin1String()).chop(1);

    return {pathWithoutSuffix, suffix};
}

auto FileUtils::incrementNumString(const QString &num) -> int
{
    if (!num.isEmpty())
    {
        bool ok = false;
        auto temp = num.toInt(&ok);

        if (ok)
        {
            return temp + 1;
        }
    }

    return 0;
}
