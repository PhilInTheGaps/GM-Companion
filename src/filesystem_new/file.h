#pragma once

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QFuture>

#include "fileaccess.h"

namespace Files {

class File
{
public:
    static QFuture<FileDataResult*> getDataAsync(const QString& path, bool allowCache = true, FileAccess *fileAccess = nullptr);
    static QFuture<QVector<FileDataResult*>> getDataAsync(const QStringList& paths, bool allowCache = true, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult*> saveAsync(const QString& path, const QByteArray& data, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult*> moveAsync(const QString& oldPath, const QString& newPath, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult*> deleteAsync(const QString& path, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult*> copyAsync(const QString& path, const QString& copy, FileAccess *fileAccess = nullptr);

    static QFuture<FileListResult*> listAsync(const QString& path, bool files, bool folders, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult*> createDirAsync(const QString& path, FileAccess *fileAccess = nullptr);

    static QFuture<FileCheckResult*> checkAsync(const QString& path, bool allowCache = true, FileAccess *fileAccess = nullptr);

    static QFuture<FileMultiCheckResult*> checkAsync(const QStringList& paths, bool allowCache = true, FileAccess *fileAccess = nullptr);

private:
    static FileAccess *getFileAccess(FileAccess* fileAccess);

};

}

