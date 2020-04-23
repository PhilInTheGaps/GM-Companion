#include "fileaccessgoogledrive.h"
#include "logging.h"
#include "services/services.h"

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))
# include <QRegExp>
#else // if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))
# include <QRegularExpression>
#endif // if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))

/**
 * @brief Constructor
 */
FileAccessGoogleDrive::FileAccessGoogleDrive()
{
    m_baseFolder = new GoogleDriveFile("", "root", true);

    connect(GoogleDrive::getInstance(), &GoogleDrive::receivedReply, this, &FileAccessGoogleDrive::receivedInternalReply);

    if (!GoogleDrive::getInstance()->isGranted())
    {
        GoogleDrive::getInstance()->grant();
    }
}

/**
 * @brief Get the content of a single file
 */
void FileAccessGoogleDrive::getFile(const int& requestId, const QString& filePath)
{
    qCDebug(gmFileAccessGoogle()) << "Processing getFile request" << requestId << filePath;
    auto reply = m_baseFolder->getFile(filePath);

    // Found file
    if (reply.status == Valid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found file, requesting ...";

        // Try to read data from temporary file
        auto data = reply.file->readData();

        if (!data.isEmpty())
        {
            emit receivedFile(requestId, data);
            dequeueRequests();
            return;
        }

        startFileDownload(requestId, reply.file);
        return;
    }

    // Found a folder but content is not valid, so we need to refresh
    if (reply.status == Invalid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
        m_requestList.push_back(FileRequestContainer(requestId, filePath));
        reply.file->setRefreshing();
        m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
        return;
    }

    // Folder is currently refreshing, we need to wait
    if (reply.status == Refreshing)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a file / directory in cache, but it is loading, waiting ...";
        m_requestList.push_back(FileRequestContainer(requestId, filePath));
        return;
    }

    // File was not found
    if (reply.status == NotFound)
    {
        qCWarning(gmFileAccessGoogle()) << "Warning: Could not find file" << filePath;
        emit receivedFile(requestId, "");
        dequeueRequests();
        return;
    }

    // Error
    qCCritical(gmFileAccessGoogle()) << "Error: could not find file" << filePath;
    dequeueRequests();
}

/**
 * @brief Start downloading a file
 */
void FileAccessGoogleDrive::startFileDownload(int requestId, GoogleDriveFile *file)
{
    file->setRefreshing();

    QUrl url("https://www.googleapis.com/drive/v3/files/" + file->id());

    QUrlQuery query;
    query.addQueryItem("alt", "media");
    url.setQuery(query);

    qCDebug(gmFileAccessGoogle()) << "Found file id, starting download" << url << "...";

    m_requestMap[GoogleDrive::getInstance()->get(QNetworkRequest(url))] = { requestId, file };
}

/**
 * @brief File was received, save it in cache
 */
void FileAccessGoogleDrive::saveFileData(GoogleDriveFile *file, const QByteArray& data)
{
    file->writeData(data);
}

/**
 * @brief Get the contents of all files matching a fileEnding in a directory
 */
void FileAccessGoogleDrive::getFiles(const int& requestId, const QString& directory, const QString& fileEnding)
{
    qCDebug(gmFileAccessGoogle()) << "Processing getFiles request" << requestId << directory << fileEnding;
    auto reply = m_baseFolder->getFile(directory);

    // Found folder and it's still valid, so get all files in it
    if (reply.status == Valid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found directory, requesting files ...";
        getFiles(requestId, reply.file, fileEnding);
        dequeueRequests();
        return;
    }

    // Found a folder but content is not valid, so we need to refresh
    if (reply.status == Invalid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
        m_requestList.push_back(FileRequestContainer(requestId, directory, fileEnding));
        reply.file->setRefreshing();
        m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
        return;
    }

    // Folder is currently refreshing, we need to wait
    if (reply.status == Refreshing)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, but it is refreshing, waiting ...";
        m_requestList.push_back(FileRequestContainer(requestId, directory, fileEnding));
        return;
    }

    // Folder was not found
    if (reply.status == NotFound)
    {
        qCWarning(gmFileAccessGoogle()) << "Warning: Could not find" << directory;
        dequeueRequests();
        return;
    }

    // Error
    qCCritical(gmFileAccessGoogle()) << "Error: could not find folder" << directory;
    dequeueRequests();
}

/**
 * @brief Get a list of files in a directory
 */
void FileAccessGoogleDrive::getFileList(const int& requestId, const QString& directory, const bool& folders)
{
    qCDebug(gmFileAccessGoogle()) << "Processing getFileList request" << requestId << directory << folders;
    auto reply = m_baseFolder->getFile(directory);

    // Found folder and it's still valid, so get all files in it
    if (reply.status == Valid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found directory ...";
        QStringList fileNames;

        for (auto file : reply.file->children())
        {
            if (file && (file->isFolder() == folders))
            {
                fileNames.append(file->name());
            }
        }

        emit receivedFileList(requestId, fileNames);
        dequeueRequests();
        return;
    }

    // Found a folder but content is not valid, so we need to refresh
    if (reply.status == Invalid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
        m_requestList.push_back(FileRequestContainer(requestId, directory, folders));
        reply.file->setRefreshing();
        m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
        return;
    }

    // Folder is currently refreshing, we need to wait
    if (reply.status == Refreshing)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, but it is refreshing, waiting ...";
        m_requestList.push_back(FileRequestContainer(requestId, directory, folders));
        return;
    }

    qCWarning(gmFileAccessGoogle()) << "Warning: Could not find" << directory;
    dequeueRequests();
}

/**
 * @brief Helper function that starts downloading all files in a folder that
 * match the fileEnding
 */
void FileAccessGoogleDrive::getFiles(int requestId, GoogleDriveFile *folder, const QString& fileEnding)
{
    if (!folder) return;

    qCDebug(gmFileAccessGoogle()) << "Requesting files in folder" << folder->name() << "that match" << fileEnding;

    m_getFilesMap[requestId] = QPair<QList<int>, QList<QByteArray> >();
    QMap<int, GoogleDriveFile *> tempMap;

    // Find files to request
    for (auto file : folder->children())
    {
        if (!file) continue;

        bool hasMatch = false;

#if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))
        QRegExp rx(fileEnding);
        rx.setPatternSyntax(QRegExp::Wildcard);
        hasMatch = rx.exactMatch(file->name());
#else // if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))
        auto re    = QRegularExpression(QRegularExpression::wildcardToRegularExpression(fileEnding));
        auto match = re.match(file->name());
        hasMatch = match.hasMatch();
#endif // if (QT_VERSION < QT_VERSION_CHECK(5, 12, 0))

        if (hasMatch)
        {
            auto subId = GoogleDrive::getInstance()->getUniqueRequestId();
            m_getFilesMap[requestId].first.append(subId);
            tempMap[subId] = file;
        }
    }

    // Try to load files from cache or start downloads
    for (auto subId : tempMap.keys())
    {
        auto file = tempMap[subId];
        auto data = file->readData();

        if (!data.isEmpty())
        {
            qCDebug(gmFileAccessGoogle()) << "Found file" << file->name() << "on disk.";

            if (!tryProcessSubrequest(subId, data))
            {
                qCCritical(gmFileAccessGoogle()) << "Error: file request does not appear in requests map";
            }
            continue;
        }

        startFileDownload(subId, file);
    }
}

/**
 * @brief Try to match a request for a file to the corresponding getFiles
 * request
 */
auto FileAccessGoogleDrive::tryProcessSubrequest(int subId, const QByteArray& data)->bool
{
    for (auto key : m_getFilesMap.keys())
    {
        if (m_getFilesMap[key].first.contains(subId))
        {
            qCDebug(gmFileAccessGoogle()) << "Received sub reply (" << subId << ") for getFiles request" << key;
            m_getFilesMap[key].first.removeOne(subId);
            m_getFilesMap[key].second.append(data);

            // If this subrequest was the last one, emit signal
            if (m_getFilesMap[key].first.length() < 1)
            {
                qCDebug(gmFileAccessGoogle()) << "Request (" << key << ") complete, received all sub requests.";
                emit receivedFiles(key, m_getFilesMap[key].second);
                m_getFilesMap.remove(key);
            }

            return true;
        }
    }

    return false;
}

/**
 * @brief Save a file. Either update it or upload a new file if it does not
 * exist
 */
void FileAccessGoogleDrive::saveFile(const QString& filePath, const QByteArray& data)
{
    qCDebug(gmFileAccessGoogle()) << "Processing saveFile request" << filePath;

    // Does file exist?
    auto reply = m_baseFolder->getFile(filePath);

    if (reply.status == Valid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found file, saving ...";
        reply.file->writeData(data);
        updateFilePart1(reply.file->id(), data);
        return;
    }

    // Found a folder but content is not valid, so we need to refresh
    if (reply.status == Invalid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
        m_requestList.push_back(FileRequestContainer(filePath, data));
        reply.file->setRefreshing();
        m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
        return;
    }

    // Folder is currently refreshing, we need to wait
    if (reply.status == Refreshing)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, but it is refreshing, waiting ...";
        m_requestList.push_back(FileRequestContainer(filePath, data));
        return;
    }

    // File was not found
    if (reply.status == NotFound)
    {
        auto fileName = filePath.split("/").last();

        // Do all folders exist only the file does not?
        if (reply.nextFile == fileName)
        {
            uploadFilePart1(fileName, reply.file, data);
        }

        // A folder in the path does not exist, create it
        else
        {
            reply.file->setRefreshing();
            m_createFolderRequests[createFolder(reply.nextFile, reply.file->id())] = reply.file;
            m_requestList.push_back(FileRequestContainer(filePath, data));
        }

        return;
    }

    dequeueRequests();
}

/**
 * @brief Update a file part 1: Send a patch request, response contains a new
 * url to upload file.
 */
void FileAccessGoogleDrive::updateFilePart1(const QString& fileId, const QByteArray& data)
{
    qCDebug(gmFileAccessGoogle()) << "Updating file" << fileId;
    QUrl url("https://www.googleapis.com/upload/drive/v3/files/" + fileId);

    QUrlQuery query;
    query.addQueryItem("uploadType", "resumable");
    url.setQuery(query);

    m_updateFileRequests[GoogleDrive::getInstance()->customRequest(QNetworkRequest(url), "PATCH", "")] = data;
}

/**
 * @brief Received patch response, upload file to the url specified in headers
 */
void FileAccessGoogleDrive::updateFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data)
{
    qCDebug(gmFileAccessGoogle()) << headers;

    for (const auto& header : headers)
    {
        if (header.first == "Location")
        {
            GoogleDrive::getInstance()->customRequest(QNetworkRequest(QUrl(header.second)), "PATCH", data);
            return;
        }
    }
}

/**
 * @brief Upload a file part 1: Send post request to receive a url to upload to.
 * Also send file metadata.
 */
void FileAccessGoogleDrive::uploadFilePart1(const QString& fileName, GoogleDriveFile *parentFolder, const QByteArray& data)
{
    qCDebug(gmFileAccessGoogle()) << "Uploading file to folder" << parentFolder;
    QUrl url("https://www.googleapis.com/upload/drive/v3/files");

    QUrlQuery query;
    query.addQueryItem("uploadType", "resumable");
    url.setQuery(query);

    QJsonObject metaData = {
        { "name", fileName }, { "parents", QJsonArray({ parentFolder->id() }) }
    };

    m_uploadFileRequests[GoogleDrive::getInstance()->post(QNetworkRequest(url), QJsonDocument(metaData).toJson(QJsonDocument::Compact))] = { data, parentFolder };
}

/**
 * @brief Upload a file part 2: Send a post request with file data to the url
 * specified in headers
 */
void FileAccessGoogleDrive::uploadFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data, GoogleDriveFile *parentFolder)
{
    qCDebug(gmFileAccessGoogle()) << headers;

    for (const auto& header : headers)
    {
        if (header.first == "Location")
        {
            m_uploadedFileRequests[GoogleDrive::getInstance()->post(QNetworkRequest(QUrl(header.second)), data)] = { data, parentFolder };
            return;
        }
    }
}

/**
 * @brief Upload a file part 3: Upload was successful, save file data in cache.
 * We needed to wait until now, because the response included the file id.
 */
void FileAccessGoogleDrive::uploadFilePart3(const QByteArray& metaData, GoogleDriveFile *parentFolder, const QByteArray& data)
{
    auto file =  QJsonDocument::fromJson(metaData).object();
    auto name = file["name"].toString();
    auto id   = file["id"].toString();

    auto gFile = new GoogleDriveFile(name, id, false, parentFolder);

    gFile->writeData(data);
    parentFolder->addChild(gFile);
}

/**
 * @brief Save a file under a new name and delete the old file. We could
 * probably simply rename the file, but it is easier to just reuse the saveFile
 * and deleteFile functions.
 */
void FileAccessGoogleDrive::saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile)
{
    saveFile(newFile, data);
    deleteFile(oldFile);
}

/**
 * @brief Delete the file at filePath
 */
void FileAccessGoogleDrive::deleteFile(const QString& filePath)
{
    qCDebug(gmFileAccessGoogle()) << "Processing deleteFile request" << filePath;

    // Does file exist?
    auto reply = m_baseFolder->getFile(filePath);

    if (reply.status == Valid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found file, deleting ...";
        QUrl url("https://www.googleapis.com/drive/v3/files/" + reply.file->id());
        GoogleDrive::getInstance()->customRequest(QNetworkRequest(url), "DELETE", "");
        delete reply.file;
        dequeueRequests();
        return;
    }

    // Found a folder but content is not valid, so we need to refresh
    if (reply.status == Invalid)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
        m_requestList.push_back(FileRequestContainer(filePath));
        reply.file->setRefreshing();
        m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
        return;
    }

    // Folder is currently refreshing, we need to wait
    if (reply.status == Refreshing)
    {
        qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, but it is refreshing, waiting ...";
        m_requestList.push_back(FileRequestContainer(filePath));
        return;
    }

    qCWarning(gmFileAccessGoogle()) << "Could not find file" << filePath << "to delete it.";
    dequeueRequests();
}

/**
 * @brief Find out if a list of files exist
 */
void FileAccessGoogleDrive::checkIfFilesExist(const int& requestId, QStringList filePaths)
{
    qCDebug(gmFileAccessGoogle()) << "Processing checkIfFilesExist request" << requestId << filePaths;

    m_checkIfFilesExistRequests[requestId] = QPair<QStringList, QStringList>();

    for (const auto& filePath : filePaths)
    {
        // Does file exist?
        auto reply = m_baseFolder->getFile(filePath);

        if (reply.status == Valid)
        {
            qCDebug(gmFileAccessGoogle()) << "Found file" << filePath;
            filePaths.removeOne(filePath);
            m_checkIfFilesExistRequests[requestId].first.append(filePath);
        }

        // Found a folder but content is not valid, so we need to refresh
        if (reply.status == Invalid)
        {
            qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, it is not valid, refreshing ...";
            m_requestList.push_back(FileRequestContainer(requestId, filePaths));
            reply.file->setRefreshing();
            m_folderIndexRequests[getFilesInFolder(reply.file->id())] = reply.file;
            return;
        }

        // Folder is currently refreshing, we need to wait
        if (reply.status == Refreshing)
        {
            qCDebug(gmFileAccessGoogle()) << "Found a directory in cache, but it is refreshing, waiting ...";
            m_requestList.push_back(FileRequestContainer(requestId, filePaths));
            return;
        }

        // File does not exist
        if (reply.status == NotFound)
        {
            qCDebug(gmFileAccessGoogle()) << "Did not find file" << filePath;
            filePaths.removeOne(filePath);
            m_checkIfFilesExistRequests[requestId].second.append(filePath);
        }
    }

    // We checked all filePaths
    if (filePaths.isEmpty())
    {
        auto pair = m_checkIfFilesExistRequests[requestId];
        m_checkIfFilesExistRequests.remove(requestId);

        qCDebug(gmFileAccessGoogle()) << "Finished checkIfFilesExist request";
        qCDebug(gmFileAccessGoogle()) << "Exist:" << pair.first;
        qCDebug(gmFileAccessGoogle()) << "Missing:" << pair.second;

        emit checkedIfFilesExist(requestId, pair.first, pair.second);
    }

    dequeueRequests();
}

/**
 * @brief Send a request to create a folder
 */
auto FileAccessGoogleDrive::createFolder(const QString& name, const QString& parentId)->int
{
    QUrl url("https://www.googleapis.com/drive/v3/files");
    QJsonObject root;

    root.insert("name",     name);
    root.insert("mimeType", GOOGLE_MIMETYPE_FOLDER);
    root.insert("parents",  QJsonArray({ parentId }));

    return GoogleDrive::getInstance()->post(QNetworkRequest(url), QJsonDocument(root).toJson(QJsonDocument::Compact));
}

void FileAccessGoogleDrive::createdFolder(const QByteArray& data, GoogleDriveFile *parentFolder)
{
    auto file = QJsonDocument::fromJson(data).object();

    parentFolder->addChild(new GoogleDriveFile(file, parentFolder));
    parentFolder->setRefreshed();

    qCDebug(gmFileAccessGoogle()) << "Created folder" << file["name"].toString();
}

/**
 * @brief Get all files with mimetype in a folder, to cache
 */
auto FileAccessGoogleDrive::getFilesInFolder(const QString& folderId, const QString& mimetype)->int
{
    QUrl url("https://www.googleapis.com/drive/v3/files");

    QString q = QString("trashed = false and '%1' in parents").arg(folderId);

    if (!mimetype.isEmpty()) q.append(QString(" and mimeType = '%1'").arg(mimetype));

    QUrlQuery query;

    query.addQueryItem("q",        QUrl::toPercentEncoding(q));
    query.addQueryItem("fields",   "files(name,id,parents,mimeType)");
    query.addQueryItem("orderBy",  "name");
    query.addQueryItem("pageSize", "1000");
    url.setQuery(query);
    return GoogleDrive::getInstance()->get(QNetworkRequest(url));
}

/**
 * @brief Folder index received, update cache
 */
void FileAccessGoogleDrive::receivedFolderIndex(const QByteArray& data, GoogleDriveFile *parentFolder)
{
    if (!parentFolder) parentFolder = m_baseFolder;

    qCDebug(gmFileAccessGoogle()) << "Found index for folder" << parentFolder->name();

    auto root = QJsonDocument::fromJson(data).object();

    // Insert new files into cache
    for (auto file : root["files"].toArray())
    {
        if (!parentFolder->containsChild(file.toObject()["name"].toString()))
        {
            qCDebug(gmFileAccessGoogle()) << "Inserting new file" << file.toObject();
            parentFolder->addChild(new GoogleDriveFile(file.toObject(), parentFolder));
        }
    }

    // Did not receive all files in one reply, request the next page
    qCDebug(gmFileAccessGoogle()) << root;

    if (root.contains("nextPageToken"))
    {
        QUrl url("https://www.googleapis.com/drive/v3/files");
        QUrlQuery query;
        query.addQueryItem("pageToken", root["nextPageToken"].toString());
        url.setQuery(query);

        m_folderIndexRequests[GoogleDrive::getInstance()->get(QNetworkRequest(url))] = parentFolder;
        return;
    }

    parentFolder->receivedIndex();

    // Continue processing requests, maybe files can be found now
    dequeueRequests();
}

void FileAccessGoogleDrive::dequeueRequests()
{
    if (m_requestList.isEmpty()) return;

    auto request = m_requestList.takeFirst();

    switch (request.requestType)
    {
    case GetFile:
        getFile(request.requestId, request.string1);
        break;

    case GetFiles:
        getFiles(request.requestId, request.string1, request.string2);
        break;

    case GetFileList:
        getFileList(request.requestId, request.string1, request.folders);
        break;

    case SaveFile:
        saveFile(request.string1, request.data);
        break;

    case SaveFileDeleteOld:
        saveFileDeleteOld(request.string1, request.data, request.string2);
        break;

    case DeleteFile:
        deleteFile(request.string1);
        break;

    case CheckIfFilesExist:
        checkIfFilesExist(request.requestId, request.files);
        break;
    }
}

void FileAccessGoogleDrive::receivedInternalReply(int internalId, QNetworkReply::NetworkError error, const QByteArray& data, const QList<QNetworkReply::RawHeaderPair>& headers)
{
    if (error != QNetworkReply::NoError)
    {
        qCCritical(gmFileAccessGoogle()) << error << data << headers;
        return;
    }

    if (m_folderIndexRequests.contains(internalId))
    {
        receivedFolderIndex(data, m_folderIndexRequests[internalId]);
        m_folderIndexRequests.remove(internalId);
        return;
    }

    if (m_createFolderRequests.contains(internalId))
    {
        m_createFolderRequests.remove(internalId);
        createdFolder(data, m_createFolderRequests[internalId]);
        dequeueRequests();
        return;
    }

    if (m_updateFileRequests.contains(internalId))
    {
        updateFilePart2(headers, m_updateFileRequests[internalId]);
        m_updateFileRequests.remove(internalId);
        dequeueRequests();
        return;
    }

    if (m_uploadFileRequests.contains(internalId))
    {
        auto pair = m_uploadFileRequests[internalId];
        uploadFilePart2(headers, pair.first, pair.second);
        m_uploadFileRequests.remove(internalId);
        return;
    }

    if (m_uploadedFileRequests.contains(internalId))
    {
        auto pair = m_uploadedFileRequests[internalId];
        m_uploadedFileRequests.remove(internalId);
        uploadFilePart3(data, pair.second, pair.first);
        dequeueRequests();
        return;
    }

    if (m_requestMap.contains(internalId))
    {
        auto requestPair = m_requestMap[internalId];
        m_requestMap.remove(internalId);

        // Check if request was part of a "getFiles" request, then external is
        // actually a subId
        if (!tryProcessSubrequest(requestPair.first, data))
        {
            // Request was an independet "getFile" request
            qCDebug(gmFileAccessGoogle()) << "Received file from independent request (" << requestPair.first << ")" << internalId;
            emit receivedFile(requestPair.first, data);
        }

        saveFileData(requestPair.second, data);
        dequeueRequests();
        return;
    }

    qCDebug(gmFileAccessGoogle()) << "Received reply:" << internalId << QJsonDocument::fromJson(data).toJson(QJsonDocument::Compact);
    dequeueRequests();
}
