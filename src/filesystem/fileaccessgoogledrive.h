#ifndef FILEACCESSGOOGLEDRIVE_H
#define FILEACCESSGOOGLEDRIVE_H

#include "fileaccess.h"
#include <QObject>

#include "services/googledrive.h"
#include "filesystem/googledrivefile.h"

class FileAccessGoogleDrive : public FileAccess
{
    Q_OBJECT
public:
    FileAccessGoogleDrive();

    void getFile(int requestId, QString filePath);
    void getFiles(int requestId, QString directory, QString fileEnding);
    void getFileList(int requestId, QString directory, bool folders);
    void saveFile(QString filePath, QByteArray data);
    void saveFileDeleteOld(QString newFile, QByteArray data, QString oldFile);
    void deleteFile(QString filePath);
    void checkIfFilesExist(int requestId, QStringList filePaths);

private:
    void getFiles(int requestId, GoogleDriveFile *folder, const QString& fileEnding);
    bool tryProcessSubrequest(int subId, const QByteArray& data);
    static void saveFileData(GoogleDriveFile *file, QByteArray data);

    void updateFilePart1(const QString& fileId, QByteArray data);
    static void updateFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data);

    void uploadFilePart1(const QString& fileName, GoogleDriveFile *parentFolder, const QByteArray& data);
    void uploadFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data, GoogleDriveFile *parentFolder);
    static void uploadFilePart3(const QByteArray& metaData, GoogleDriveFile *parentFolder, QByteArray data);

    static int createFolder(const QString& name, const QString& parentId);
    static void createdFolder(const QByteArray& data, GoogleDriveFile *parentFolder);

    static int getFilesInFolder(const QString& folderId, const QString& mimetype = "");
    void receivedFolderIndex(const QByteArray& data, GoogleDriveFile *parentFolder = nullptr);

    void startFileDownload(int requestId, GoogleDriveFile *file);
    void fileDownloadReceived(int requestId, QByteArray data);

    void dequeueRequests();

    GoogleDriveFile *m_baseFolder = nullptr;

    QList<FileRequestContainer> m_requestList;
    QMap<int, QByteArray> m_updateFileRequests;
    QMap<int, QPair<QByteArray, GoogleDriveFile*>> m_uploadFileRequests;
    QMap<int, QPair<QByteArray, GoogleDriveFile*>> m_uploadedFileRequests;
    QMap<int, QPair<QStringList, QStringList>> m_checkIfFilesExistRequests;

    QMap<int, GoogleDriveFile*> m_createFolderRequests;
    QMap<int, GoogleDriveFile*> m_folderIndexRequests;

    QMap<int, QPair<int, GoogleDriveFile*>> m_requestMap;
    QMap<int, QPair<QList<int>, QList<QByteArray>>> m_getFilesMap;

private slots:
    void receivedInternalReply(int internalId, QNetworkReply::NetworkError error, const QByteArray& data, const QList<QNetworkReply::RawHeaderPair>& headers);

};

#endif // FILEACCESSGOOGLEDRIVE_H
