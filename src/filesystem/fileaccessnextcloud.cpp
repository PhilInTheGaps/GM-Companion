#include "fileaccessnextcloud.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QQueue>
#include <QXmlStreamReader>

using namespace Files;
using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmFileAccessNextCloud, "gm.files.access.nextcloud")

FileAccessNextcloud::FileAccessNextcloud(NextCloud &nextcloud) : m_nc(nextcloud)
{
}

auto FileAccessNextcloud::getDataAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileDataResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting data from file" << path;

    // Try to get from cache first
    if (allowCache)
    {
        QByteArray data;
        if (m_cache.tryGetData(path, data))
        {
            return QtFuture::makeReadyFuture(std::make_shared<FileDataResult>(data));
        }
    }

    // Fetch from web
    auto future = m_nc.sendDavRequest("GET", encodePath(path), "");

    return future
        .then(context(),
              [this, future, path](QNetworkReply *reply) {
                  const auto callback = [path, reply]() -> std::shared_ptr<FileDataResult> {
                      if (replyHasError(reply))
                      {
                          const auto errorMessage = makeAndPrintError(u"Could not get data from %1"_s.arg(path), reply);
                          return deleteReplyAndReturn(std::make_shared<FileDataResult>(errorMessage), reply);
                      }

                      qCDebug(gmFileAccessNextCloud()) << "Received data from file" << path;
                      return deleteReplyAndReturn(std::make_shared<FileDataResult>(reply->readAll()), reply);
                  };

                  return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), callback);
              })
        .unwrap();
}

auto FileAccessNextcloud::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>
{
    return FileAccess::multiGetDataAsync(std::make_shared<MultiGetHelper<FileDataResult>>(paths), allowCache);
}

template <typename T1, typename T2>
auto FileAccessNextcloud::createDirThenContinue(
    const QString &dir, const T1 &arg1, const T2 &arg2,
    const std::function<QFuture<std::shared_ptr<FileResult>>(const T1 &, const T2 &)> &func)
    -> QFuture<std::shared_ptr<FileResult>>
{
    return createDirAsync(dir)
        .then(context(),
              [arg1, arg2, func](std::shared_ptr<FileResult> result) {
                  if (!result->success()) return QtFuture::makeReadyFuture(result);
                  return func(arg1, arg2);
              })
        .unwrap();
}

auto FileAccessNextcloud::saveAsync(const QString &path, const QByteArray &data) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Saving file" << path;

    auto future = m_nc.sendDavRequest("PUT", encodePath(path), data);

    return future
        .then(
            context(),
            [this, future, path, data](QNetworkReply *reply) {
                return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), [this, path, data, reply]() {
                    if (replyHasError(reply))
                    {
                        const auto isDirMissing = reply->error() == QNetworkReply::ContentNotFoundError;
                        if (isDirMissing)
                        {
                            return deleteReplyAndReturn(createDirThenContinue<QString, QByteArray>(
                                                            FileUtils::dirFromPath(path), path, data,
                                                            [this](const QString &path, const QByteArray &data) {
                                                                return saveAsync(path, data);
                                                            }),
                                                        reply);
                        }

                        const auto errorMessage = makeAndPrintError(u"Could not save file %1"_s.arg(path), reply);
                        return deleteReplyAndReturn(
                            QtFuture::makeReadyFuture(std::make_shared<FileResult>(errorMessage)), reply);
                    }

                    if (!m_cache.createOrUpdateEntry(path, data))
                    {
                        qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << path << "in cache";
                    }

                    qCDebug(gmFileAccessNextCloud()) << "Successfully saved file" << path;
                    return deleteReplyAndReturn(QtFuture::makeReadyFuture(std::make_shared<FileResult>(true)), reply);
                });
            })
        .unwrap();
}

auto FileAccessNextcloud::moveAsync(const QString &oldPath, const QString &newPath)
    -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Moving file" << oldPath << "to" << newPath;

    auto future = m_nc.sendDavRequest("MOVE", encodePath(oldPath), QByteArray(), makeMoveHeaders(newPath));

    return future
        .then(context(),
              [this, future, oldPath, newPath](QNetworkReply *reply) {
                  return QtFuture::connect(reply, &QNetworkReply::finished)
                      .then(context(), [this, oldPath, newPath, reply]() {
                          qCDebug(gmFileAccessNextCloud()) << "MOVE Reply:" << reply->error() << reply->errorString();

                          if (replyHasError(reply))
                          {
                              const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError;
                              if (isDirMissing)
                              {
                                  return deleteReplyAndReturn(
                                      createDirThenContinue<QString, QString>(
                                          FileUtils::dirFromPath(newPath), oldPath, newPath,
                                          [this](const QString &oldPath, const QString &newPath) {
                                              return moveAsync(oldPath, newPath);
                                          }),
                                      reply);
                              }

                              const auto errorMessage =
                                  makeAndPrintError(u"Could not move file %1 to %2"_s.arg(oldPath, newPath), reply);
                              return deleteReplyAndReturn(
                                  QtFuture::makeReadyFuture(std::make_shared<FileResult>(errorMessage)), reply);
                          }

                          if (!m_cache.moveEntry(oldPath, newPath))
                          {
                              qCWarning(gmFileAccessNextCloud())
                                  << "Error: Could not update file" << oldPath << "in cache";
                          }

                          qCDebug(gmFileAccessNextCloud()) << "Successfully moved file" << oldPath << "to" << newPath;
                          return deleteReplyAndReturn(QtFuture::makeReadyFuture(std::make_shared<FileResult>(true)),
                                                      reply);
                      });
              })
        .unwrap();
}

auto FileAccessNextcloud::deleteAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Deleting file" << path << "...";

    auto future = m_nc.sendDavRequest("DELETE", encodePath(path), QByteArray());

    return future
        .then(context(),
              [this, future, path](QNetworkReply *reply) {
                  return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), [this, path, reply]() {
                      if (replyHasError(reply))
                      {
                          const auto errorMessage =
                              makeAndPrintError(u"Could not delete file/folder %1"_s.arg(path), reply);
                          return deleteReplyAndReturn(
                              QtFuture::makeReadyFuture(std::make_shared<FileResult>(errorMessage)), reply);
                      }

                      m_cache.removeEntry(path);

                      qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
                      return deleteReplyAndReturn(QtFuture::makeReadyFuture(std::make_shared<FileResult>(true)), reply);
                  });
              })
        .unwrap();
}

auto FileAccessNextcloud::copyAsync(const QString &path, const QString &copy) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Copying file" << path << "to" << copy;

    const auto destinationHeader = std::pair(QByteArray("Destination"), copy.toUtf8());
    const auto overwriteHeader = std::pair(QByteArray("Overwrite"), QByteArray("F"));

    auto future = m_nc.sendDavRequest("COPY", encodePath(path), QByteArray(), {destinationHeader, overwriteHeader});

    return future
        .then(
            context(),
            [this, future, path, copy](QNetworkReply *reply) {
                return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), [this, path, copy, reply]() {
                    if (replyHasError(reply))
                    {
                        const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError;
                        if (isDirMissing)
                        {
                            return deleteReplyAndReturn(
                                createDirThenContinue<QString, QString>(
                                    FileUtils::dirFromPath(copy), path, copy,
                                    [this](const QString &path, const QString &copy) { return copyAsync(path, copy); }),
                                reply);
                        }

                        const auto errorMessage =
                            makeAndPrintError(u"Could not copy %1 to %2: %3 %4"_s.arg(path), reply);
                        return deleteReplyAndReturn(
                            QtFuture::makeReadyFuture(std::make_shared<FileResult>(errorMessage)), reply);
                    }

                    if (!m_cache.copyEntry(path, copy))
                    {
                        qCDebug(gmFileAccessNextCloud()) << "Could not copy file" << path << "to" << copy
                                                         << "in cache. (local version of file is probably too old)";
                    }

                    qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
                    return deleteReplyAndReturn(QtFuture::makeReadyFuture(std::make_shared<FileResult>(true)), reply);
                });
            })
        .unwrap();
}

auto FileAccessNextcloud::listAsync(const QString &path, bool files, bool folders)
    -> QFuture<std::shared_ptr<FileListResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting list of"
                                     << (folders ? (files ? "files and folders" : "folders") : "files") << "in path"
                                     << path << "...";

    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), QByteArray());

    return future
        .then(context(),
              [this, future, path, files, folders](QNetworkReply *reply) {
                  return QtFuture::connect(reply, &QNetworkReply::finished)
                      .then(context(), [this, path, files, folders, reply]() {
                          if (replyHasError(reply))
                          {
                              const auto errorMessage =
                                  makeAndPrintError(u"Could not list content of folder %1"_s.arg(path), reply);
                              return deleteReplyAndReturn(std::make_shared<FileListResult>(path, errorMessage), reply);
                          }

                          qCDebug(gmFileAccessNextCloud()) << "Successfully received content of" << path;
                          return deleteReplyAndReturn(parseListResponse(reply->readAll(), path, files, folders), reply);
                      });
              })
        .unwrap();
}

auto FileAccessNextcloud::parseListResponse(const QByteArray &data, const QString &path, bool files, bool folders)
    -> std::shared_ptr<FileListResult>
{
    QXmlStreamReader xml(data);
    QStringList fileList;
    QStringList folderList;
    QString element;
    bool foundFirstFolder = false;

    qCDebug(gmFileAccessNextCloud()) << "PROPFIND:" << data;

    while (!xml.atEnd())
    {
        const auto tokenType = xml.readNext();

        if (tokenType == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "href"_L1)
            {
                const auto rawElement = xml.readElementText();
                const auto decoded = QByteArray::fromPercentEncoding(rawElement.toUtf8());
                element = FileUtils::fileName(decoded);
            }
            else if (folders && xml.name() == "collection"_L1)
            {
                if (!element.isEmpty())
                {
                    if (foundFirstFolder)
                    {
                        folderList << element;
                    }
                    else
                    {
                        foundFirstFolder = true;
                    }
                }
            }
            else if (files && xml.name() == "getcontenttype"_L1 &&
                     !xml.readElementText(QXmlStreamReader::SkipChildElements).isEmpty())
            {
                if (!element.isEmpty()) fileList << element;
            }
        }
    }

    if (xml.hasError())
    {
        qCWarning(gmFileAccessNextCloud())
            << "Error during parsing of PROPFIND response:" << xml.error() << xml.errorString();
        return std::make_shared<FileListResult>(path, xml.errorString());
    }

    return std::make_shared<FileListResult>(path, folderList, fileList);
}

auto FileAccessNextcloud::createDirAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Creating dir" << path << "...";

    auto future = m_nc.sendDavRequest("MKCOL", encodePath(path), {});

    return future
        .then(context(),
              [this, future, path](QNetworkReply *reply) {
                  return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), [path, reply]() {
                      if (replyHasError(reply))
                      {
                          const auto errorMessage =
                              makeAndPrintError(u"Could not create directory %1"_s.arg(path), reply);
                          return deleteReplyAndReturn(std::make_shared<FileResult>(errorMessage), reply);
                      }

                      qCDebug(gmFileAccessNextCloud()) << "Successfully created directory" << path;
                      return deleteReplyAndReturn(std::make_shared<FileResult>(true), reply);
                  });
              })
        .unwrap();
}

auto FileAccessNextcloud::checkAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileCheckResult>>
{
    qCDebug(gmFileAccessNextCloud()) << "Checking if file" << path << "exists ...";

    // Check cache first
    if (allowCache && m_cache.checkEntry(path))
    {
        return QtFuture::makeReadyFuture(std::make_shared<FileCheckResult>(path, true));
    }

    // If file is not in cache or cache is outdated, fetch from web
    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), QByteArray());

    return future
        .then(context(),
              [this, future, path](QNetworkReply *reply) {
                  return QtFuture::connect(reply, &QNetworkReply::finished).then(context(), [path, reply]() {
                      const auto doesExist = reply->error() != QNetworkReply::ContentNotFoundError;
                      const auto hasError = replyHasError(reply) && doesExist;

                      if (hasError)
                      {
                          const auto errorMessage =
                              makeAndPrintError(u"Could not check if file %1 exists"_s.arg(path), reply);
                          return deleteReplyAndReturn(std::make_shared<FileCheckResult>(path, errorMessage), reply);
                      }

                      qCDebug(gmFileAccessNextCloud()) << "PROPFIND:" << reply->readAll();

                      qCDebug(gmFileAccessNextCloud())
                          << "Successfully checked if file" << path << "exists:" << doesExist;
                      return deleteReplyAndReturn(std::make_shared<FileCheckResult>(path, doesExist), reply);
                  });
              })
        .unwrap();
}

auto FileAccessNextcloud::checkAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::shared_ptr<FileMultiCheckResult>>
{
    return FileAccess::multiCheckAsync(std::make_shared<MultiGetHelper<FileCheckResult>>(paths), allowCache);
}

auto FileAccessNextcloud::encodePath(const QString &data) -> QByteArray
{
    return QUrl::toPercentEncoding(data, "/");
}

auto FileAccessNextcloud::replyHasError(QNetworkReply *reply) -> bool
{
    return reply->error() != QNetworkReply::NoError;
}

auto FileAccessNextcloud::makeAndPrintError(const QString &errorMessage, const QNetworkReply *reply) -> QString
{
    auto result = u"%1: %2"_s.arg(errorMessage, replyErrorToString(reply));
    qCWarning(gmFileAccessNextCloud()) << "Warning:" << result;
    return result;
}

auto FileAccessNextcloud::replyErrorToString(const QNetworkReply *reply) -> QString
{
    return u"%1 (%2)"_s.arg(QString::number(reply->error()), reply->errorString());
}

auto FileAccessNextcloud::makeMoveHeaders(const QString &newPath) -> QList<std::pair<QByteArray, QByteArray>>
{
    auto destinationHeader = std::pair(QByteArray("Destination"), newPath.toUtf8());
    auto overwriteHeader = std::pair(QByteArray("Overwrite"), QByteArray("F"));
    return {destinationHeader, overwriteHeader};
}

template <typename T> auto FileAccessNextcloud::deleteReplyAndReturn(T value, QNetworkReply *reply) -> T
{
    QScopedPointer scopedReply(reply);
    return value;
}
