#include "fileaccessnextcloud.h"
#include "logging.h"
#include "nextcloud/nextcloud.h"
#include "utils/fileutils.h"
#include <QQueue>
#include <QXmlStreamReader>
#include "thirdparty/asyncfuture/asyncfuture.h"

using namespace AsyncFuture;
using namespace Files;

auto FileAccessNextcloud::getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting data from file" << path;

    // Try to get from cache first
    if (allowCache)
    {
        QByteArray data;
        if (m_cache.tryGetData(path, data))
        {
            return completed(new FileDataResult(data, this));
        }
    }

    // Fetch from web
    auto *reply = NextCloud::getInstance()->sendDavRequest("GET", encodePath(path), "");
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, reply]() {
        if (replyHasError(reply))
        {
            const auto errorMessage = makeAndPrintError(QString("Could not get data from %1").arg(path), reply);
            return deleteReplyAndReturn(new FileDataResult(errorMessage, this), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Received data from file" << path;
        return deleteReplyAndReturn(new FileDataResult(reply->readAll(), this), reply);
    }).future();
}

auto FileAccessNextcloud::getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<QVector<FileDataResult*>>
{
    return getDataAsync(new MultiGetHelper<FileDataResult>(paths, this), allowCache);
}

auto FileAccessNextcloud::getDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache) -> QFuture<QVector<FileDataResult*>>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return completed(results);
    }

    auto next = getDataAsync(helper->getNextPath(), allowCache);
    return observe(next).subscribe([this, helper, allowCache](FileDataResult *result){
        helper->addResult(result);
        return getDataAsync(helper, allowCache);
    }).future();
}

template<typename T1, typename T2>
auto FileAccessNextcloud::createDirThenContinue(const QString& dir, const T1 &arg1, const T2 &arg2,
                                                const std::function<QFuture<FileResult*>(const T1&, const T2&)>& func)
                                                -> QFuture<FileResult*>
{
    return observe(createDirAsync(dir)).subscribe([arg1, arg2, func](FileResult *result) {
        if (!result->success()) return completed(result);

        result->deleteLater();
        return func(arg1, arg2);
    }).future();
}

auto FileAccessNextcloud::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Saving file" << path;

    auto *reply = NextCloud::getInstance()->sendDavRequest("PUT", encodePath(path), data);
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, data, reply]() {
        if (replyHasError(reply))
        {
            const auto isDirMissing = reply->error() == QNetworkReply::ContentNotFoundError;
            if (isDirMissing)
            {
                return deleteReplyAndReturn(createDirThenContinue<QString, QByteArray>(
                    FileUtils::dirFromPath(path), path, data,
                    [this](const QString& path, const QByteArray& data){
                        return saveAsync(path, data);
                }), reply);
            }

            const auto errorMessage = makeAndPrintError(QString("Could not save file %1").arg(path), reply);
            return deleteReplyAndReturn(completed(new FileResult(errorMessage, this)), reply);
        }

        if (!m_cache.createOrUpdateEntry(path, data))
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << path << "in cache";
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully saved file" << path;
        return deleteReplyAndReturn(completed(new FileResult(true, this)), reply);
    }).future();
}

auto FileAccessNextcloud::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Moving file" << oldPath << "to" << newPath;

    auto *reply = NextCloud::getInstance()->sendDavRequest("MOVE", encodePath(oldPath), QByteArray(), makeMoveHeaders(newPath));
    return observe(reply, &QNetworkReply::finished).context(this, [this, oldPath, newPath, reply]() {
        if (replyHasError(reply))
        {
            const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError;
            if (isDirMissing)
            {
                return deleteReplyAndReturn(createDirThenContinue<QString, QString>(
                    FileUtils::dirFromPath(newPath), oldPath, newPath,
                    [this](const QString& oldPath, const QString& newPath){
                        return moveAsync(oldPath, newPath);
                }), reply);
            }

            const auto errorMessage = makeAndPrintError(QString("Could not move file %1 to %2").arg(oldPath, newPath), reply);
            return deleteReplyAndReturn(completed(new FileResult(errorMessage, this)), reply);
        }

        if (!m_cache.moveEntry(oldPath, newPath))
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << oldPath << "in cache";
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully moved file" << oldPath << "to" << newPath;
        return deleteReplyAndReturn(completed(new FileResult(true, this)), reply);
    }).future();
}

auto FileAccessNextcloud::deleteAsync(const QString &path) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Deleting file" << path << "...";

    auto *reply = NextCloud::getInstance()->sendDavRequest("DELETE", encodePath(path), QByteArray());
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, reply]() {
        if (replyHasError(reply))
        {
            const auto errorMessage = makeAndPrintError(QString("Could not delete file/folder %1").arg(path), reply);
            return deleteReplyAndReturn(completed(new FileResult(errorMessage, this)), reply);
        }

        if (!m_cache.removeEntry(path))
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not remove file" << path << "from cache";
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
        return deleteReplyAndReturn(completed(new FileResult(true, this)), reply);
    }).future();
}

auto FileAccessNextcloud::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Copying file" << path << "to" << copy;

    auto destinationHeader = QPair(QByteArray("Destination"), copy.toUtf8());
    auto overwriteHeader  = QPair(QByteArray("Overwrite"), QByteArray("F"));

    auto *reply = NextCloud::getInstance()->sendDavRequest("COPY", encodePath(path), QByteArray(), {destinationHeader, overwriteHeader});

    return observe(reply, &QNetworkReply::finished).context(this, [this, path, copy, reply]() {
        if (replyHasError(reply))
        {
            const auto isDirMissing = reply->error() == QNetworkReply::ContentConflictError;
            if (isDirMissing)
            {
                return deleteReplyAndReturn(createDirThenContinue<QString, QString>(
                    FileUtils::dirFromPath(copy), path, copy,
                    [this](const QString& path, const QString& copy){
                        return copyAsync(path, copy);
                }), reply);
            }

            const auto errorMessage = makeAndPrintError(QString("Could not copy %1 to %2: %3 %4").arg(path), reply);
            return deleteReplyAndReturn(completed(new FileResult(errorMessage, this)), reply);
        }

        if (!m_cache.copyEntry(path, copy))
        {
            qCDebug(gmFileAccessNextCloud()) << "Could not copy file" << path << "to" << copy << "in cache. (local version of file is probably too old)";
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
        return deleteReplyAndReturn(completed(new FileResult(true, this)), reply);
    }).future();
}

auto FileAccessNextcloud::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting list of" << (folders ? (files ? "files and folders" : "folders") : "files") << "in path" << path << "...";

    auto *reply = NextCloud::getInstance()->sendDavRequest("PROPFIND", encodePath(path), QByteArray());
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, files, folders, reply]() {
        if (replyHasError(reply))
        {
            const auto errorMessage = makeAndPrintError(QString("Could not list content of folder %1").arg(path), reply);
            return deleteReplyAndReturn(new FileListResult(path, errorMessage, this), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully received content of" << path;
        return deleteReplyAndReturn(parseListResponse(reply->readAll(), path, files, folders), reply);
    }).future();
}

auto FileAccessNextcloud::parseListResponse(const QByteArray& data, const QString& path, bool files, bool folders) -> FileListResult*
{
    QXmlStreamReader xml(data);
    QStringList fileList;
    QStringList folderList;
    QString element;

    while (!xml.atEnd())
    {
        const auto tokenType = xml.readNext();

        if (tokenType == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "href")
            {
                element = FileUtils::fileName(xml.readElementText());
            }
            else if (folders && xml.name() == "collection")
            {
                if (!element.isEmpty() && element != "/") folderList << element;
            }
            else if (files && xml.name() == "getcontenttype" &&
                     !xml.readElementText(QXmlStreamReader::SkipChildElements).isEmpty())
            {
                if (!element.isEmpty()) fileList << element;
            }
        }
    }

    if (xml.hasError())
    {
        qCWarning(gmFileAccessNextCloud()) << "Error during parsing of PROPFIND response:" << xml.error() << xml.errorString();
        return new FileListResult(path, xml.errorString(), this);
    }

    return new FileListResult(path, folderList, fileList, this);
}

auto FileAccessNextcloud::createDirAsync(const QString &path) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Creating dir" << path << "...";

    auto *reply = NextCloud::getInstance()->sendDavRequest("MKCOL", encodePath(path), QByteArray());
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, reply]() {
        if (replyHasError(reply))
        {
            const auto errorMessage = makeAndPrintError(QString("Could not create directory %1").arg(path), reply);
            return deleteReplyAndReturn(new FileResult(errorMessage, this), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully created directory" << path;
        return deleteReplyAndReturn(new FileResult(true, this), reply);
    }).future();
}

auto FileAccessNextcloud::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult*>
{
    qCDebug(gmFileAccessNextCloud()) << "Checking if file" << path << "exists ...";

    // Check cache first
    if (allowCache && m_cache.checkEntry(path))
    {
        return completed(new FileCheckResult(path, true, this));
    }

    // If file is not in cache or cache is outdated, fetch from web
    auto *reply = NextCloud::getInstance()->sendDavRequest("PROPFIND", encodePath(path), QByteArray());
    return observe(reply, &QNetworkReply::finished).context(this, [this, path, reply]() {
        const auto doesExist = reply->error() != QNetworkReply::ContentNotFoundError;
        const auto hasError = replyHasError(reply) && doesExist;

        if (hasError)
        {
            const auto errorMessage = makeAndPrintError(QString("Could not check if file %1 exists").arg(path), reply);
            return deleteReplyAndReturn(new FileCheckResult(path, errorMessage, this), reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully checked if file" << path << "exists:" << doesExist;
        return deleteReplyAndReturn(new FileCheckResult(path, doesExist, this), reply);
    }).future();
}

auto FileAccessNextcloud::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult*>
{
    return checkAsync(new MultiGetHelper<FileCheckResult>(paths, this), allowCache);
}

auto FileAccessNextcloud::checkAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache) -> QFuture<FileMultiCheckResult*>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return completed(new FileMultiCheckResult(results, this));
    }

    auto next = checkAsync(helper->getNextPath(), allowCache);

    return observe(next).subscribe([this, helper, allowCache](FileCheckResult *result){
        helper->addResult(result);
        return checkAsync(helper, allowCache);
    }).future();
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
    auto result = QString("%1: %2").arg(errorMessage, replyErrorToString(reply));
    qCWarning(gmFileAccessNextCloud()) << "Warning:" << result;
    return result;
}

auto FileAccessNextcloud::replyErrorToString(const QNetworkReply *reply) -> QString
{
    return QString("%1 (%2)").arg(QString(reply->error()), reply->errorString());
}

auto FileAccessNextcloud::makeMoveHeaders(const QString& newPath) -> QList<QPair<QByteArray, QByteArray>>
{
    auto destinationHeader = QPair(QByteArray("Destination"), newPath.toUtf8());
    auto overwriteHeader  = QPair(QByteArray("Overwrite"), QByteArray("F"));
    return { destinationHeader, overwriteHeader };
}

template<typename T>
auto FileAccessNextcloud::deleteReplyAndReturn(const T &value, QNetworkReply *reply) -> T
{
    if (reply) reply->deleteLater();
    return value;
}