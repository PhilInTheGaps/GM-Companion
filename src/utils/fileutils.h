#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QStringList>

class FileUtils
{
public:
    static QString dirFromFolders(const QStringList& folders);
    static QString dirFromPath(const QString& path);
    static QStringList foldersFromDir(const QString& dir);
    static QString suffix(const QString& fileName);
    static QString fileName(const QString& path);
    static QString incrementFileName(const QString& fileName);
};

#endif // FILEUTILS_H
