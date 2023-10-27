#pragma once

#include <QByteArray>
#include <QFuture>
#include <QString>
#include <QStringList>
#include <memory>
#include <vector>

namespace Services
{
class NextCloud;
} // namespace Services

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
    static void init(Services::NextCloud *nc);

    static auto getDataAsync(const QString &path, bool allowCache = true,
                             std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<FileDataResult>;

    static auto getDataAsync(const QStringList &paths, bool allowCache = true,
                             std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<std::vector<FileDataResult>>;

    static auto saveAsync(const QString &path, const QByteArray &data, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<FileResult>;

    static auto moveAsync(const QString &oldPath, const QString &newPath,
                          std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<FileResult>;

    static auto deleteAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<FileResult>;

    static auto copyAsync(const QString &path, const QString &copy, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<FileResult>;

    static auto listAsync(const QString &path, bool files, bool folders,
                          std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<FileListResult>;

    static auto createDirAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess = nullptr)
        -> QFuture<FileResult>;

    static auto checkAsync(const QString &path, bool allowCache = true,
                           std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<FileCheckResult>;

    static auto checkAsync(const QStringList &paths, bool allowCache = true,
                           std::shared_ptr<FileAccess> fileAccess = nullptr) -> QFuture<FileMultiCheckResult>;

    static void updateFileAccess();

private:
    static auto getFileAccess(std::shared_ptr<FileAccess> fileAccess) -> std::shared_ptr<FileAccess>;

    static inline Services::NextCloud *s_nc = nullptr;
};

} // namespace Files
