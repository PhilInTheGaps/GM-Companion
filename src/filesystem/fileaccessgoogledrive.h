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
public:
    explicit FileAccessGoogleDrive(GoogleDrive &googleDrive);

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
    GoogleDrive &m_gd;
    FileCache m_fileCache;
    FileCache m_idCache;

    auto createFileAsync(const QString &path, const QByteArray &mimeType) -> QFuture<std::shared_ptr<FileResult>>;
    auto updateFileContent(const QString &id, const QString &path, const QByteArray &data)
        -> QFuture<std::shared_ptr<FileResult>>;

    auto getFolderEntryIds(const QString &parentId, const QString &parentPath,
                           const QString &pageToken = QLatin1String()) -> QFuture<void>;
    auto getFolderEntryIds(const std::pair<QString, QByteArray> &dir, const QString &pageToken = QLatin1String())
        -> QFuture<void>;
    auto getFileIdAsync(const QString &path) -> QFuture<QString>;
    auto getParentIdAsync(const QString &path) -> QFuture<std::pair<QString, QByteArray>>;

    static auto makeMetaData(const QString &name, const QString &parentId = QLatin1String(),
                             const QByteArray &mimeType = {}) -> QByteArray;

    auto listAsync(const QString &path, const QString &q, const QString &pageToken = QLatin1String())
        -> QFuture<std::shared_ptr<FileListResult>>;
    static auto makeListQuery(const QString &id, bool files, bool folders) -> QString;
};

} // namespace Files
