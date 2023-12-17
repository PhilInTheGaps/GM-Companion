#include "fileaccessnextcloud.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QQueue>
#include <QXmlStreamReader>
#include <utility>

using namespace Files;
using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmFileAccessNextCloud, "gm.files.access.nextcloud")

FileAccessNextcloud::FileAccessNextcloud(Services::NextCloud &nextcloud) : m_nc(nextcloud)
{
}

auto FileAccessNextcloud::getDataAsync(const QString &path, Options options) -> QFuture<FileDataResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting data from file" << path;

    // Try to get from cache first
    if (options.testFlag(Option::AllowCache))
    {
        QByteArray data;
        if (m_cache.tryGetData(path, data))
        {
            return QtFuture::makeReadyFuture(FileDataResult(std::move(data)));
        }
    }

    // Fetch from web
    auto future = m_nc.sendDavRequest("GET", encodePath(path), {}, {},
                                      options.testFlag(Option::LowPriority) ? Services::Option::LowPriority
                                                                            : Services::Option::Authenticated);

    return future.then([future, path](const Services::RestReply &reply) {
        if (reply.hasError())
        {
            printError(u"Could not get data from %1"_s.arg(path), reply);
        }
        else
        {
            qCDebug(gmFileAccessNextCloud()) << "Received data from file" << path;
        }

        return FileDataResult::fromRestReply(reply);
    });
}

auto FileAccessNextcloud::getDataAsync(const QStringList &paths, Options options)
    -> QFuture<std::vector<FileDataResult>>
{
    return FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult>(paths), options);
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

    auto future = m_nc.sendDavRequest("PUT", encodePath(path), data, {}, Services::Option::Authenticated);

    return future
        .then([this, future, path, data](const Services::RestReply &reply) {
            if (reply.hasError())
            {
                if (const auto isDirMissing = reply.error() == QNetworkReply::ContentNotFoundError; isDirMissing)
                {
                    return createDirThenContinue<QString, QByteArray>(
                        FileUtils::dirFromPath(path), path, data,
                        [this](const QString &path, const QByteArray &data) { return saveAsync(path, data); });
                }

                printError(u"Could not save file %1"_s.arg(path), reply);
                return QtFuture::makeReadyFuture(FileResult::fromRestReply(reply));
            }

            if (!m_cache.createOrUpdateEntry(path, data))
            {
                qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << path << "in cache";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully saved file" << path;
            return QtFuture::makeReadyFuture(FileResult(true));
        })
        .unwrap();
}

auto FileAccessNextcloud::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Moving file" << oldPath << "to" << newPath;

    auto future = m_nc.sendDavRequest("MOVE", encodePath(oldPath), QByteArray(), makeMoveHeaders(newPath),
                                      Services::Option::Authenticated);

    return future
        .then([this, future, oldPath, newPath](const Services::RestReply &reply) {
            qCDebug(gmFileAccessNextCloud()) << "MOVE Reply:" << reply.error() << reply.errorText();

            if (reply.hasError())
            {
                if (const auto isDirMissing = reply.error() == QNetworkReply::ContentConflictError; isDirMissing)
                {
                    return createDirThenContinue<QString, QString>(
                        FileUtils::dirFromPath(newPath), oldPath, newPath,
                        [this](const QString &oldPath, const QString &newPath) { return moveAsync(oldPath, newPath); });
                }

                printError(u"Could not move file %1 to %2"_s.arg(oldPath, newPath), reply);
                return QtFuture::makeReadyFuture(FileResult::fromRestReply(reply));
            }

            if (!m_cache.moveEntry(oldPath, newPath))
            {
                qCWarning(gmFileAccessNextCloud()) << "Error: Could not update file" << oldPath << "in cache";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully moved file" << oldPath << "to" << newPath;
            return QtFuture::makeReadyFuture(FileResult(true));
        })
        .unwrap();
}

auto FileAccessNextcloud::deleteAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Deleting file" << path << "...";

    auto future = m_nc.sendDavRequest("DELETE", encodePath(path), {}, {}, Services::Option::Authenticated);

    return future
        .then([this, future, path](const Services::RestReply &reply) {
            if (reply.hasError())
            {
                printError(u"Could not delete file/folder %1"_s.arg(path), reply);
                return QtFuture::makeReadyFuture(FileResult::fromRestReply(reply));
            }

            m_cache.removeEntry(path);

            qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
            return QtFuture::makeReadyFuture(FileResult(true));
        })
        .unwrap();
}

auto FileAccessNextcloud::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Copying file" << path << "to" << copy;

    const auto destinationHeader = std::make_pair(QByteArray("Destination"), copy.toUtf8());
    const auto overwriteHeader = std::make_pair(QByteArray("Overwrite"), QByteArray("F"));

    auto future = m_nc.sendDavRequest("COPY", encodePath(path), {}, {destinationHeader, overwriteHeader},
                                      Services::Option::Authenticated);

    return future
        .then([this, future, path, copy](const Services::RestReply &reply) {
            if (reply.hasError())
            {
                if (const auto isDirMissing = reply.error() == QNetworkReply::ContentConflictError; isDirMissing)
                {
                    return createDirThenContinue<QString, QString>(
                        FileUtils::dirFromPath(copy), path, copy,
                        [this](const QString &path, const QString &copy) { return copyAsync(path, copy); });
                }

                printError(u"Could not copy %1 to %2: %3 %4"_s.arg(path), reply);
                return QtFuture::makeReadyFuture(FileResult::fromRestReply(reply));
            }

            if (!m_cache.copyEntry(path, copy))
            {
                qCDebug(gmFileAccessNextCloud()) << "Could not copy file" << path << "to" << copy
                                                 << "in cache. (local version of file is probably too old)";
            }

            qCDebug(gmFileAccessNextCloud()) << "Successfully deleted file" << path;
            return QtFuture::makeReadyFuture(FileResult(true));
        })
        .unwrap();
}

auto FileAccessNextcloud::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Getting list of"
                                     << (folders ? (files ? "files and folders" : "folders") : "files") << "in path"
                                     << path << "...";

    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), {}, {}, Services::Option::Authenticated);

    return future.then([future, path, files, folders](const Services::RestReply &reply) {
        if (reply.hasError())
        {
            printError(u"Could not list content of folder %1"_s.arg(path), reply);
            return FileListResult(path, reply.errorText());
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully received content of" << path;
        return parseListResponse(reply.data(), path, files, folders);
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

    auto future = m_nc.sendDavRequest("MKCOL", encodePath(path), {}, {}, Services::Option::Authenticated);

    return future.then([future, path](const Services::RestReply &reply) {
        if (reply.hasError())
        {
            printError(u"Could not create directory %1"_s.arg(path), reply);
            return FileResult::fromRestReply(reply);
        }

        qCDebug(gmFileAccessNextCloud()) << "Successfully created directory" << path;
        return FileResult(true);
    });
}

auto FileAccessNextcloud::checkAsync(const QString &path, Options options) -> QFuture<FileCheckResult>
{
    qCDebug(gmFileAccessNextCloud()) << "Checking if file" << path << "exists ...";

    // Check cache first
    if (options.testFlag(Option::AllowCache) && m_cache.checkEntry(path))
    {
        return QtFuture::makeReadyFuture(FileCheckResult(path, true));
    }

    // If file is not in cache or cache is outdated, fetch from web
    auto future = m_nc.sendDavRequest("PROPFIND", encodePath(path), QByteArray(), {}, Services::Option::Authenticated);

    return future.then([future, path](const Services::RestReply &reply) {
        const auto doesExist = reply.error() != QNetworkReply::ContentNotFoundError;

        if (const auto hasError = reply.hasError() && doesExist; hasError)
        {
            printError(u"Could not check if file %1 exists"_s.arg(path), reply);
            return FileCheckResult(path, reply.errorText());
        }

        qCDebug(gmFileAccessNextCloud()) << "PROPFIND:" << reply.data();
        qCDebug(gmFileAccessNextCloud()) << "Successfully checked if file" << path << "exists:" << doesExist;
        return FileCheckResult(path, doesExist);
    });
}

auto FileAccessNextcloud::checkAsync(const QStringList &paths, Options options) -> QFuture<FileMultiCheckResult>
{
    return FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult>(paths), options);
}

auto FileAccessNextcloud::encodePath(const QString &data) -> QByteArray
{
    return QUrl::toPercentEncoding(data, "/");
}

void FileAccessNextcloud::printError(const QString &errorMessage, const Services::RestReply &reply)
{
    qCWarning(gmFileAccessNextCloud()) << "Warning:" << errorMessage << reply.error() << ":" << reply.errorText();
}

auto FileAccessNextcloud::makeMoveHeaders(const QString &newPath) -> QList<std::pair<QByteArray, QByteArray>>
{
    auto destinationHeader = std::make_pair(QByteArray("Destination"), newPath.toUtf8());
    auto overwriteHeader = std::make_pair(QByteArray("Overwrite"), QByteArray("F"));
    return {destinationHeader, overwriteHeader};
}
