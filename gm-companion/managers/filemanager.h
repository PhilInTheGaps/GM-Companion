#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();
    void run();

private:

    QString origPath;
    QStringList filesToBeDeleted;
    QStringList foldersToBeDeleted;

    bool removeDir(const QString & dirName);
};

#endif // FILEMANAGER_H
