#ifndef FILEACCESSGOOGLEDRIVE_H
#define FILEACCESSGOOGLEDRIVE_H

#include "fileaccess.h"
#include "filerequestcontainer.h"
#include "services/google/googledrive.h"
#include "googledrivefile.h"

class FileAccessGoogleDrive : public FileAccess
{
    Q_OBJECT
public:
    FileAccessGoogleDrive(QObject *parent);

    void getFile(int requestId, const QString &filePath) override;
    void getFiles(int requestId, const QString &directory, const QString &fileEnding) override;
    void getFileList(int requestId, const QString &directory, bool folders) override;

    void saveFile(int requestId, const QString &filePath, const QByteArray &data) override;
    void renameFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data) override;
    void renameFolder(int requestId, const QString &newFolder, const QString &oldFolder) override;
    void deleteFile(int requestId, const QString &filePath) override;
    void checkIfFilesExist(int requestId, QStringList filePaths) override;
    void createFolder(int requestId, const QString &folderPath) override;

private:
    void getFiles(int requestId, GoogleDriveFile *folder, const QString& fileEnding);
    bool tryProcessSubrequest(int subId, const QByteArray& data);
    static void saveFileData(GoogleDriveFile *file, const QByteArray& data);

    void updateFilePart1(int requestId, const QString& fileId, const QByteArray &data);
    static void updateFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data);

    void uploadFilePart1(int requestId, const QString& fileName, GoogleDriveFile *parentFolder, const QByteArray& data);
    void uploadFilePart2(const QList<QNetworkReply::RawHeaderPair>& headers, const QByteArray& data, GoogleDriveFile *parentFolder);
    static void uploadFilePart3(const QByteArray& metaData, GoogleDriveFile *parentFolder, const QByteArray& data);

    int createFolder(int requestId, const QString& name, const QString& parentId);
    static void onCreatedFolder(const QByteArray& data, GoogleDriveFile *parentFolder);

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
