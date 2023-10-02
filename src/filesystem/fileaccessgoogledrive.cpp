#include "fileaccessgoogledrive.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QPromise>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmFileAccessGoogle, "gm.files.access.google")

class GoogleDriveMimeType
{
public:
    static constexpr auto FOLDER = "application/vnd.google-apps.folder";
    static constexpr auto FILE = "application/vnd.google-apps.file";
    static constexpr auto UNKNOWN = "application/vnd.google-apps.unknown";
    static constexpr auto AUDIO = "application/vnd.google-apps.audio";
    static constexpr auto VIDEO = "application/vnd.google-apps.video";
    static constexpr auto PHOTO = "application/vnd.google-apps.photo";
};

constexpr auto FILES_ENDPOINT = "https://www.googleapis.com/drive/v3/files";
constexpr auto UPLOAD_ENDPOINT = "https://www.googleapis.com/upload/drive/v3/files";
constexpr auto BATCH_ENDPOINT = "https://www.googleapis.com/batch/drive/v3";

using namespace Files;

FileAccessGoogleDrive::FileAccessGoogleDrive(GoogleDrive &googleDrive) : m_gd(googleDrive)
{
}

auto FileAccessGoogleDrive::getDataAsync(const QString &path, bool allowCache)
    -> QFuture<std::shared_ptr<FileDataResult>>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << path << "," << allowCache << ")";

    if (QByteArray data; allowCache && m_fileCache.tryGetData(path, data))
    {
        // Return file from cache
        return QtFuture::makeReadyFuture(std::make_shared<FileDataResult>(data));
    }

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileDataResult>>>();
    promise->start();

    auto errorCallback = [promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not download file!" << error;
        promise->addResult(std::make_shared<FileDataResult>(error));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(context(),
              [this, path, promise, errorCallback](const QString &id) mutable {
                  const auto url = QUrl(u"%1/%2?alt=media"_s.arg(FILES_ENDPOINT, id));

                  m_gd.get(url)
                      .then(context(),
                            [this, path, promise](RestNetworkReply *reply) mutable {
                                if (reply->error() == QNetworkReply::NoError)
                                {
                                    qCDebug(gmFileAccessGoogle()) << "Successfully downloaded file" << path;
                                    m_fileCache.createOrUpdateEntry(path, reply->data());
                                }
                                else
                                {
                                    qCWarning(gmFileAccessGoogle()) << reply->errorText();
                                }

                                promise->addResult(FileDataResult::fromNetworkReply(reply));
                                promise->finish();
                            })
                      .onCanceled(context(),
                                  [errorCallback]() mutable { errorCallback(u"Could not get GoogleDrive ID"_s); });
              })
        .onCanceled(context(), [errorCallback]() mutable { errorCallback(u"Could not get GoogleDrive ID"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiGetDataAsync(std::make_shared<MultiGetHelper<FileDataResult>>(paths), allowCache);
}

auto FileAccessGoogleDrive::saveAsync(const QString &path, const QByteArray &data)
    -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "saveAsync(" << path << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileResult>>>();
    promise->start();

    auto errorCallback = [promise]() mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not save file content!";
        promise->future().cancel();
    };

    auto updateExistingFile = [this, path, data, promise, errorCallback](const QString &id) mutable {
        updateFileContent(id, path, data)
            .then(context(),
                  [promise](std::shared_ptr<FileResult> result) mutable {
                      promise->addResult(std::move(result));
                      promise->finish();
                  })
            .onCanceled(context(), errorCallback);
    };

    auto createNewFile = [this, path, updateExistingFile, promise, errorCallback]() mutable {
        // Create file first
        createFileAsync(path, QByteArray())
            .then(context(),
                  [this, path, updateExistingFile, errorCallback](std::shared_ptr<FileResult>) mutable {
                      // Get file id and then update the file with data
                      getFileIdAsync(path).then(context(), updateExistingFile).onCanceled(context(), errorCallback);
                  })
            .onCanceled(context(), errorCallback);
    };

    getFileIdAsync(path).then(context(), updateExistingFile).onCanceled(context(), createNewFile);

    return promise->future();
}

auto FileAccessGoogleDrive::updateFileContent(const QString &id, const QString &path, const QByteArray &data)
    -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "updateFile(" << path << ")";

    const auto url = QUrl(u"%1/%2?uploadType=resumable"_s.arg(UPLOAD_ENDPOINT, id));
    const auto request = NetworkUtils::makeJsonRequest(url);

    const auto errorCallback = []() {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not update file!";
        return QFuture<std::shared_ptr<FileResult>>();
    };

    // Tell GoogleDrive that we want to upload a file
    return m_gd.customRequest(request, "PATCH", "")
        .then(context(),
              [this, path, data, errorCallback](RestNetworkReply *reply) {
                  if (reply->error() != QNetworkReply::NoError)
                  {
                      qCWarning(gmFileAccessGoogle()) << "Error during file upload:" << reply->errorText();
                      return QtFuture::makeReadyFuture(std::make_shared<FileResult>(false, reply->errorText()));
                  }

                  auto uploadUrl = reply->getHeader("location");

                  if (uploadUrl.isEmpty()) uploadUrl = reply->getHeader("Location");

                  reply->deleteLater();

                  if (uploadUrl.isEmpty())
                  {
                      qCWarning(gmFileAccessGoogle()) << "Did not receive an upload URL for file" << path;
                      return QFuture<std::shared_ptr<FileResult>>();
                  }

                  qCDebug(gmFileAccessGoogle()) << "Received upload URL for file" << path;

                  auto request = QNetworkRequest(QUrl(QString::fromUtf8(uploadUrl)));
                  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
                  request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

                  return m_gd.put(request, data)
                      .then(context(),
                            [this, path, data](RestNetworkReply *reply) {
                                if (reply->error() == QNetworkReply::NoError)
                                {
                                    qCDebug(gmFileAccessGoogle()) << "Successfully updated file " << path;
                                    m_fileCache.createOrUpdateEntry(path, data);
                                }
                                else
                                {
                                    qCWarning(gmFileAccessGoogle()) << reply->errorText();
                                }

                                return QtFuture::makeReadyFuture(FileResult::fromNetworkReply(reply));
                            })
                      .onCanceled(context(), errorCallback)
                      .unwrap();
              })
        .onCanceled(context(), errorCallback)
        .unwrap();
}

/// Move a file from one path to another
/// 1. Get file ID
/// 2. Get ID of old parent folder
/// 3. Get ID of new parent folder
/// 4. Build new metadata (update name)
/// 5. Send PATCH request
auto FileAccessGoogleDrive::moveAsync(const QString &oldPath, const QString &newPath)
    -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "moveAsync(" << oldPath << "," << newPath << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileResult>>>();
    promise->start();

    auto errorCallback = [promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << "Error during moveAsync():" << error;
        promise->addResult(std::make_shared<FileResult>(error));
        promise->finish();
    };

    auto createNewParentFolderAndTryAgain = [this, oldPath, newPath, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(newPath);

        createDirAsync(parentPath)
            .then(context(),
                  [this, oldPath, newPath, promise](std::shared_ptr<FileResult>) mutable {
                      moveAsync(oldPath, newPath).then(context(), [promise](std::shared_ptr<FileResult> result) {
                          promise->addResult(std::move(result));
                          promise->finish();
                      });
                  })
            .onCanceled(context(),
                        [errorCallback]() mutable { errorCallback(u"Could not create new parent folder"_s); });
    };

    getFileIdAsync(oldPath)
        .then(context(),
              [this, oldPath, newPath, promise, errorCallback,
               createNewParentFolderAndTryAgain](const QString &fileId) mutable {
                  getParentIdAsync(oldPath)
                      .then(context(),
                            [this, fileId, oldPath, newPath, promise, errorCallback,
                             createNewParentFolderAndTryAgain](const std::pair<QString, QByteArray> &oldPair) mutable {
                                getParentIdAsync(newPath)
                                    .then(context(),
                                          [this, fileId, oldPath, newPath, oldPair, promise, errorCallback,
                                           createNewParentFolderAndTryAgain](
                                              const std::pair<QString, QByteArray> &newPair) mutable {
                                              const auto filename = FileUtils::fileName(newPath);
                                              const auto metadata = makeMetaData(filename);

                                              const auto url = QUrl(u"%1/%2?addParents=%3&removeParents=%4"_s.arg(
                                                  FILES_ENDPOINT, fileId, newPair.second, oldPair.second));
                                              const auto request = NetworkUtils::makeJsonRequest(url);

                                              m_gd.customRequest(request, "PATCH", metadata)
                                                  .then(context(),
                                                        [this, oldPath, newPath,
                                                         promise](RestNetworkReply *reply) mutable {
                                                            qCDebug(gmFileAccessGoogle())
                                                                << "Successfully moved file/folder from" << oldPath
                                                                << "to" << newPath;

                                                            m_fileCache.moveEntry(oldPath, newPath);
                                                            m_idCache.moveEntry(oldPath, newPath);

                                                            promise->addResult(FileResult::fromNetworkReply(reply));
                                                            promise->finish();
                                                        })
                                                  .onCanceled(context(), [errorCallback]() mutable {
                                                      errorCallback(u"Could not send PATCH request"_s);
                                                  });
                                          })
                                    .onCanceled(context(), createNewParentFolderAndTryAgain);
                            })
                      .onCanceled(context(), [errorCallback]() mutable {
                          errorCallback(u"Could not find ID of old parent folder"_s);
                      });
              })
        .onCanceled(context(), [errorCallback]() mutable { errorCallback(u"Could not find file/folder ID"_s); });

    return promise->future();
}

/// Delete a file/folder
/// 1. Get file/folder ID
/// 2. Send DELETE request
auto FileAccessGoogleDrive::deleteAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "deleteAsync(" << path << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileResult>>>();
    promise->start();

    auto errorCallback = [path, promise]() mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not delete file" << path << "!";
        promise->addResult(std::make_shared<FileResult>(false));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(context(),
              [this, path, promise, errorCallback](const QString &id) mutable {
                  const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, id));
                  const auto request = QNetworkRequest(url);

                  m_gd.customRequest(request, "DELETE", "")
                      .then(context(),
                            [this, path, promise](RestNetworkReply *reply) mutable {
                                if (reply->error() == QNetworkReply::NoError)
                                {
                                    // Save the folder ID
                                    m_idCache.removeEntry(path);
                                    m_fileCache.removeEntry(path);

                                    qCDebug(gmFileAccessGoogle()) << "Successfully deleted file " << path;
                                }
                                else
                                {
                                    qCWarning(gmFileAccessGoogle())
                                        << "Error during deletion of file" << path << ":" << reply->errorText();
                                }

                                promise->addResult(FileResult::fromNetworkReply(reply));
                                promise->finish();
                            })
                      .onCanceled(errorCallback);
              })
        .onCanceled(context(), errorCallback);

    return promise->future();
}

/// Copy a file
/// 1. Make sure the destination file does not exist already
/// 2. Get file ID
/// 3. Get ID of destination folder
/// 4. Build MetaData
/// 5. Send copy request (POST)
auto FileAccessGoogleDrive::copyAsync(const QString &path, const QString &copy) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "copyAsync(" << path << "," << copy << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileResult>>>();
    promise->start();

    auto errorCallback = [promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << error;
        promise->addResult(std::make_shared<FileResult>(false, error));
        promise->finish();
    };

    auto createNewParentFolderAndTryAgain = [this, path, copy, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(copy);

        createDirAsync(parentPath)
            .then(context(),
                  [this, path, copy, promise](std::shared_ptr<FileResult>) mutable {
                      copyAsync(path, copy).then(context(), [promise](std::shared_ptr<FileResult> result) {
                          promise->addResult(std::move(result));
                          promise->finish();
                      });
                  })
            .onCanceled(context(),
                        [errorCallback]() mutable { errorCallback(u"Could not create new parent folder"_s); });
    };

    checkAsync(copy, true)
        .then(context(),
              [this, path, copy, promise, errorCallback,
               createNewParentFolderAndTryAgain](std::shared_ptr<FileCheckResult> result) mutable {
                  if (!result)
                  {
                      errorCallback(u"Tried to check if destination already exists -> received null result"_s);
                      return;
                  }

                  if (result->exists())
                  {
                      errorCallback(u"Destination already exists!"_s);
                      return;
                  }

                  getFileIdAsync(path)
                      .then(context(),
                            [this, path, copy, promise, errorCallback,
                             createNewParentFolderAndTryAgain](const QString &id) mutable {
                                getParentIdAsync(copy)
                                    .then(context(),
                                          [this, path, copy, id, promise,
                                           errorCallback](const std::pair<QString, QByteArray> &parent) mutable {
                                              const auto filename = FileUtils::fileName(copy);
                                              const auto metadata = makeMetaData(filename, parent.second);

                                              const auto url = QUrl(u"%1/%2/copy"_s.arg(FILES_ENDPOINT, id));
                                              const auto request = NetworkUtils::makeJsonRequest(url);

                                              m_gd.post(request, metadata)
                                                  .then(context(),
                                                        [this, path, copy, promise](RestNetworkReply *reply) mutable {
                                                            if (reply->error() == QNetworkReply::NoError)
                                                            {
                                                                // Save the folder ID
                                                                const auto json =
                                                                    QJsonDocument::fromJson(reply->data()).object();
                                                                m_idCache.createOrUpdateEntry(
                                                                    copy, json["id"_L1].toString().toUtf8());
                                                                m_fileCache.copyEntry(path, copy);

                                                                qCDebug(gmFileAccessGoogle())
                                                                    << "Successfully copied file" << path << "to"
                                                                    << copy;
                                                            }
                                                            else
                                                            {
                                                                qCWarning(gmFileAccessGoogle())
                                                                    << "Error during copying of file" << path << ":"
                                                                    << reply->errorText();
                                                            }

                                                            promise->addResult(FileResult::fromNetworkReply(reply));
                                                            promise->finish();
                                                        })
                                                  .onCanceled(context(), [errorCallback]() mutable {
                                                      errorCallback(u"Could not send copy request"_s);
                                                  });
                                          })
                                    .onCanceled(context(), createNewParentFolderAndTryAgain);
                            })
                      .onCanceled(context(), [errorCallback]() mutable { errorCallback(u"Could not get file ID"_s); });
              })
        .onCanceled(context(),
                    [errorCallback]() mutable { errorCallback(u"Could not check if destination already exists!"_s); });

    return promise->future();
}

/// List files and/or folders in a directory
auto FileAccessGoogleDrive::listAsync(const QString &path, bool files, bool folders)
    -> QFuture<std::shared_ptr<FileListResult>>
{
    qCDebug(gmFileAccessGoogle()) << "listAsync(" << path << "," << files << "," << folders << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileListResult>>>();
    promise->start();

    auto errorCallback = [path, promise](const QString &error) mutable {
        promise->addResult(std::make_shared<FileListResult>(path, error));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(context(),
              [this, path, files, folders, promise](const QString &id) mutable {
                  const auto q = makeListQuery(id, files, folders);
                  listAsync(path, q).then(context(), [promise](std::shared_ptr<FileListResult> result) {
                      promise->addResult(std::move(result));
                      promise->finish();
                  });
              })
        .onCanceled(context(), [errorCallback]() mutable { errorCallback(u"Could not get folder id"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::listAsync(const QString &path, const QString &q, const QString &pageToken)
    -> QFuture<std::shared_ptr<FileListResult>>
{
    QUrl url(FILES_ENDPOINT);
    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(q)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem(u"pageToken"_s, pageToken);
    }

    url.setQuery(query);

    return m_gd.get(NetworkUtils::makeJsonRequest(url))
        .then(context(),
              [this, path, q](RestNetworkReply *reply) {
                  if (reply->error() == QNetworkReply::NoError)
                  {
                      const auto json = QJsonDocument::fromJson(reply->data()).object();
                      const auto fileArray = json["files"_L1].toArray();

                      QStringList files;
                      QStringList folders;

                      for (const auto &file : fileArray)
                      {
                          const auto fileName = file["name"_L1].toString();
                          const auto filePath = FileUtils::fileInDir(fileName, path);
                          const auto id = file["id"_L1].toString().toUtf8();
                          const auto mimeType = file["mimeType"_L1].toString();

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
                      if (json.contains("nextPageToken"_L1))
                      {
                          reply->deleteLater();
                          const auto nextPageToken = json["nextPageToken"_L1].toString();

                          return listAsync(path, q, nextPageToken)
                              .then(context(),
                                    [path, files, folders](std::shared_ptr<FileListResult> result) mutable {
                                        files.append(result->files());
                                        folders.append(result->folders());

                                        return QtFuture::makeReadyFuture(
                                            std::make_shared<FileListResult>(path, folders, files));
                                    })
                              .onCanceled(context(), []() { return QFuture<std::shared_ptr<FileListResult>>(); })
                              .unwrap();
                      }

                      return QtFuture::makeReadyFuture(std::make_shared<FileListResult>(path, folders, files));
                  }

                  const auto error = reply->errorText();
                  reply->deleteLater();

                  qCWarning(gmFileAccessGoogle()) << error;
                  return QtFuture::makeReadyFuture(std::make_shared<FileListResult>(path, error));
              })
        .onCanceled(context(), []() { return QFuture<std::shared_ptr<FileListResult>>(); })
        .unwrap();
}

auto FileAccessGoogleDrive::makeListQuery(const QString &id, bool files, bool folders) -> QString
{
    auto q = u"'%1' in parents and trashed = false"_s.arg(id);

    if (folders && !files)
    {
        q.append("and mimeType = 'application/vnd.google-apps.folder'"_L1);
    }
    else if (files && !folders)
    {
        q.append("and mimeType != 'application/vnd.google-apps.folder'"_L1);
    }

    return q;
}

auto FileAccessGoogleDrive::createDirAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "createDirAsync(" << path << ")";
    return createFileAsync(path, GoogleDriveMimeType::FOLDER);
}

/// Create a file or folder
/// 1. Make sure the file/folder does not exist already
/// 2. Get parent folder ID
/// 3. Build MetaData
/// 4. Send "create" request
auto FileAccessGoogleDrive::createFileAsync(const QString &path, const QByteArray &mimeType)
    -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessGoogle()) << "createFileAsync(" << path << "," << mimeType << ")";

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileResult>>>();
    promise->start();

    auto errorCallback = [promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << error;
        promise->addResult(std::make_shared<FileResult>(false, error));
        promise->finish();
    };

    auto createParentDirAndTryAgain = [this, path, mimeType, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(path);
        return createDirAsync(parentPath)
            .then(context(),
                  [this, path, mimeType, promise](std::shared_ptr<FileResult>) mutable {
                      createFileAsync(path, mimeType).then(context(), [promise](std::shared_ptr<FileResult> result) {
                          promise->addResult(std::move(result));
                          promise->finish();
                      });
                  })
            .onCanceled(context(), [errorCallback]() mutable { errorCallback(u"Could not create parent dir!"_s); });
    };

    checkAsync(path, true)
        .then(context(),
              [this, path, mimeType, promise, errorCallback,
               createParentDirAndTryAgain](std::shared_ptr<FileCheckResult> result) mutable {
                  if (!result)
                  {
                      errorCallback(u"Tried to check if file already exists -> received null result"_s);
                      return;
                  }

                  if (result->exists())
                  {
                      errorCallback(u"File already exists!"_s);
                      return;
                  }

                  // Get parent folder ID first
                  getParentIdAsync(path)
                      .then(context(),
                            [this, path, mimeType, promise,
                             errorCallback](const std::pair<QString, QByteArray> &parent) mutable {
                                // Build file meta data
                                const auto data = makeMetaData(FileUtils::fileName(path), parent.second, mimeType);
                                const auto request = NetworkUtils::makeJsonRequest(QUrl(FILES_ENDPOINT));

                                // Send "create" request
                                return m_gd.post(request, data)
                                    .then(context(),
                                          [this, path, mimeType, promise](RestNetworkReply *reply) mutable {
                                              if (reply->error() == QNetworkReply::NoError)
                                              {
                                                  // Save the folder ID
                                                  const auto json = QJsonDocument::fromJson(reply->data()).object();
                                                  m_idCache.createOrUpdateEntry(path,
                                                                                json["id"_L1].toString().toUtf8());

                                                  qCDebug(gmFileAccessGoogle())
                                                      << "Successfully created file" << path << mimeType;
                                              }
                                              else
                                              {
                                                  qCWarning(gmFileAccessGoogle())
                                                      << "Error during creation of file" << path << mimeType << ":"
                                                      << reply->errorText();
                                              }

                                              promise->addResult(FileResult::fromNetworkReply(reply));
                                              promise->finish();
                                          })
                                    .onCanceled(context(), [errorCallback]() mutable {
                                        errorCallback(u"Could not send POST request!"_s);
                                    });
                            })
                      .onCanceled(context(), createParentDirAndTryAgain);
              })
        .onCanceled(context(),
                    [errorCallback]() mutable { errorCallback(u"Could not check if file already exists!"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::checkAsync(const QString &path, bool allowCache)
    -> QFuture<std::shared_ptr<FileCheckResult>>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << path << allowCache << ")";

    if (allowCache && m_fileCache.checkEntry(path))
    {
        // File is still fresh in cache, so we are confident that it exists
        return QtFuture::makeReadyFuture(std::make_shared<FileCheckResult>(path, true));
    }

    auto promise = std::make_shared<QPromise<std::shared_ptr<FileCheckResult>>>();
    promise->start();

    auto errorCallback = [path, promise]() mutable {
        promise->addResult(std::make_shared<FileCheckResult>(path, false));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(context(),
              [this, path, promise](const QString &folderId) mutable {
                  const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, folderId));

                  m_gd.get(url)
                      .then(context(),
                            [this, path, promise](RestNetworkReply *reply) mutable {
                                const auto json = QJsonDocument::fromJson(reply->data()).object();
                                const auto id = json["id"_L1].toString();
                                m_idCache.createOrUpdateEntry(path, id.toUtf8());

                                promise->addResult(FileCheckResult::fromNetworkReply(reply, path));
                                promise->finish();
                            })
                      .onCanceled(context(), [path, promise]() mutable {
                          promise->addResult(std::make_shared<FileCheckResult>(path, false));
                          promise->finish();
                      });
              })
        .onCanceled(context(), errorCallback);

    return promise->future();
}

auto FileAccessGoogleDrive::checkAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::shared_ptr<FileMultiCheckResult>>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiCheckAsync(std::make_shared<MultiGetHelper<FileCheckResult>>(paths), allowCache);
}

/// Find IDs of all files located in a folder
auto FileAccessGoogleDrive::getFolderEntryIds(const QString &parentId, const QString &parentPath,
                                              const QString &pageToken) -> QFuture<void>
{
    QUrl url(FILES_ENDPOINT);

    const auto queryString = u"'%1' in parents and trashed = false"_s.arg(parentId);

    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(queryString)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem(u"pageToken"_s, pageToken);
    }

    url.setQuery(query);

    const auto callback = [this, parentId, parentPath](RestNetworkReply *reply) {
        // Success
        if (reply->error() == QNetworkReply::NoError)
        {
            const auto json = QJsonDocument::fromJson(reply->data()).object();
            const auto files = json["files"_L1].toArray();

            for (const auto &file : files)
            {
                const auto path = FileUtils::fileInDir(file["name"_L1].toString(), parentPath);
                const auto id = file["id"_L1].toString().toUtf8();
                m_idCache.createOrUpdateEntry(path, id);
            }

            if (json.contains("nextPageToken"_L1))
            {
                reply->deleteLater();
                const auto nextPageToken = json["nextPageToken"_L1].toString();
                return getFolderEntryIds(parentId, parentPath, nextPageToken);
            }
        }
        else
        {
            qCWarning(gmFileAccessGoogle()) << reply->error();
        }

        reply->deleteLater();
        return QtFuture::makeReadyFuture();
    };

    const auto errorCallback = []() { return QFuture<void>(); };

    return m_gd.get(url).then(context(), callback).onCanceled(context(), errorCallback).unwrap();
}

auto FileAccessGoogleDrive::getFolderEntryIds(const std::pair<QString, QByteArray> &dir, const QString &pageToken)
    -> QFuture<void>
{
    return getFolderEntryIds(dir.second, dir.first, pageToken);
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

    // This is executed when the file id could not be found
    const auto errorCallback = []() { return QFuture<QString>(); };

    // This is executed after ids in parent folder have been found
    const auto callback = [this, path, errorCallback]() {
        if (QByteArray id; m_idCache.tryGetData(path, id))
        {
            return QtFuture::makeReadyFuture(QString(id));
        }

        return errorCallback();
    };

    return getParentIdAsync(path)
        .then(context(),
              [this, path, callback, errorCallback](const std::pair<QString, QByteArray> &parentPair) {
                  // Has ID been found in the meantime?
                  if (QByteArray id; m_idCache.tryGetData(path, id))
                  {
                      return QtFuture::makeReadyFuture(QString(id));
                  }

                  return getFolderEntryIds(parentPair)
                      .then(context(), callback)
                      .onCanceled(context(), errorCallback)
                      .unwrap();
              })
        .onCanceled(context(), errorCallback)
        .unwrap();
}

/// Get the ID of the parent folder
auto FileAccessGoogleDrive::getParentIdAsync(const QString &path) -> QFuture<std::pair<QString, QByteArray>>
{
    const auto parentPath = FileUtils::dirFromPath(path);

    // top level ID is "root"
    if (parentPath.isEmpty() || parentPath == '/')
    {
        return QtFuture::makeReadyFuture(std::pair(parentPath, QByteArray("root")));
    }

    return getFileIdAsync(parentPath).then(context(), [parentPath](const QString &id) {
        return std::pair(parentPath, id.toUtf8());
    });
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
