#pragma once

#include <QNetworkReply>
#include <QObject>
#include "fileaccess.h"
#include "multigethelper.h"
#include "cache/filecache.h"

namespace Files
{

class FileAccessNextcloud : public FileAccess
{
    Q_OBJECT

public:
    explicit FileAccessNextcloud(QObject *parent) : FileAccess(parent) {}

    QFuture<FileDataResult*> getDataAsync(const QString &path, bool allowCache) override;
    QFuture<QVector<FileDataResult*>> getDataAsync(const QStringList& paths, bool allowCache) override;
    QFuture<FileResult*> saveAsync(const QString& path, const QByteArray& data) override;
    QFuture<FileResult*> moveAsync(const QString& oldPath, const QString& newPath) override;
    QFuture<FileResult*> deleteAsync(const QString& path) override;
    QFuture<FileResult*> copyAsync(const QString& path, const QString& copy) override;
    QFuture<FileListResult*> listAsync(const QString& path, bool files, bool folders) override;
    QFuture<FileResult*> createDirAsync(const QString& path) override;
    QFuture<FileCheckResult*> checkAsync(const QString& path, bool allowCache) override;
    QFuture<FileMultiCheckResult*> checkAsync(const QStringList& paths, bool allowCache) override;

private:
    FileCache m_cache;

    static QByteArray encodePath(const QString& data);
    static inline bool replyHasError(QNetworkReply *reply);
    static QString makeAndPrintError(const QString& errorMessage, const QNetworkReply *reply);
    static QString replyErrorToString(const QNetworkReply *reply);
    static QList<QPair<QByteArray, QByteArray>> makeMoveHeaders(const QString &newPath);

    template<typename T>
    static T deleteReplyAndReturn(const T& value, QNetworkReply *reply);

    template<typename T1, typename T2>
    QFuture<FileResult*> createDirThenContinue(const QString& dir, const T1 &arg1, const T2 &arg2,
                             const std::function<QFuture<FileResult*>(const T1&, const T2&)>& func);

    FileListResult* parseListResponse(const QByteArray& data, const QString& path, bool files, bool folders);

    QFuture<QVector<FileDataResult*>> getDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache);
    QFuture<FileMultiCheckResult*> checkAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache);
};

}

