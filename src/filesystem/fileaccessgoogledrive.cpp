#include "fileaccessgoogledrive.h"
#include "googledrive/checkimpl.h"
#include "googledrive/copyimpl.h"
#include "googledrive/createfileimpl.h"
#include "googledrive/deleteimpl.h"
#include "googledrive/endpoints.h"
#include "googledrive/getdataimpl.h"
#include "googledrive/listimpl.h"
#include "googledrive/mimetype.h"
#include "googledrive/moveimpl.h"
#include "googledrive/saveimpl.h"
#include "utils/fileutils.h"
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

Q_LOGGING_CATEGORY(gmFileAccessGoogle, "gm.files.access.google")

FileAccessGoogleDrive::FileAccessGoogleDrive(Services::GoogleDrive &googleDrive) : m_gd(googleDrive)
{
}

auto FileAccessGoogleDrive::getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << path << "," << allowCache << ")";

    if (QByteArray data; allowCache && m_fileCache.tryGetData(path, data))
    {
        // Return file from cache
        return QtFuture::makeReadyFuture(FileDataResult(data));
    }

    return GetDataImpl::getData(path, this);
}

auto FileAccessGoogleDrive::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<FileDataResult>>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult>(paths), allowCache);
}

auto FileAccessGoogleDrive::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "saveAsync(" << path << ")";
    return SaveImpl::save(path, data, this);
}

auto FileAccessGoogleDrive::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "moveAsync(" << oldPath << "," << newPath << ")";
    return MoveImpl::move(oldPath, newPath, this);
}

auto FileAccessGoogleDrive::deleteAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "deleteAsync(" << path << ")";
    return DeleteImpl::deleteFile(path, this);
}

auto FileAccessGoogleDrive::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "copyAsync(" << path << "," << copy << ")";
    return CopyImpl::copy(path, copy, this);
}

/// List files and/or folders in a directory
auto FileAccessGoogleDrive::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult>
{
    qCDebug(gmFileAccessGoogle()) << "listAsync(" << path << "," << files << "," << folders << ")";
    return ListImpl::list(path, files, folders, this);
}

auto FileAccessGoogleDrive::createDirAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "createDirAsync(" << path << ")";
    return createFileAsync(path, MimeType::FOLDER);
}

auto FileAccessGoogleDrive::createFileAsync(const QString &path, const QByteArray &mimeType) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessGoogle()) << "createFileAsync(" << path << "," << mimeType << ")";
    return CreateFileImpl::create(path, mimeType, this);
}

auto FileAccessGoogleDrive::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << path << allowCache << ")";

    if (allowCache && m_fileCache.checkEntry(path))
    {
        // File is still fresh in cache, so we are confident that it exists
        return QtFuture::makeReadyFuture(FileCheckResult(path, true));
    }

    return CheckImpl::check(path, this);
}

auto FileAccessGoogleDrive::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult>(paths), allowCache);
}

/// Find IDs of all files located in a folder
auto FileAccessGoogleDrive::loadFolderEntries(const QString &parentId, const QString &parentPath,
                                              const QString &pageToken) -> QFuture<void>
{
    if (parentId.isEmpty()) return {};

    QUrl url(FILES_ENDPOINT);
    const auto queryString = u"'%1' in parents and trashed = false"_s.arg(parentId);
    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(queryString)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem(u"pageToken"_s, pageToken);
    }

    url.setQuery(query);

    const auto callback = [this, parentId, parentPath](RestReply reply) {
        // Success
        if (!reply.hasError())
        {
            const auto json = QJsonDocument::fromJson(reply.data()).object();

            foreach (const auto &file, json["files"_L1].toArray())
            {
                const auto path = FileUtils::fileInDir(file["name"_L1].toString(), parentPath);
                const auto id = file["id"_L1].toString().toUtf8();
                m_idCache.createOrUpdateEntry(path, id);
            }

            if (json.contains("nextPageToken"_L1))
            {
                const auto nextPageToken = json["nextPageToken"_L1].toString();
                return loadFolderEntries(parentId, parentPath, nextPageToken);
            }
        }
        else
        {
            qCWarning(gmFileAccessGoogle()) << reply.error();
        }

        return QtFuture::makeReadyFuture();
    };

    return m_gd.get(url).then(callback).onCanceled([]() { return QtFuture::makeReadyFuture(); }).unwrap();
}

auto FileAccessGoogleDrive::loadFolderEntries(const std::pair<QString, QString> &dir, const QString &pageToken)
    -> QFuture<void>
{
    if (dir.second.isEmpty())
    {
        qCDebug(gmFileAccessGoogle()) << "loadFolderEntries(): id is empty!";
        return {};
    }

    return loadFolderEntries(dir.second, dir.first, pageToken);
}

/// Find the ID of a file
auto FileAccessGoogleDrive::getFileIdAsync(const QString &path) -> QFuture<QString>
{
    qCDebug(gmFileAccessGoogle()) << "getFileId(" << path << ")";

    if (path.isEmpty()) return QtFuture::makeReadyFuture(u"root"_s);

    // First check if id is in cache
    if (QByteArray id; m_idCache.tryGetData(path, id))
    {
        return QtFuture::makeReadyFuture(QString(id));
    }

    // This is executed after ids in parent folder have been found
    const auto callback = [this, path]() {
        if (QByteArray id; m_idCache.tryGetData(path, id))
        {
            return QString(id);
        }

        return QStringLiteral();
    };

    return getParentIdAsync(path)
        .then(context(),
              [this, path, callback](const std::pair<QString, QString> &parentPair) {
                  // Has ID been found in the meantime?
                  if (QByteArray id; m_idCache.tryGetData(path, id))
                  {
                      return QtFuture::makeReadyFuture(QString(id));
                  }

                  return loadFolderEntries(parentPair).then(callback);
              })
        .unwrap();
}

/// Get the ID of the parent folder
auto FileAccessGoogleDrive::getParentIdAsync(const QString &path) -> QFuture<std::pair<QString, QString>>
{
    const auto parentPath = FileUtils::dirFromPath(path);

    // top level ID is "root"
    if (parentPath.isEmpty() || parentPath == '/')
    {
        return QtFuture::makeReadyFuture(std::make_pair(parentPath, u"root"_s));
    }

    return getFileIdAsync(parentPath).then([parentPath](const QString &id) { return std::make_pair(parentPath, id); });
}

/// Construct file metadata in json format
auto FileAccessGoogleDrive::makeMetaData(const QString &name, const QString &parentId, const QByteArray &mimeType)
    -> QByteArray
{
    auto json = QJsonObject({{"name", name}});

    if (!parentId.isEmpty())
    {
        json["parents"_L1] = QJsonArray({parentId});
    }

    if (!mimeType.isEmpty())
    {
        json["mimeType"_L1] = QString(mimeType);
    }

    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}
