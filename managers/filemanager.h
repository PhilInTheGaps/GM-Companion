#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();

    void copyFiles();

    void copyContents(QString path, QString destination);

private:

    QString origPath;
    QStringList filesToBeDeleted;
    QStringList foldersToBeDeleted;

};

#endif // FILEMANAGER_H
