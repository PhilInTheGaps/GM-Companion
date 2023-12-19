#pragma once

#include "cache/filecache.h"
#include "fileaccess.h"
#include "nextcloud/nextcloud.h"
#include <QObject>

namespace Files
{

class FileAccessNextcloud : public FileAccess
{
public:
    explicit FileAccessNextcloud(Services::NextCloud &nextcloud);
    ~FileAccessNextcloud() override = default;
    Q_DISABLE_COPY_MOVE(FileAccessNextcloud)

    auto getDataAsync(const QString &path, Options options) -> QFuture<FileDataResult> override;
    auto getDataAsync(const QStringList &paths, Options options) -> QFuture<std::vector<FileDataResult>> override;
    auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult> override;
    auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult> override;
    auto deleteAsync(const QString &path) -> QFuture<FileResult> override;
    auto copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult> override;
    auto listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult> override;
    auto createDirAsync(const QString &path) -> QFuture<FileResult> override;
    auto checkAsync(const QString &path, Options options) -> QFuture<FileCheckResult> override;
    auto checkAsync(const QStringList &paths, Options options) -> QFuture<FileMultiCheckResult> override;
    auto getHomeDir() -> QString override;

private:
    Services::NextCloud &m_nc;
    FileCache m_cache;

    static auto encodePath(const QString &data) -> QByteArray;
    static void printError(const QString &errorMessage, const Services::RestReply &reply);
    static auto makeMoveHeaders(const QString &newPath) -> QList<std::pair<QByteArray, QByteArray>>;

    template <typename T1, typename T2>
    auto createDirThenContinue(const QString &dir, const T1 &arg1, const T2 &arg2,
                               const std::function<QFuture<FileResult>(const T1 &, const T2 &)> &func)
        -> QFuture<FileResult>;

    static auto parseListResponse(const QByteArray &data, const QString &path, bool files, bool folders)
        -> FileListResult;
};

} // namespace Files
