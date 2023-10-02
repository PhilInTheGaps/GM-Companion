#pragma once

#include "fileaccess.h"
#include <QDir>

namespace Files
{

class FileAccessLocal : public FileAccess
{
public:
    FileAccessLocal() = default;

    auto getDataAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileDataResult>> override;
    auto getDataAsync(const QStringList &paths, bool allowCache)
        -> QFuture<std::vector<std::shared_ptr<FileDataResult>>> override;
    auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<std::shared_ptr<FileResult>> override;
    auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<std::shared_ptr<FileResult>> override;
    auto deleteAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>> override;
    auto copyAsync(const QString &path, const QString &copy) -> QFuture<std::shared_ptr<FileResult>> override;
    auto listAsync(const QString &path, bool files, bool folders) -> QFuture<std::shared_ptr<FileListResult>> override;
    auto createDirAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>> override;
    auto checkAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileCheckResult>> override;
    auto checkAsync(const QStringList &paths, bool allowCache)
        -> QFuture<std::shared_ptr<FileMultiCheckResult>> override;

private:
    static auto getData(const QString &path, bool allowCache) -> std::shared_ptr<FileDataResult>;
    static auto createDir(const QDir &dir) -> std::shared_ptr<FileResult>;
    static auto save(const QString &path, const QByteArray &data) -> std::shared_ptr<FileResult>;
    static auto move(const QString &oldPath, const QString &newPath) -> std::shared_ptr<FileResult>;
    static auto copy(const QString &path, const QString &copy) -> std::shared_ptr<FileResult>;
    static auto getDirFilter(bool files, bool folders) -> QFlags<QDir::Filter>;
    static auto check(const QString &path, bool allowCache) -> std::shared_ptr<FileCheckResult>;
};

} // namespace Files
