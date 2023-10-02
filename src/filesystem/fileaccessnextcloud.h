#pragma once

#include "cache/filecache.h"
#include "fileaccess.h"
#include "nextcloud/nextcloud.h"
#include <QNetworkReply>
#include <QObject>

namespace Files
{

class FileAccessNextcloud : public FileAccess
{
public:
    explicit FileAccessNextcloud(NextCloud &nextcloud);

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
    NextCloud &m_nc;
    FileCache m_cache;

    static auto encodePath(const QString &data) -> QByteArray;
    static auto replyHasError(QNetworkReply *reply) -> bool;
    static auto makeAndPrintError(const QString &errorMessage, const QNetworkReply *reply) -> QString;
    static auto replyErrorToString(const QNetworkReply *reply) -> QString;
    static auto makeMoveHeaders(const QString &newPath) -> QList<std::pair<QByteArray, QByteArray>>;

    template <typename T> static auto deleteReplyAndReturn(T value, QNetworkReply *reply) -> T;

    template <typename T1, typename T2>
    auto createDirThenContinue(const QString &dir, const T1 &arg1, const T2 &arg2,
                               const std::function<QFuture<std::shared_ptr<FileResult>>(const T1 &, const T2 &)> &func)
        -> QFuture<std::shared_ptr<FileResult>>;

    auto parseListResponse(const QByteArray &data, const QString &path, bool files, bool folders)
        -> std::shared_ptr<FileListResult>;
};

} // namespace Files
