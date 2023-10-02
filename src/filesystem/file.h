#pragma once

#include <QByteArray>
#include <QFuture>
#include <QString>
#include <QStringList>

class NextCloud;
class GoogleDrive;

namespace Files
{

class FileAccess;
class FileResult;
class FileDataResult;
class FileListResult;
class FileCheckResult;
class FileMultiCheckResult;

class File
{
public:
    static void init(NextCloud *nc, GoogleDrive *gd);

    static auto getDataAsync(const QString &path, bool allowCache = true,
                             std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileDataResult>>;

    static auto getDataAsync(const QStringList &paths, bool allowCache = true,
                             std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>;

    static auto saveAsync(const QString &path, const QByteArray &data, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileResult>>;

    static auto moveAsync(const QString &oldPath, const QString &newPath,
                          std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<std::shared_ptr<FileResult>>;

    static auto deleteAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileResult>>;

    static auto copyAsync(const QString &path, const QString &copy, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileResult>>;

    static auto listAsync(const QString &path, bool files, bool folders,
                          std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<std::shared_ptr<FileListResult>>;

    static auto createDirAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileResult>>;

    static auto checkAsync(const QString &path, bool allowCache = true,
                           std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileCheckResult>>;

    static auto checkAsync(const QStringList &paths, bool allowCache = true,
                           std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<std::shared_ptr<FileMultiCheckResult>>;

    static void updateFileAccess();

private:
    static auto getFileAccess(std::shared_ptr<FileAccess> fileAccess) -> std::shared_ptr<FileAccess>;

    static inline NextCloud *s_nc = nullptr;
    static inline GoogleDrive *s_gd = nullptr;
};

} // namespace Files
