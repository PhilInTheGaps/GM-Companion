#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QStringList>

class FileUtils
{
public:
    static QString dirFromFolders(const QStringList &folders);
    static QString dirFromPath(const QString &path);
    static QStringList foldersFromDir(const QString &dir);
    static QString suffix(const QString &fileName);
    static QString fileName(const QString &path);
    static QString fileName(const QStringRef &path);
    static QString incrementFileName(const QString &fileName);
    static QString incrementName(const QString &name);
    static QString fileInDir(const QString &fileName, const QString &dir);

private:
    static QPair<QString, QString> splitFileNameAndSuffix(const QString &fileName);
    static int incrementNumString(const QString &num);
};

#endif // FILEUTILS_H
