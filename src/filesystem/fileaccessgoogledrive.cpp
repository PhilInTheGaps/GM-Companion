#include "fileaccessgoogledrive.h"
#include "google/googledrive.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "logging.h"
#include "thirdparty/o2/src/o0globals.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QHttpMultiPart>

using namespace Files;

auto FileAccessGoogleDrive::getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult *>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << path << "," << allowCache << ")";

    QByteArray data;
    if (allowCache && m_fileCache.tryGetData(path, data))
    {
        // Return file from cache
        return AsyncFuture::completed(new FileDataResult(data, this));
    }

    auto deferred = AsyncFuture::deferred<FileDataResult*>();

    auto errorCallback = [this, deferred](const QString &error) mutable
    {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not download file!";
        deferred.complete(new FileDataResult(error, this));
    };

    AsyncFuture::observe(getFileIdAsync(path))
            .subscribe([this, path, deferred, errorCallback](const QString &id) mutable
    {
        const auto url = QUrl(QString("%1/%2?alt=media").arg(FILES_ENDPOINT, id));

        AsyncFuture::observe(GoogleDrive::getInstance()->get(url))
                .subscribe([this, path, deferred](RestNetworkReply *reply) mutable
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                qCDebug(gmFileAccessGoogle()) << "Successfully downloaded file" << path;
                m_fileCache.createOrUpdateEntry(path, reply->data());
            }
            else
            {
                qCWarning(gmFileAccessGoogle()) << reply->errorText();
            }

            deferred.complete(FileDataResult::fromNetworkReply(reply, this));
        }, [errorCallback]() mutable
        {
            errorCallback(QStringLiteral("Could not get GoogleDrive ID"));
        });

    }, [errorCallback]() mutable
    {
        errorCallback(QStringLiteral("Could not get GoogleDrive ID"));
    });

    return deferred.future();
}

auto FileAccessGoogleDrive::getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<QVector<FileDataResult *> >
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << paths << "," << allowCache << ")";
    return FileAccess::getDataAsync(new MultiGetHelper<FileDataResult>(paths, this), allowCache);
}

auto FileAccessGoogleDrive::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "saveAsync(" << path << ")";

    auto deferred = AsyncFuture::deferred<FileResult*>();

    auto errorCallback = [deferred]() mutable
    {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not save file content!";
        deferred.cancel();
    };

    auto updateExistingFile = [this, path, data, deferred, errorCallback](const QString &id) mutable
    {
        AsyncFuture::observe(updateFileContent(id, path, data))
                .subscribe([deferred](FileResult *result) mutable
        {
            deferred.complete(result);
        }, errorCallback);
    };

    auto createNewFile = [this, path, data, updateExistingFile, deferred, errorCallback]() mutable
    {
        // Create file first
        AsyncFuture::observe(createFileAsync(path, QByteArray()))
                .subscribe([this, path, updateExistingFile, deferred, errorCallback]()
        {
            // Get file id and then update the file with data
            AsyncFuture::observe(getFileIdAsync(path))
                    .subscribe(updateExistingFile, errorCallback);
        }, errorCallback);
    };

    AsyncFuture::observe(getFileIdAsync(path)).subscribe(updateExistingFile, createNewFile);

    return deferred.future();
}

auto FileAccessGoogleDrive::updateFileContent(const QString &id, const QString &path, const QByteArray &data) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "updateFile(" << path << ")";

    const auto url = QUrl(QString("%1/%2?uploadType=resumable").arg(UPLOAD_ENDPOINT, id));
    const auto request = makeJsonRequest(url);

    const auto errorCallback = []()
    {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not update file!";
        return QFuture<FileResult *>();
    };

    // Tell GoogleDrive that we want to upload a file
    return AsyncFuture::observe(GoogleDrive::getInstance()->customRequest(request, "PATCH", ""))
            .subscribe([this, path, data, errorCallback](RestNetworkReply *reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmFileAccessGoogle()) << "Error during file upload:" << reply->errorText();
            return AsyncFuture::completed(new FileResult(false, reply->errorText(), this));
        }

        qCDebug(gmFileAccessGoogle()) << "Received upload URL for file" << path;

        const auto uploadUrl = reply->getHeader("Location");
        reply->deleteLater();

        auto request = QNetworkRequest(QUrl(QString::fromUtf8(uploadUrl)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

        return AsyncFuture::observe(GoogleDrive::getInstance()->put(request, data))
                .subscribe([this, path, data](RestNetworkReply *reply)
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                qCDebug(gmFileAccessGoogle()) << "Successfully updated file" << path;
                m_fileCache.createOrUpdateEntry(path, data);
            }
            else
            {
                qCWarning(gmFileAccessGoogle()) << reply->errorText();
            }

            return FileResult::fromNetworkReply(reply, this);
        }, errorCallback).future();
    }, errorCallback).future();
}

/// Move a file from one path to another
/// 1. Get file ID
/// 2. Get ID of old parent folder
/// 3. Get ID of new parent folder
/// 4. Build new metadata (update name)
/// 5. Send PATCH request
auto FileAccessGoogleDrive::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "moveAsync(" << oldPath << "," << newPath << ")";

    auto deferred = AsyncFuture::deferred<FileResult*>();

    auto errorCallback = [this, deferred](const QString &error) mutable
    {
        qCWarning(gmFileAccessGoogle()) << "Error during moveAsync():" << error;
        deferred.complete(new FileResult(error, this));
    };

    auto createNewParentFolderAndTryAgain = [this, oldPath, newPath, deferred, errorCallback]() mutable
    {
        const auto parentPath = FileUtils::dirFromPath(newPath);

        AsyncFuture::observe(createDirAsync(parentPath))
                .subscribe([this, oldPath, newPath, deferred]() mutable
        {
            deferred.complete(moveAsync(oldPath, newPath));
        }, [errorCallback]() mutable
        {
            errorCallback(QStringLiteral("Could not create new parent folder"));
        });
    };

    AsyncFuture::observe(getFileIdAsync(oldPath))
            .subscribe([this, oldPath, newPath, deferred, errorCallback, createNewParentFolderAndTryAgain](const QString &fileId) mutable
    {
        AsyncFuture::observe(getParentIdAsync(oldPath))
                .subscribe([this, fileId, oldPath, newPath, deferred, errorCallback,createNewParentFolderAndTryAgain](const QPair<QString, QByteArray> &oldPair) mutable
        {
            AsyncFuture::observe(getParentIdAsync(newPath))
                    .subscribe([this, fileId, oldPath, newPath, oldPair, deferred, errorCallback, createNewParentFolderAndTryAgain](const QPair<QString, QByteArray> &newPair) mutable
            {
                const auto filename = FileUtils::fileName(newPath);
                const auto metadata = makeMetaData(filename);

                const auto url = QUrl(QString("%1/%2?addParents=%3&removeParents=%4")
                                      .arg(FILES_ENDPOINT, fileId, newPair.second, oldPair.second));
                const auto request = makeJsonRequest(url);

                AsyncFuture::observe(GoogleDrive::getInstance()->customRequest(request, "PATCH", metadata))
                        .subscribe([this, oldPath, newPath, deferred](RestNetworkReply *reply) mutable
                {
                    qCDebug(gmFileAccessGoogle()) << "Successfully moved file/folder from" << oldPath << "to" << newPath;

                    m_fileCache.moveEntry(oldPath, newPath);
                    m_idCache.moveEntry(oldPath, newPath);

                    deferred.complete(FileResult::fromNetworkReply(reply, this));
                }, [errorCallback]() mutable
                {
                    errorCallback(QStringLiteral("Could not send PATCH request"));
                });

            }, createNewParentFolderAndTryAgain);

        }, [errorCallback]() mutable
        {
            errorCallback(QStringLiteral("Could not find ID of old parent folder"));
        });

    }, [errorCallback]() mutable
    {
        errorCallback(QStringLiteral("Could not find file/folder ID"));
    });

    return deferred.future();
}

/// Delete a file/folder
/// 1. Get file/folder ID
/// 2. Send DELETE request
auto FileAccessGoogleDrive::deleteAsync(const QString &path) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "deleteAsync(" << path << ")";

    auto deferred = AsyncFuture::deferred<FileResult*>();

    const auto errorCallback = [this, path, deferred]() mutable
    {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not delete file" << path << "!";
        deferred.complete(new FileResult(false, this));
    };

    AsyncFuture::observe(getFileIdAsync(path))
            .subscribe([this, path, deferred, errorCallback](const QString &id) mutable
    {
        const auto url = QUrl(QString("%1/%2").arg(FILES_ENDPOINT, id));
        const auto request = QNetworkRequest(url);

        AsyncFuture::observe(GoogleDrive::getInstance()->customRequest(request, "DELETE", ""))
                .subscribe([this, path, deferred](RestNetworkReply *reply) mutable
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                // Save the folder ID
                m_idCache.removeEntry(path);
                m_fileCache.removeEntry(path);

                qCDebug(gmFileAccessGoogle()) << "Successfully deleted file" << path;
            }
            else
            {
                qCWarning(gmFileAccessGoogle()) << "Error during deletion of file" << path << ":" << reply->errorText();
            }

            deferred.complete(AsyncFuture::completed(FileResult::fromNetworkReply(reply, this)));
        }, errorCallback);
    }, errorCallback);

    return deferred.future();
}

/// Copy a file
/// 1. Make sure the destination file does not exist already
/// 2. Get file ID
/// 3. Get ID of destination folder
/// 4. Build MetaData
/// 5. Send copy request (POST)
auto FileAccessGoogleDrive::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "copyAsync(" << path << "," << copy << ")";

    auto deferred = AsyncFuture::deferred<FileResult*>();

    auto errorCallback = [this, deferred](const QString &error) mutable
    {
        qCWarning(gmFileAccessGoogle()) << error;
        deferred.complete(new FileResult(false, error, this));
    };

    auto createNewParentFolderAndTryAgain = [this, path, copy, deferred, errorCallback]() mutable
    {
        const auto parentPath = FileUtils::dirFromPath(copy);

        AsyncFuture::observe(createDirAsync(parentPath))
                .subscribe([this, path, copy, deferred]() mutable
        {
            deferred.complete(copyAsync(path, copy));
        },[errorCallback]() mutable
        {
            errorCallback(QStringLiteral("Could not create new parent folder"));
        });
    };

    AsyncFuture::observe(checkAsync(copy, true))
            .subscribe([this, path, copy, deferred, errorCallback, createNewParentFolderAndTryAgain](FileCheckResult *result) mutable
    {
        if (!result)
        {
            errorCallback(QStringLiteral("Tried to check if destination already exists -> received null result"));
            return;
        }

        if (result->exists())
        {
            errorCallback(QStringLiteral("Destination already exists!"));
            return;
        }

        result->deleteLater();

        AsyncFuture::observe(getFileIdAsync(path)).subscribe([this, path, copy, deferred, errorCallback, createNewParentFolderAndTryAgain](const QString &id) mutable
        {
            AsyncFuture::observe(getParentIdAsync(copy)).subscribe([this, path, copy, id, deferred, errorCallback](const QPair<QString, QByteArray> &parent) mutable
            {
                const auto filename = FileUtils::fileName(copy);
                const auto metadata = makeMetaData(filename, parent.second);

                const auto url = QUrl(QString("%1/%2/copy").arg(FILES_ENDPOINT, id));
                const auto request = makeJsonRequest(url);

                AsyncFuture::observe(GoogleDrive::getInstance()->post(request, metadata))
                        .subscribe([this, path, copy, deferred](RestNetworkReply *reply) mutable
                {
                    if (reply->error() == QNetworkReply::NoError)
                    {
                        // Save the folder ID
                        const auto json = QJsonDocument::fromJson(reply->data()).object();
                        m_idCache.createOrUpdateEntry(copy, json["id"].toString().toUtf8());
                        m_fileCache.copyEntry(path, copy);

                        qCDebug(gmFileAccessGoogle()) << "Successfully copied file" << path << "to" << copy;
                    }
                    else
                    {
                        qCWarning(gmFileAccessGoogle()) << "Error during copying of file" << path << ":" << reply->errorText();
                    }

                    deferred.complete(FileResult::fromNetworkReply(reply, this));
                }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not send copy request")); });
            }, createNewParentFolderAndTryAgain);
        }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not get file ID")); });
    }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not check if destination already exists!")); });

    return deferred.future();
}

/// List files and/or folders in a directory
auto FileAccessGoogleDrive::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult *>
{
    qCDebug(gmFileAccessGoogle()) << "listAsync(" << path << "," << files << "," << folders << ")";

    auto deferred = AsyncFuture::deferred<FileListResult*>();

    auto errorCallback = [this, path, deferred](const QString &error) mutable
    {
        deferred.complete(new FileListResult(path, error, this));
    };

    AsyncFuture::observe(getFileIdAsync(path)).subscribe([this, path, files, folders, deferred](const QString &id) mutable
    {
        const auto q = makeListQuery(id, files, folders);
        deferred.complete(listAsync(path, q));
    },
    [errorCallback]() mutable { errorCallback(QStringLiteral("Could not get folder id")); });

    return deferred.future();
}

auto FileAccessGoogleDrive::listAsync(const QString &path, const QString &q, const QString &pageToken) -> QFuture<FileListResult *>
{
    QUrl url(FILES_ENDPOINT);
    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(q)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem("pageToken", pageToken);
    }

    url.setQuery(query);

    return AsyncFuture::observe(GoogleDrive::getInstance()->get(makeJsonRequest(url)))
            .subscribe([this, path, q](RestNetworkReply *reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            const auto json = QJsonDocument::fromJson(reply->data()).object();
            const auto fileArray = json["files"].toArray();

            QStringList files;
            QStringList folders;

            for (const auto &file : fileArray)
            {
                const auto fileName = file["name"].toString();
                const auto filePath = FileUtils::fileInDir(fileName, path);
                const auto id = file["id"].toString().toUtf8();
                const auto mimeType = file["mimeType"].toString();

                if (mimeType == GoogleDriveMimeType::FOLDER)
                {
                    folders << fileName;
                }
                else
                {
                    files << fileName;
                }

                m_idCache.createOrUpdateEntry(filePath, id);
            }

            // Get the next page
            if (json.contains("nextPageToken"))
            {
                reply->deleteLater();
                const auto nextPageToken = json["nextPageToken"].toString();

                return AsyncFuture::observe(listAsync(path, q, nextPageToken))
                        .subscribe([this, path, files, folders](FileListResult *result) mutable
                {
                    files.append(result->files());
                    folders.append(result->folders());
                    result->deleteLater();

                    return new FileListResult(path, folders, files, this);
                },[]() { return QFuture<FileListResult*>(); }).future();
            }

            return AsyncFuture::completed(new FileListResult(path, folders, files));
        }

        const auto error = reply->errorText();
        reply->deleteLater();

        qCWarning(gmFileAccessGoogle()) << error;
        return AsyncFuture::completed(new FileListResult(path, error, this));
    }, []() { return QFuture<FileListResult*>(); }).future();
}

auto FileAccessGoogleDrive::makeListQuery(const QString &id, bool files, bool folders) -> QString
{
    auto q = QString("'%1' in parents and trashed = false").arg(id);

    if (folders && !files)
    {
        q.append("and mimeType = 'application/vnd.google-apps.folder'");
    }
    else if (files && !folders)
    {
        q.append("and mimeType != 'application/vnd.google-apps.folder'");
    }

    return q;
}

auto FileAccessGoogleDrive::createDirAsync(const QString &path) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "createDirAsync(" << path << ")";
    return createFileAsync(path, GoogleDriveMimeType::FOLDER);
}

/// Create a file or folder
/// 1. Make sure the file/folder does not exist already
/// 2. Get parent folder ID
/// 3. Build MetaData
/// 4. Send "create" request
auto FileAccessGoogleDrive::createFileAsync(const QString &path, const QByteArray &mimeType) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "createFileAsync(" << path << "," << mimeType << ")";

    auto deferred = AsyncFuture::deferred<FileResult*>();

    auto errorCallback = [this, deferred](const QString &error) mutable
    {
        qCWarning(gmFileAccessGoogle()) << error;
        deferred.complete(new FileResult(false, error, this));
    };

    auto createParentDirAndTryAgain = [this, path, mimeType, deferred, errorCallback]() mutable
    {
        const auto parentPath = FileUtils::dirFromPath(path);
        AsyncFuture::observe(createDirAsync(parentPath))
                .subscribe([this, path, mimeType, deferred]() mutable
        {
            deferred.complete(createFileAsync(path, mimeType));
        }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not create parent dir!")); });
    };

    AsyncFuture::observe(checkAsync(path, true))
            .subscribe([this, path, mimeType, deferred, errorCallback, createParentDirAndTryAgain](FileCheckResult *result) mutable
    {
        if (!result)
        {
            errorCallback(QStringLiteral("Tried to check if file already exists -> received null result"));
            return;
        }

        if (result->exists())
        {
            errorCallback(QStringLiteral("File already exists!"));
            return;
        }

        result->deleteLater();

        // Get parent folder ID first
        AsyncFuture::observe(getParentIdAsync(path))
                .subscribe([this, path, mimeType, deferred, errorCallback](const QPair<QString, QByteArray> &parent) mutable
        {
            // Build file meta data
            const auto data = makeMetaData(FileUtils::fileName(path), parent.second, mimeType);
            const auto request = makeJsonRequest(QUrl(FILES_ENDPOINT));

            // Send "create" request
            return AsyncFuture::observe(GoogleDrive::getInstance()->post(request, data))
                    .subscribe([this, path, mimeType, deferred](RestNetworkReply *reply) mutable
            {
                if (reply->error() == QNetworkReply::NoError)
                {
                    // Save the folder ID
                    const auto json = QJsonDocument::fromJson(reply->data()).object();
                    m_idCache.createOrUpdateEntry(path, json["id"].toString().toUtf8());

                    qCDebug(gmFileAccessGoogle()) << "Successfully created file" << path << mimeType;
                }
                else
                {
                    qCWarning(gmFileAccessGoogle()) << "Error during creation of file" << path << mimeType << ":" << reply->errorText();
                }

                deferred.complete(FileResult::fromNetworkReply(reply, this));
            }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not send POST request!")); }).future();
        }, createParentDirAndTryAgain);
    }, [errorCallback]() mutable { errorCallback(QStringLiteral("Could not check if file already exists!")); });

    return deferred.future();
}

auto FileAccessGoogleDrive::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult *>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << path << allowCache << ")";

    if (allowCache && m_fileCache.checkEntry(path))
    {
        m_fileCache.printEntries();

        // File is still fresh in cache, so we are confident that it exists
        return AsyncFuture::completed(new FileCheckResult(path, true, this));
    }

    auto deferred = AsyncFuture::deferred<FileCheckResult *>();

    const auto errorCallback = [this, path, deferred]() mutable
    {
        deferred.complete(new FileCheckResult(path, false, this));
    };

    AsyncFuture::observe(getFileIdAsync(path))
            .subscribe([this, path, deferred](const QString &folderId) mutable
    {
        const auto url = QUrl(QString("%1/%2").arg(FILES_ENDPOINT, folderId));

        AsyncFuture::observe(GoogleDrive::getInstance()->get(url))
                .subscribe([this, path, deferred](RestNetworkReply *reply) mutable
        {
            const auto json = QJsonDocument::fromJson(reply->data()).object();
            const auto id = json["id"].toString();
            m_idCache.createOrUpdateEntry(path, id.toUtf8());

            deferred.complete(FileCheckResult::fromNetworkReply(reply, path, this));
        }, [this, path, deferred]() mutable
        {
            deferred.complete(new FileCheckResult(path, false, this));
        });

        return deferred.future();
    }, errorCallback);

    return deferred.future();
}

auto FileAccessGoogleDrive::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult *>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << paths << "," << allowCache << ")";
    return FileAccess::checkAsync(new MultiGetHelper<FileCheckResult>(paths, this), allowCache);
}

/// Find IDs of all files located in a folder
auto FileAccessGoogleDrive::getFolderEntryIds(const QString &parentId, const QString &parentPath, const QString &pageToken) -> QFuture<void>
{
    QUrl url(FILES_ENDPOINT);

    const auto queryString = QString("'%1' in parents and trashed = false")
            .arg(parentId);

    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(queryString)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem("pageToken", pageToken);
    }

    url.setQuery(query);

    const auto callback = [this, parentId, parentPath](RestNetworkReply *reply)
    {
        // Success
        if (reply->error() == QNetworkReply::NoError)
        {
            const auto json = QJsonDocument::fromJson(reply->data()).object();
            const auto files = json["files"].toArray();

            for (const auto &file : files)
            {
                const auto path = FileUtils::fileInDir(file["name"].toString(), parentPath);
                const auto id = file["id"].toString().toUtf8();
                m_idCache.createOrUpdateEntry(path, id);
            }

            if (json.contains("nextPageToken"))
            {
                reply->deleteLater();
                const auto nextPageToken = json["nextPageToken"].toString();
                return getFolderEntryIds(parentId, parentPath, nextPageToken);
            }
        }
        else
        {
            qCWarning(gmFileAccessGoogle()) << reply->error();
        }

        reply->deleteLater();
        return AsyncFuture::completed();
    };

    const auto errorCallback = []() { return QFuture<void>(); };

    return AsyncFuture::observe(GoogleDrive::getInstance()->get(url))
            .subscribe(callback, errorCallback).future();
}

auto FileAccessGoogleDrive::getFolderEntryIds(const QPair<QString, QByteArray> &dir, const QString &pageToken) -> QFuture<void>
{
    return getFolderEntryIds(dir.second, dir.first, pageToken);
}

/// Find the ID of a file
auto FileAccessGoogleDrive::getFileIdAsync(const QString &path) -> QFuture<QString>
{
    qCDebug(gmFileAccessGoogle()) << "getFileId(" << path << ")";

    if (path.isEmpty()) return AsyncFuture::completed(QStringLiteral("root"));;

    // First check if id is in cache
    QByteArray id;
    if (m_idCache.tryGetData(path, id))
    {
        return AsyncFuture::completed(QString(id));
    }

    // This is executed when the file id could not be found
    const auto errorCallback = [path]() { return QFuture<QString>(); };

    // This is executed after ids in parent folder have been found
    const auto callback = [this, path, errorCallback]()
    {
        QByteArray id;
        if (m_idCache.tryGetData(path, id))
        {
            return AsyncFuture::completed(QString(id));
        }

        return errorCallback();
    };

    return AsyncFuture::observe(getParentIdAsync(path))
            .subscribe([this, path, callback, errorCallback](const QPair<QString, QByteArray> &parentPair)
    {
        // Has ID been found in the meantime?
        QByteArray id;
        if (m_idCache.tryGetData(path, id))
        {
            return AsyncFuture::completed(QString(id));
        }

        return AsyncFuture::observe(getFolderEntryIds(parentPair))
                .subscribe(callback, errorCallback)
                .future();
    }, errorCallback).future();
}

/// Get the ID of the parent folder
auto FileAccessGoogleDrive::getParentIdAsync(const QString &path) -> QFuture<QPair<QString, QByteArray>>
{
    const auto parentPath = FileUtils::dirFromPath(path);

    // top level ID is "root"
    if (parentPath.isEmpty() || parentPath == '/')
    {
        return AsyncFuture::completed(QPair(parentPath, QByteArray("root")));
    }

    return AsyncFuture::observe(getFileIdAsync(parentPath))
            .subscribe([parentPath](const QString &id)
    {
        return AsyncFuture::completed(QPair(parentPath, id.toUtf8()));
    }).future();
}

/// Construct file metadata in json format
auto FileAccessGoogleDrive::makeMetaData(const QString &name, const QString &parentId, const QByteArray &mimeType) -> QByteArray
{
    auto json = QJsonObject({{ "name", name }});

    if (!parentId.isEmpty())
    {
        json["parents"] = QJsonArray({ parentId });
    }

    if (!mimeType.isEmpty())
    {
        json["mimeType"] = QString(mimeType);
    }

    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

/// Construct a network request with content-type application/json
auto FileAccessGoogleDrive::makeJsonRequest(const QUrl &url) -> QNetworkRequest
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, static_cast<const char*>(O2_MIME_TYPE_JSON));
    return request;
}
