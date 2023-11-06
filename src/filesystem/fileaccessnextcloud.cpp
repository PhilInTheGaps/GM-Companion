#include "fileaccessnextcloud.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QQueue>
#include <QXmlStreamReader>

using namespace Files;
using namespace Services;
using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmFileAccessNextCloud, "gm.files.access.nextcloud")

FileAccessNextcloud::FileAccessNextcloud(NextCloud &nextcloud) : m_nc(nextcloud)
{
}

auto FileAccessNextcloud::getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting data from file" << path;

    // Try to get from cache first
    if (allowCache)
    {
        QByteArray data;
        if (m_cache.tryGetData(path, data))
        {
            return QtFuture::makeReadyFuture(FileDataResult(std::move(data)));
        }
    }

    // Fetch from web
    auto future = m_nc.sendDavRequest("GET", encodePath(path), "");

    return future.then([future, path](QNetworkReply *reply) {
        if (replyHasError(reply))
        {
            auto errorMessage = makeAndPrintError(u"Could not get data from %1"_s.arg(path), reply);
            return deleteReplyAndReturn(FileDataResult(std::move(errorMessage)), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Received data from file" << path;
        return deleteReplyAndReturn(FileDataResult(reply->readAll()), reply);
    });
}

auto FileAccessNextcloud::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<FileDataResult>>
{
    return FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult>(paths), allowCache);
}

template <typename T1, typename T2>
auto FileAccessNextcloud::createDirThenContinue(const QString &dir, const T1 &arg1, const T2 &arg2,
                                                const std::function<QFuture<FileResult>(const T1 &, const T2 &)> &func)
    -> QFuture<FileResult>
{
    return createDirAsync(dir)
        .then([arg1, arg2, func](FileResult &&result) {
            if (!result.success()) return QtFuture::makeReadyFuture(std::move(result));
            return func(arg1, arg2);
        })
        .unwrap();
}

auto FileAccessNextcloud::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Saving file" << path;

    auto future = m_nc.sendDavRequest("PUT", encodePath(path), data);

    return future
        .then([this, future, path, data](QNetworkReply *reply) {
            if (replyHasError(reply))
            {
                if (const auto isDirMissing = reply->error() == QNetworkReply::ContentNotFoundError; isDirMissing)
                {
                    return deleteReplyAndReturn(
                        createDirThenContinue<QString, QByteArray>(
                            FileUtils::dirFromPath(path), path, data,
                            [this](const QString &path, const QByteArray &data) { return saveAsync(path, data); }),
                        reply);
                }

                auto errorMessage = makeAndPrintError(u"Could not save file %1"_s.arg(path), reply);
                return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(std::move(errorMessage))), reply);
            }

            if (!m_cache.createOrUpdateEntry(path, data))
            {
                qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << path << "in cache";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully saved file" << path;
            return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(true)), reply);
        })
        .unwrap();
}

auto FileAccessNextcloud::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Moving file" << oldPath << "to" << newPath;

    auto future = m_nc.sendDavRequest("MOVE", encodePath(oldPath), QByteArray(), makeMoveHeaders(newPath));

    return future
        .then([this, future, oldPath, newPath](QNetworkReply *reply) {
            qCDebug(gmFileAccessNextCloud()) << "MOVE Reply:" << reply->error() << reply->errorString();

            if (replyHasError(reply))
            {
                if (const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError; isDirMissing)
                {
                    return deleteReplyAndReturn(
                        createDirThenContinue<QString, QString>(FileUtils::dirFromPath(newPath), oldPath, newPath,
                                                                [this](const QString &oldPath, const QString &newPath) {
                                                                    return moveAsync(oldPath, newPath);
                                                                }),
                        reply);
                }

                auto errorMessage = makeAndPrintError(u"Could not move file %1 to %2"_s.arg(oldPath, newPath), reply);
                return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(std::move(errorMessage))), reply);
            }

            if (!m_cache.moveEntry(oldPath, newPath))
            {
                qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << oldPath << "in cache";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully moved file" << oldPath << "to" << newPath;
            return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(true)), reply);
        })
        .unwrap();
}

auto FileAccessNextcloud::deleteAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Deleting file" << path << "...";

    auto future = m_nc.sendDavRequest("DELETE", encodePath(path), QByteArray());

    return future
        .then([this, future, path](QNetworkReply *reply) {
            if (replyHasError(reply))
            {
                auto errorMessage = makeAndPrintError(u"Could not delete file/folder %1"_s.arg(path), reply);
                return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(std::move(errorMessage))), reply);
            }

            m_cache.removeEntry(path);

            qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
            return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(true)), reply);
        })
        .unwrap();
}

auto FileAccessNextcloud::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Copying file" << path << "to" << copy;

    const auto destinationHeader = std::make_pair(QByteArray("Destination"), copy.toUtf8());
    const auto overwriteHeader = std::make_pair(QByteArray("Overwrite"), QByteArray("F"));

    auto future = m_nc.sendDavRequest("COPY", encodePath(path), QByteArray(), {destinationHeader, overwriteHeader});

    return future
        .then([this, future, path, copy](QNetworkReply *reply) {
            if (replyHasError(reply))
            {
                if (const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError; isDirMissing)
                {
                    return deleteReplyAndReturn(
                        createDirThenContinue<QString, QString>(
                            FileUtils::dirFromPath(copy), path, copy,
                            [this](const QString &path, const QString &copy) { return copyAsync(path, copy); }),
                        reply);
                }

                auto errorMessage = makeAndPrintError(u"Could not copy %1 to %2: %3 %4"_s.arg(path), reply);
                return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(std::move(errorMessage))), reply);
            }

            if (!m_cache.copyEntry(path, copy))
            {
                qCDebug(gmFileAccessNextCloud()) << "Could not copy file" << path << "to" << copy
                                                 << "in cache. (local version of file is probably too old)";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
            return deleteReplyAndReturn(QtFuture::makeReadyFuture(FileResult(true)), reply);
        })
        .unwrap();
}

auto FileAccessNextcloud::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting list of"
                                     << (folders ? (files ? "files and folders" : "folders") : "files") << "in path"
                                     << path << "...";

    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), QByteArray());

    return future.then([future, path, files, folders](QNetworkReply *reply) {
        if (replyHasError(reply))
        {
            auto errorMessage = makeAndPrintError(u"Could not list content of folder %1"_s.arg(path), reply);
            return deleteReplyAndReturn(FileListResult(path, std::move(errorMessage)), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully received content of" << path;
        return deleteReplyAndReturn(parseListResponse(reply->readAll(), path, files, folders), reply);
    });
}

auto FileAccessNextcloud::parseListResponse(const QByteArray &data, const QString &path, bool files, bool folders)
    -> FileListResult
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
                     !xml.readElementText(QXmlStreamReader::SkipChildElements).isEmpty() && !element.isEmpty())
            {
                fileList << element;
            }
        }
    }

    if (xml.hasError())
    {
        qCWarning(gmFileAccessNextCloud())
            << "Error during parsing of PROPFIND response:" << xml.error() << xml.errorString();
        return FileListResult(path, xml.errorString());
    }

    return FileListResult(path, folderList, fileList);
}

auto FileAccessNextcloud::createDirAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Creating dir" << path << "...";

    auto future = m_nc.sendDavRequest("MKCOL", encodePath(path), {});

    return future.then([future, path](QNetworkReply *reply) {
        if (replyHasError(reply))
        {
            auto errorMessage = makeAndPrintError(u"Could not create directory %1"_s.arg(path), reply);
            return deleteReplyAndReturn(FileResult(std::move(errorMessage)), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully created directory" << path;
        return deleteReplyAndReturn(FileResult(true), reply);
    });
}

auto FileAccessNextcloud::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Checking if file" << path << "exists ...";

    // Check cache first
    if (allowCache && m_cache.checkEntry(path))
    {
        return QtFuture::makeReadyFuture(FileCheckResult(path, true));
    }

    // If file is not in cache or cache is outdated, fetch from web
    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), QByteArray());

    return future.then([future, path](QNetworkReply *reply) {
        const auto doesExist = reply->error() != QNetworkReply::ContentNotFoundError;

        if (const auto hasError = replyHasError(reply) && doesExist; hasError)
        {
            auto errorMessage = makeAndPrintError(u"Could not check if file %1 exists"_s.arg(path), reply);
            return deleteReplyAndReturn(FileCheckResult(path, std::move(errorMessage)), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "PROPFIND:" << reply->readAll();

        qCDebug(gmFileAccessNextCloud()) << "Successfully checked if file" << path << "exists:" << doesExist;
        return deleteReplyAndReturn(FileCheckResult(path, doesExist), reply);
    });
}

auto FileAccessNextcloud::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult>
{
    return FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult>(paths), allowCache);
}

auto FileAccessNextcloud::encodePath(const QString &data) -> QByteArray
{
    return QUrl::toPercentEncoding(data, "/");
}

auto FileAccessNextcloud::replyHasError(const QNetworkReply *reply) -> bool
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
    auto destinationHeader = std::make_pair(QByteArray("Destination"), newPath.toUtf8());
    auto overwriteHeader = std::make_pair(QByteArray("Overwrite"), QByteArray("F"));
    return {destinationHeader, overwriteHeader};
}

template <typename T> auto FileAccessNextcloud::deleteReplyAndReturn(T &&value, QNetworkReply *reply) -> T
{
    QScopedPointer scopedReply(reply);
    return std::move(value);
}
