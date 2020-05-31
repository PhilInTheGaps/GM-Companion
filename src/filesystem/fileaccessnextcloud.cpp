#include "fileaccessnextcloud.h"
#include "../services/nextcloud/nextcloud.h"
#include "logging.h"
#include "utils/utils.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QUrl>

FileAccessNextCloud::FileAccessNextCloud(QObject *parent) : FileAccess(parent)
{
}

void FileAccessNextCloud::getFile(int requestId, const QString& filePath)
{
    qCDebug(gmFileAccessNextCloud()) << "Getting file" << filePath;

    QByteArray data;

    if (tryGetFileFromCache(filePath, &data))
    {
        emit receivedFile(requestId, data);
        return;
    }

    auto *reply = NextCloud::getInstance()->sendDavRequest("GET", QUrl::toPercentEncoding(filePath, "/"), "");
    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto data = reply->readAll();

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmFileAccessNextCloud()) << reply->error() << reply->errorString();
        }
        else
        {
            qCDebug(gmFileAccessNextCloud()) << "Received file" << filePath << "id:" << requestId;

            saveFileInCache(filePath, data);
            emit receivedFile(requestId, data);
        }

        reply->deleteLater();
    });
}

void FileAccessNextCloud::getFiles(int requestId, const QString& directory, const QString& fileEnding)
{
    qCDebug(gmFileAccessNextCloud()) << "Getting files in directory" << directory << "with fileEnding filter:" << fileEnding;

    auto internalRequestId = FileManager::getUniqueRequestId();
    auto *context = new QObject;
    connect(this, &FileAccessNextCloud::receivedFileList, context, [ = ](int filesRequestId, QStringList fileNames) {
        if (filesRequestId != internalRequestId) return;

        auto *fileRequests = new QMap<int, QString>;

        for (auto fileName : fileNames)
        {
            if (!Utils::hasWildcardMatch(fileName, fileEnding)) continue;

            auto filePath = directory + (directory.endsWith('/') ? "" : "/") + fileName;
            fileRequests->insert(FileManager::getUniqueRequestId(), filePath);
        }

        downloadFiles(requestId, fileRequests);
        delete context;
    });

    getFileList(internalRequestId, directory, false);
}

/**
 * @brief Stage 2 of getFiles, get all files found in folder
 */
void FileAccessNextCloud::downloadFiles(const int& requestId, QMap<int, QString> *requests)
{
    auto dataList = new QList<QByteArray>;

    for (auto id : requests->keys())
    {
        auto *context = new QObject;
        connect(this, &FileAccessNextCloud::receivedFile, context, [ = ](int fileRequestId, QByteArray data) {
            if (fileRequestId != id) return;

            dataList->append(data);
            requests->remove(fileRequestId);

            qCDebug(gmFileAccessNextCloud()) << "Received file for subrequest" << id;
            qCDebug(gmFileAccessNextCloud()) << requests->size() << "files to go";

            if (requests->isEmpty())
            {
                emit receivedFiles(requestId, *dataList);
                delete requests;
                delete dataList;
            }

            delete context;
        });

        getFile(id, requests->value(id));
    }
}

void FileAccessNextCloud::getFileList(int requestId, const QString& directory, bool folders)
{
    qCDebug(gmFileAccessNextCloud()) << "Getting a list of files in directory" << directory << "folders:" << folders;
    auto *reply = NextCloud::getInstance()->sendDavRequest("PROPFIND", QUrl::toPercentEncoding(directory, "/"), getFileListXml());

    connect(reply, &QNetworkReply::finished, [ = ]() {
        QXmlStreamReader xml(reply->readAll());

        QStringList fileNames;
        QString tempHref, tempContentType, tempStatus;

        while (!xml.atEnd())
        {
            auto tokenType = xml.readNext();

            if (tokenType == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "response")
                {
                    tempHref.clear();
                    tempContentType.clear();
                    tempStatus.clear();
                }
                else if (xml.name() == "href") tempHref = xml.readElementText();
                else if (xml.name() == "getcontenttype") tempContentType = xml.readElementText();
                else if (xml.name() == "status") tempStatus = xml.readElementText();
            }
            else if ((tokenType == QXmlStreamReader::EndElement) && (xml.name() == "response"))
            {
                // If content type is empty -> element is folder
                if ((folders && tempContentType.isEmpty()) || (!folders && !tempContentType.isEmpty()))
                {
                    // Folders always return 404, but elements should exists and have 200, safety check
                    if (folders || tempStatus.contains("200"))
                    {
                        fileNames.append(FileAccessNextCloud::fileNameFromHref(tempHref));
                    }
                }
            }
        }

        // Chop first folder as it is "."
        if (folders && !fileNames.isEmpty()) fileNames.removeFirst();

        qCDebug(gmFileAccessNextCloud()) << fileNames;
        emit receivedFileList(requestId, fileNames);
        reply->deleteLater();
    });
}

void FileAccessNextCloud::saveFile(const QString& filePath, const QByteArray& data)
{
    qCDebug(gmFileAccessNextCloud()) << "Saving file" << filePath;

    saveFileInCache(filePath, data);

    auto *reply = NextCloud::getInstance()->sendDavRequest("PUT", QUrl::toPercentEncoding(filePath, "/"), data);
    connect(reply, &QNetworkReply::finished, [ = ]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not save file" << filePath;
            qCWarning(gmFileAccessNextCloud()) << reply->error() << reply->errorString();
        }
        reply->deleteLater();
    });
}

/**
 * If data is empty, rename, else reupload data under a new filename and delete the old file.
 */
void FileAccessNextCloud::renameFile(const QString& newFile, const QString& oldFile, const QByteArray& data)
{
    if (data.isEmpty())
    {
        qCDebug(gmFileAccessNextCloud()) << "Renaming file" << oldFile << "to" << newFile;

        QPair<QByteArray, QByteArray> destinationHeader = { "Destination",  NextCloud::getInstance()->getPathUrl(newFile).toUtf8() };
        QPair<QByteArray, QByteArray> overwriteHeader = { "Overwrite",  "F" };

        auto *reply = NextCloud::getInstance()->sendDavRequest(
                    "MOVE", QUrl::toPercentEncoding(oldFile, "/"), "", { destinationHeader, overwriteHeader });

        connect(reply, &QNetworkReply::finished, [ = ]() {
            if (reply->error() != QNetworkReply::NoError)
            {
                qCWarning(gmFileAccessNextCloud()) << "Error: Could not rename file" << oldFile;
                qCWarning(gmFileAccessNextCloud()) << reply->error() << reply->errorString();
            }
            reply->deleteLater();
        });
    }
    else
    {
        qCDebug(gmFileAccessNextCloud()) << "Saving file under new name and deleting old" << newFile << oldFile;

        saveFile(newFile, data);
        deleteFile(oldFile);
    }
}

/**
 * Rename a folder, same as renameFile with empty data.
 */
void FileAccessNextCloud::renameFolder(const QString &newFolder, const QString &oldFolder)
{
    renameFile(newFolder, oldFolder, "");
}

void FileAccessNextCloud::deleteFile(const QString& filePath)
{
    qCDebug(gmFileAccessNextCloud()) << "Deleting file" << filePath;

    auto *reply = NextCloud::getInstance()->sendDavRequest("DELETE", QUrl::toPercentEncoding(filePath, "/"), "");
    connect(reply, &QNetworkReply::finished, [ = ]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not delete file" << filePath;
            qCWarning(gmFileAccessNextCloud()) << reply->error() << reply->errorString();
        }
        reply->deleteLater();
    });
}

void FileAccessNextCloud::checkIfFilesExist(int requestId, QStringList files)
{
    qCDebug(gmFileAccessNextCloud()) << "Checking if files exist:" << files;

    auto *found       = new QStringList;
    auto *missing     = new QStringList;
    auto *filesLeft   = new QStringList(files);

    for (auto file : files)
    {
        auto *reply = NextCloud::getInstance()->sendDavRequest("PROPFIND", file, "");
        connect(reply, &QNetworkReply::finished, [=](){
            if (reply->error() != QNetworkReply::NoError) missing->append(file);
            else found->append(file);

            filesLeft->removeOne(file);
            reply->deleteLater();

            // Received the last reply
            if (filesLeft->isEmpty())
            {
                emit checkedIfFilesExist(requestId, *found, *missing);
                delete found;
                delete missing;
                delete filesLeft;
            }
        });
    }
}

void FileAccessNextCloud::createFolder(const QString &folderPath)
{
    qCDebug(gmFileAccessNextCloud()) << "Creating folder" << folderPath;

    auto *reply = NextCloud::getInstance()->sendDavRequest("MKCOL", QUrl::toPercentEncoding(folderPath, "/"), "");
    connect(reply, &QNetworkReply::finished, [ = ]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmFileAccessNextCloud()) << "Error: Could not create folder" << folderPath;
            qCWarning(gmFileAccessNextCloud()) << reply->error() << reply->errorString();
        }
        reply->deleteLater();
    });
}

auto FileAccessNextCloud::fileNameFromHref(QString href)->QString
{
    if (href.endsWith('/')) href.chop(1);

    auto index = href.lastIndexOf('/');
    href = href.right(href.length() - index - 1);
    return QUrl::fromPercentEncoding(href.toUtf8());
}

bool FileAccessNextCloud::tryGetFileFromCache(const QString &filePath, QByteArray *data)
{
    if (!m_fileCache.contains(filePath)) return false;

    auto file = m_fileCache[filePath];

    if (!file->open()) return false;

    *data = file->readAll();
    file->close();

    qCDebug(gmFileAccessNextCloud()) << "Cache hit!" << filePath;

    return true;
}

void FileAccessNextCloud::saveFileInCache(const QString &filePath, const QByteArray &data)
{
    QTemporaryFile *file = nullptr;

    if (m_fileCache.contains(filePath))
    {
        file = m_fileCache[filePath];
    }
    else
    {
        file = new QTemporaryFile;
        m_fileCache.insert(filePath, file);
    }

    if (file && file->open())
    {
        file->write(data);
        file->close();
    }
}

void FileAccessNextCloud::removeFileFromCache(const QString &filePath)
{
    if (!m_fileCache.contains(filePath)) return;

    auto file = m_fileCache.take(filePath);
    delete file;
}

auto FileAccessNextCloud::getXml(QString filepath) const->QByteArray
{
    QFile f(filepath);
    QByteArray data;

    if (f.open(QIODevice::ReadOnly))
    {
        data = f.readAll();
        f.close();
    }

    return data;
}

auto FileAccessNextCloud::getFileListXml() const->QByteArray
{
    return getXml(":/services/nextcloud/getFileList.xml");
}
