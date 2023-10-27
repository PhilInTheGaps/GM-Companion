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
namespace GoogleDrive
{
class CheckImpl;
class CopyImpl;
class CreateFileImpl;
class DeleteImpl;
class GetDataImpl;
class ListImpl;
class MoveImpl;
class SaveImpl;
} // namespace GoogleDrive

class FileAccessGoogleDrive : public FileAccess
{
    friend GoogleDrive::CheckImpl;
    friend GoogleDrive::CopyImpl;
    friend GoogleDrive::CreateFileImpl;
    friend GoogleDrive::DeleteImpl;
    friend GoogleDrive::GetDataImpl;
    friend GoogleDrive::ListImpl;
    friend GoogleDrive::MoveImpl;
    friend GoogleDrive::SaveImpl;

public:
    explicit FileAccessGoogleDrive(Services::GoogleDrive &googleDrive);

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
    Services::GoogleDrive &m_gd;
    FileCache m_fileCache;
    FileCache m_idCache;

    auto createFileAsync(const QString &path, const QByteArray &mimeType) -> QFuture<FileResult>;
    
    auto loadFolderEntries(const QString &parentId, const QString &parentPath,
                           const QString &pageToken = QLatin1String()) -> QFuture<void>;
    auto loadFolderEntries(const std::pair<QString, QString> &dir, const QString &pageToken = QLatin1String())
        -> QFuture<void>;
    auto getFileIdAsync(const QString &path) -> QFuture<QString>;
    auto getParentIdAsync(const QString &path) -> QFuture<std::pair<QString, QString>>;

    static auto makeMetaData(const QString &name, const QString &parentId = QLatin1String(),
                             const QByteArray &mimeType = {}) -> QByteArray;
};

} // namespace Files
