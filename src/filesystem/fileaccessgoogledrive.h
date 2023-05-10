#pragma once

#include "cache/filecache.h"
#include "fileaccess.h"
#include "google/googledrive.h"
#include <QJsonArray>
#include <QMap>
#include <QNetworkRequest>
#include <QObject>

namespace Files
{

class FileAccessGoogleDrive : public FileAccess
{
    Q_OBJECT
public:
    explicit FileAccessGoogleDrive(GoogleDrive &googleDrive, QObject *parent);

    auto getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult *> override;
    auto getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<QVector<FileDataResult *>> override;
    auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult *> override;
    auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult *> override;
    auto deleteAsync(const QString &path) -> QFuture<FileResult *> override;
    auto copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult *> override;
    auto listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult *> override;
    auto createDirAsync(const QString &path) -> QFuture<FileResult *> override;
    auto checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult *> override;
    auto checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult *> override;

private:
    GoogleDrive &m_gd;
    FileCache m_fileCache;
    FileCache m_idCache;

    auto createFileAsync(const QString &path, const QByteArray &mimeType) -> QFuture<FileResult *>;
    auto updateFileContent(const QString &id, const QString &path, const QByteArray &data) -> QFuture<FileResult *>;

    auto getFolderEntryIds(const QString &parentId, const QString &parentPath,
                           const QString &pageToken = QLatin1String()) -> QFuture<void>;
    auto getFolderEntryIds(const QPair<QString, QByteArray> &dir, const QString &pageToken = QLatin1String())
        -> QFuture<void>;
    auto getFileIdAsync(const QString &path) -> QFuture<QString>;
    auto getParentIdAsync(const QString &path) -> QFuture<QPair<QString, QByteArray>>;

    static auto makeMetaData(const QString &name, const QString &parentId = QLatin1String(),
                             const QByteArray &mimeType = {}) -> QByteArray;
    static auto makeJsonRequest(const QUrl &url) -> QNetworkRequest;

    auto listAsync(const QString &path, const QString &q, const QString &pageToken = QLatin1String())
        -> QFuture<FileListResult *>;
    static auto makeListQuery(const QString &id, bool files, bool folders) -> QString;
};

} // namespace Files
