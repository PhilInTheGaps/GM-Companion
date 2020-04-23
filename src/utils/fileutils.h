#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QStringList>

class FileUtils
{
public:
    static QString dirFromFolders(const QStringList& folders);
    static QStringList foldersFromDir(const QString& dir);
};

#endif // FILEUTILS_H
