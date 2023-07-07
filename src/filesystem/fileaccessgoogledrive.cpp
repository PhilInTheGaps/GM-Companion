#include "fileaccessgoogledrive.h"
#include "thirdparty/o2/src/o0globals.h"
#include "utils/fileutils.h"
#include "utils/stringutils.h"
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QPromise>
#include <QSharedPointer>
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

constexpr ConstQString FILES_ENDPOINT = "https://www.googleapis.com/drive/v3/files";
constexpr ConstQString UPLOAD_ENDPOINT = "https://www.googleapis.com/upload/drive/v3/files";
constexpr ConstQString BATCH_ENDPOINT = "https://www.googleapis.com/batch/drive/v3";

using namespace Files;

FileAccessGoogleDrive::FileAccessGoogleDrive(GoogleDrive &googleDrive, QObject *parent)
    : FileAccess(parent), m_gd(googleDrive)
{
}

auto FileAccessGoogleDrive::getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult *>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << path << "," << allowCache << ")";

    if (QByteArray data; allowCache && m_fileCache.tryGetData(path, data))
    {
        // Return file from cache
        return QtFuture::makeReadyFuture(new FileDataResult(data, this));
    }

    auto promise = QSharedPointer<QPromise<FileDataResult *>>::create();
    promise->start();

    auto errorCallback = [this, promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not download file!" << error;
        promise->addResult(new FileDataResult(error, this));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(this,
              [this, path, promise, errorCallback](const QString &id) mutable {
                  const auto url = QUrl(u"%1/%2?alt=media"_s.arg(FILES_ENDPOINT, id));

                  m_gd.get(url)
                      .then(this,
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

                                promise->addResult(FileDataResult::fromNetworkReply(reply, this));
                                promise->finish();
                            })
                      .onCanceled(this,
                                  [errorCallback]() mutable { errorCallback(u"Could not get GoogleDrive ID"_s); });
              })
        .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not get GoogleDrive ID"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<FileDataResult *>>
{
    qCDebug(gmFileAccessGoogle()) << "getDataAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiGetDataAsync(new MultiGetHelper<FileDataResult>(paths, this), allowCache);
}

auto FileAccessGoogleDrive::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "saveAsync(" << path << ")";

    auto promise = QSharedPointer<QPromise<FileResult *>>::create();
    promise->start();

    auto errorCallback = [promise]() mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not save file content!";
        promise->future().cancel();
    };

    auto updateExistingFile = [this, path, data, promise, errorCallback](const QString &id) mutable {
        updateFileContent(id, path, data)
            .then(this,
                  [promise](FileResult *result) mutable {
                      promise->addResult(result);
                      promise->finish();
                  })
            .onCanceled(this, errorCallback);
    };

    auto createNewFile = [this, path, updateExistingFile, promise, errorCallback]() mutable {
        // Create file first
        createFileAsync(path, QByteArray())
            .then(this,
                  [this, path, updateExistingFile, errorCallback](FileResult *) mutable {
                      // Get file id and then update the file with data
                      getFileIdAsync(path).then(this, updateExistingFile).onCanceled(this, errorCallback);
                  })
            .onCanceled(this, errorCallback);
    };

    getFileIdAsync(path).then(this, updateExistingFile).onCanceled(this, createNewFile);

    return promise->future();
}

auto FileAccessGoogleDrive::updateFileContent(const QString &id, const QString &path, const QByteArray &data)
    -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "updateFile(" << path << ")";

    const auto url = QUrl(u"%1/%2?uploadType=resumable"_s.arg(UPLOAD_ENDPOINT, id));
    const auto request = makeJsonRequest(url);

    const auto errorCallback = []() {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not update file!";
        return QFuture<FileResult *>();
    };

    // Tell GoogleDrive that we want to upload a file
    return m_gd.customRequest(request, "PATCH", "")
        .then(this,
              [this, path, data, errorCallback](RestNetworkReply *reply) {
                  if (reply->error() != QNetworkReply::NoError)
                  {
                      qCWarning(gmFileAccessGoogle()) << "Error during file upload:" << reply->errorText();
                      return QtFuture::makeReadyFuture(new FileResult(false, reply->errorText(), this));
                  }

                  auto uploadUrl = reply->getHeader("location");

                  if (uploadUrl.isEmpty()) uploadUrl = reply->getHeader("Location");

                  reply->deleteLater();

                  if (uploadUrl.isEmpty())
                  {
                      qCWarning(gmFileAccessGoogle()) << "Did not receive an upload URL for file" << path;
                      return QFuture<FileResult *>();
                  }

                  qCDebug(gmFileAccessGoogle()) << "Received upload URL for file" << path;

                  auto request = QNetworkRequest(QUrl(QString::fromUtf8(uploadUrl)));
                  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
                  request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

                  return m_gd.put(request, data)
                      .then(this,
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

                                return QtFuture::makeReadyFuture(FileResult::fromNetworkReply(reply, this));
                            })
                      .onCanceled(this, errorCallback)
                      .unwrap();
              })
        .onCanceled(this, errorCallback)
        .unwrap();
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

    auto promise = QSharedPointer<QPromise<FileResult *>>::create();
    promise->start();

    auto errorCallback = [this, promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << "Error during moveAsync():" << error;
        promise->addResult(new FileResult(error, this));
        promise->finish();
    };

    auto createNewParentFolderAndTryAgain = [this, oldPath, newPath, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(newPath);

        createDirAsync(parentPath)
            .then(this,
                  [this, oldPath, newPath, promise](FileResult *) mutable {
                      moveAsync(oldPath, newPath).then(this, [promise](FileResult *result) {
                          promise->addResult(result);
                          promise->finish();
                      });
                  })
            .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not create new parent folder"_s); });
    };

    getFileIdAsync(oldPath)
        .then(this,
              [this, oldPath, newPath, promise, errorCallback,
               createNewParentFolderAndTryAgain](const QString &fileId) mutable {
                  getParentIdAsync(oldPath)
                      .then(this,
                            [this, fileId, oldPath, newPath, promise, errorCallback,
                             createNewParentFolderAndTryAgain](const std::pair<QString, QByteArray> &oldPair) mutable {
                                getParentIdAsync(newPath)
                                    .then(
                                        this,
                                        [this, fileId, oldPath, newPath, oldPair, promise, errorCallback,
                                         createNewParentFolderAndTryAgain](
                                            const std::pair<QString, QByteArray> &newPair) mutable {
                                            const auto filename = FileUtils::fileName(newPath);
                                            const auto metadata = makeMetaData(filename);

                                            const auto url = QUrl(u"%1/%2?addParents=%3&removeParents=%4"_s.arg(
                                                FILES_ENDPOINT, fileId, newPair.second, oldPair.second));
                                            const auto request = makeJsonRequest(url);

                                            m_gd.customRequest(request, "PATCH", metadata)
                                                .then(
                                                    this,
                                                    [this, oldPath, newPath, promise](RestNetworkReply *reply) mutable {
                                                        qCDebug(gmFileAccessGoogle())
                                                            << "Successfully moved file/folder from" << oldPath << "to"
                                                            << newPath;

                                                        m_fileCache.moveEntry(oldPath, newPath);
                                                        m_idCache.moveEntry(oldPath, newPath);

                                                        promise->addResult(FileResult::fromNetworkReply(reply, this));
                                                        promise->finish();
                                                    })
                                                .onCanceled(this, [errorCallback]() mutable {
                                                    errorCallback(u"Could not send PATCH request"_s);
                                                });
                                        })
                                    .onCanceled(this, createNewParentFolderAndTryAgain);
                            })
                      .onCanceled(this, [errorCallback]() mutable {
                          errorCallback(u"Could not find ID of old parent folder"_s);
                      });
              })
        .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not find file/folder ID"_s); });

    return promise->future();
}

/// Delete a file/folder
/// 1. Get file/folder ID
/// 2. Send DELETE request
auto FileAccessGoogleDrive::deleteAsync(const QString &path) -> QFuture<FileResult *>
{
    qCDebug(gmFileAccessGoogle()) << "deleteAsync(" << path << ")";

    auto promise = QSharedPointer<QPromise<FileResult *>>::create();
    promise->start();

    auto errorCallback = [this, path, promise]() mutable {
        qCWarning(gmFileAccessGoogle()) << "Error: Could not delete file" << path << "!";
        promise->addResult(new FileResult(false, this));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(this,
              [this, path, promise, errorCallback](const QString &id) mutable {
                  const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, id));
                  const auto request = QNetworkRequest(url);

                  m_gd.customRequest(request, "DELETE", "")
                      .then(this,
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

                                promise->addResult(FileResult::fromNetworkReply(reply, this));
                                promise->finish();
                            })
                      .onCanceled(errorCallback);
              })
        .onCanceled(this, errorCallback);

    return promise->future();
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

    auto promise = QSharedPointer<QPromise<FileResult *>>::create();
    promise->start();

    auto errorCallback = [this, promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << error;
        promise->addResult(new FileResult(false, error, this));
        promise->finish();
    };

    auto createNewParentFolderAndTryAgain = [this, path, copy, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(copy);

        createDirAsync(parentPath)
            .then(this,
                  [this, path, copy, promise](FileResult *) mutable {
                      copyAsync(path, copy).then(this, [promise](FileResult *result) {
                          promise->addResult(result);
                          promise->finish();
                      });
                  })
            .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not create new parent folder"_s); });
    };

    checkAsync(copy, true)
        .then(this,
              [this, path, copy, promise, errorCallback,
               createNewParentFolderAndTryAgain](FileCheckResult *result) mutable {
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

                  result->deleteLater();

                  getFileIdAsync(path)
                      .then(this,
                            [this, path, copy, promise, errorCallback,
                             createNewParentFolderAndTryAgain](const QString &id) mutable {
                                getParentIdAsync(copy)
                                    .then(this,
                                          [this, path, copy, id, promise,
                                           errorCallback](const std::pair<QString, QByteArray> &parent) mutable {
                                              const auto filename = FileUtils::fileName(copy);
                                              const auto metadata = makeMetaData(filename, parent.second);

                                              const auto url = QUrl(u"%1/%2/copy"_s.arg(FILES_ENDPOINT, id));
                                              const auto request = makeJsonRequest(url);

                                              m_gd.post(request, metadata)
                                                  .then(
                                                      this,
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
                                                                  << "Successfully copied file" << path << "to" << copy;
                                                          }
                                                          else
                                                          {
                                                              qCWarning(gmFileAccessGoogle())
                                                                  << "Error during copying of file" << path << ":"
                                                                  << reply->errorText();
                                                          }

                                                          promise->addResult(FileResult::fromNetworkReply(reply, this));
                                                          promise->finish();
                                                      })
                                                  .onCanceled(this, [errorCallback]() mutable {
                                                      errorCallback(u"Could not send copy request"_s);
                                                  });
                                          })
                                    .onCanceled(this, createNewParentFolderAndTryAgain);
                            })
                      .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not get file ID"_s); });
              })
        .onCanceled(this,
                    [errorCallback]() mutable { errorCallback(u"Could not check if destination already exists!"_s); });

    return promise->future();
}

/// List files and/or folders in a directory
auto FileAccessGoogleDrive::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult *>
{
    qCDebug(gmFileAccessGoogle()) << "listAsync(" << path << "," << files << "," << folders << ")";

    auto promise = QSharedPointer<QPromise<FileListResult *>>::create();
    promise->start();

    auto errorCallback = [this, path, promise](const QString &error) mutable {
        promise->addResult(new FileListResult(path, error, this));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(this,
              [this, path, files, folders, promise](const QString &id) mutable {
                  const auto q = makeListQuery(id, files, folders);
                  listAsync(path, q).then(this, [promise](FileListResult *result) {
                      promise->addResult(result);
                      promise->finish();
                  });
              })
        .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not get folder id"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::listAsync(const QString &path, const QString &q, const QString &pageToken)
    -> QFuture<FileListResult *>
{
    QUrl url(FILES_ENDPOINT);
    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(q)}});

    if (!pageToken.isEmpty())
    {
        query.addQueryItem(u"pageToken"_s, pageToken);
    }

    url.setQuery(query);

    return m_gd.get(makeJsonRequest(url))
        .then(this,
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
                              .then(this,
                                    [this, path, files, folders](FileListResult *result) mutable {
                                        files.append(result->files());
                                        folders.append(result->folders());
                                        result->deleteLater();

                                        return QtFuture::makeReadyFuture(
                                            new FileListResult(path, folders, files, this));
                                    })
                              .onCanceled(this, []() { return QFuture<FileListResult *>(); })
                              .unwrap();
                      }

                      return QtFuture::makeReadyFuture(new FileListResult(path, folders, files, this));
                  }

                  const auto error = reply->errorText();
                  reply->deleteLater();

                  qCWarning(gmFileAccessGoogle()) << error;
                  return QtFuture::makeReadyFuture(new FileListResult(path, error, this));
              })
        .onCanceled(this, []() { return QFuture<FileListResult *>(); })
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

    auto promise = QSharedPointer<QPromise<FileResult *>>::create();
    promise->start();

    auto errorCallback = [this, promise](const QString &error) mutable {
        qCWarning(gmFileAccessGoogle()) << error;
        promise->addResult(new FileResult(false, error, this));
        promise->finish();
    };

    auto createParentDirAndTryAgain = [this, path, mimeType, promise, errorCallback]() mutable {
        const auto parentPath = FileUtils::dirFromPath(path);
        return createDirAsync(parentPath)
            .then(this,
                  [this, path, mimeType, promise](FileResult *) mutable {
                      createFileAsync(path, mimeType).then(this, [promise](FileResult *result) {
                          promise->addResult(result);
                          promise->finish();
                      });
                  })
            .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not create parent dir!"_s); });
    };

    checkAsync(path, true)
        .then(this,
              [this, path, mimeType, promise, errorCallback,
               createParentDirAndTryAgain](FileCheckResult *result) mutable {
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

                  result->deleteLater();

                  // Get parent folder ID first
                  getParentIdAsync(path)
                      .then(this,
                            [this, path, mimeType, promise,
                             errorCallback](const std::pair<QString, QByteArray> &parent) mutable {
                                // Build file meta data
                                const auto data = makeMetaData(FileUtils::fileName(path), parent.second, mimeType);
                                const auto request = makeJsonRequest(QUrl(FILES_ENDPOINT));

                                // Send "create" request
                                return m_gd.post(request, data)
                                    .then(this,
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

                                              promise->addResult(FileResult::fromNetworkReply(reply, this));
                                              promise->finish();
                                          })
                                    .onCanceled(this, [errorCallback]() mutable {
                                        errorCallback(u"Could not send POST request!"_s);
                                    });
                            })
                      .onCanceled(this, createParentDirAndTryAgain);
              })
        .onCanceled(this, [errorCallback]() mutable { errorCallback(u"Could not check if file already exists!"_s); });

    return promise->future();
}

auto FileAccessGoogleDrive::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult *>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << path << allowCache << ")";

    if (allowCache && m_fileCache.checkEntry(path))
    {
        m_fileCache.printEntries();

        // File is still fresh in cache, so we are confident that it exists
        return QtFuture::makeReadyFuture(new FileCheckResult(path, true, this));
    }

    auto promise = QSharedPointer<QPromise<FileCheckResult *>>::create();
    promise->start();

    auto errorCallback = [this, path, promise]() mutable {
        promise->addResult(new FileCheckResult(path, false, this));
        promise->finish();
    };

    getFileIdAsync(path)
        .then(this,
              [this, path, promise](const QString &folderId) mutable {
                  const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, folderId));

                  m_gd.get(url)
                      .then(this,
                            [this, path, promise](RestNetworkReply *reply) mutable {
                                const auto json = QJsonDocument::fromJson(reply->data()).object();
                                const auto id = json["id"_L1].toString();
                                m_idCache.createOrUpdateEntry(path, id.toUtf8());

                                promise->addResult(FileCheckResult::fromNetworkReply(reply, path, this));
                                promise->finish();
                            })
                      .onCanceled(this, [this, path, promise]() mutable {
                          promise->addResult(new FileCheckResult(path, false, this));
                          promise->finish();
                      });
              })
        .onCanceled(this, errorCallback);

    return promise->future();
}

auto FileAccessGoogleDrive::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult *>
{
    qCDebug(gmFileAccessGoogle()) << "checkAsync(" << paths << "," << allowCache << ")";
    return FileAccess::multiCheckAsync(new MultiGetHelper<FileCheckResult>(paths, this), allowCache);
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

    return m_gd.get(url).then(this, callback).onCanceled(this, errorCallback).unwrap();
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
        .then(this,
              [this, path, callback, errorCallback](const std::pair<QString, QByteArray> &parentPair) {
                  // Has ID been found in the meantime?
                  if (QByteArray id; m_idCache.tryGetData(path, id))
                  {
                      return QtFuture::makeReadyFuture(QString(id));
                  }

                  return getFolderEntryIds(parentPair).then(this, callback).onCanceled(this, errorCallback).unwrap();
              })
        .onCanceled(this, errorCallback)
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

    return getFileIdAsync(parentPath).then(this, [parentPath](const QString &id) {
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

/// Construct a network request with content-type application/json
auto FileAccessGoogleDrive::makeJsonRequest(const QUrl &url) -> QNetworkRequest
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, static_cast<const char *>(O2_MIME_TYPE_JSON));
    return request;
}
