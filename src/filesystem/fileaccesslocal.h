#pragma once

#include "fileaccess.h"
#include <QDir>
#include <QObject>

namespace Files
{

class FileAccessLocal : public FileAccess
{
public:
    FileAccessLocal() = default;
    ~FileAccessLocal() override = default;
    Q_DISABLE_COPY_MOVE(FileAccessLocal)

    auto getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult> override;
    auto getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<std::vector<FileDataResult>> override;
    auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult> override;
    auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult> override;
    auto deleteAsync(const QString &path) -> QFuture<FileResult> override;
    auto copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult> override;
    auto listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult> override;
    auto createDirAsync(const QString &path) -> QFuture<FileResult> override;
    auto checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult> override;
    auto checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult> override;

private:
    static auto getData(const QString &path, bool allowCache) -> FileDataResult;
    static auto createDir(const QDir &dir) -> FileResult;
    static auto save(const QString &path, const QByteArray &data) -> FileResult;
    static auto move(const QString &oldPath, const QString &newPath) -> FileResult;
    static auto copy(const QString &path, const QString &copy) -> FileResult;
    static auto getDirFilter(bool files, bool folders) -> QFlags<QDir::Filter>;
    static auto check(const QString &path, bool allowCache) -> FileCheckResult;

    // There is an issue in qt >= 6.6 where futures resolved on the same thread as the context object
    // of their continuation become stuck trying to lock a mutex.
    // As a workaround all fileaccess futures are resolved on the main thread event so that
    // the context objects for continuations become unecessary.
    // That's what this context object is for.
    QObject m_context;
};

} // namespace Files
