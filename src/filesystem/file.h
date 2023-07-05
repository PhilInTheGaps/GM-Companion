#pragma once

#include <QByteArray>
#include <QFuture>
#include <QString>
#include <QStringList>

#include "fileaccess.h"

class NextCloud;
class GoogleDrive;

namespace Files
{

class File
{
public:
    static void init(NextCloud *nc, GoogleDrive *gd);

    static QFuture<FileDataResult *> getDataAsync(const QString &path, bool allowCache = true,
                                                  FileAccess *fileAccess = nullptr);
    static QFuture<std::vector<FileDataResult *>> getDataAsync(const QStringList &paths, bool allowCache = true,
                                                               FileAccess *fileAccess = nullptr);

    static QFuture<FileResult *> saveAsync(const QString &path, const QByteArray &data,
                                           FileAccess *fileAccess = nullptr);

    static QFuture<FileResult *> moveAsync(const QString &oldPath, const QString &newPath,
                                           FileAccess *fileAccess = nullptr);

    static QFuture<FileResult *> deleteAsync(const QString &path, FileAccess *fileAccess = nullptr);

    static QFuture<FileResult *> copyAsync(const QString &path, const QString &copy, FileAccess *fileAccess = nullptr);

    static QFuture<FileListResult *> listAsync(const QString &path, bool files, bool folders,
                                               FileAccess *fileAccess = nullptr);

    static QFuture<FileResult *> createDirAsync(const QString &path, FileAccess *fileAccess = nullptr);

    static QFuture<FileCheckResult *> checkAsync(const QString &path, bool allowCache = true,
                                                 FileAccess *fileAccess = nullptr);

    static QFuture<FileMultiCheckResult *> checkAsync(const QStringList &paths, bool allowCache = true,
                                                      FileAccess *fileAccess = nullptr);

    static void updateFileAccess();

private:
    static auto getFileAccess(FileAccess *fileAccess) -> FileAccess *;

    static inline NextCloud *s_nc = nullptr;
    static inline GoogleDrive *s_gd = nullptr;
};

} // namespace Files
