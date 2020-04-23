#include "fileutils.h"

QString FileUtils::dirFromFolders(const QStringList& folders)
{
    auto dirString = folders.join("/");

    if (dirString.isEmpty()) dirString = "/";

    return dirString;
}

QStringList FileUtils::foldersFromDir(const QString& dir)
{
    return dir.split('/');
}
