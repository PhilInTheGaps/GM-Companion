#ifndef FILEACCESS_H
#define FILEACCESS_H

#include <QQmlContext>
#include <QObject>

class FileAccess : public QObject
{
    Q_OBJECT
public:
    explicit FileAccess(QObject *parent = nullptr) : QObject(parent) {}

    virtual void getFile(int requestId, const QString &filePath) = 0;
    virtual void getFiles(int requestId, const QString &directory, const QString &fileEnding) = 0;
    virtual void getFileList(int requestId, const QString &directory, bool folders) = 0;
    virtual void saveFile(const QString &filePath, const QByteArray &data) = 0;
    virtual void renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data = "") = 0;
    virtual void renameFolder(const QString &newFolder, const QString& oldFolder) = 0;
    virtual void deleteFile(const QString &filePath) = 0;
    virtual void checkIfFilesExist(int requestId, QStringList files) = 0;
    virtual void createFolder(const QString &folderPath) = 0;

    enum FileRequest {
        GetFile,
        GetFiles,
        GetFileList,
        SaveFile,
        RenameFile,
        RenameFolder,
        RemoveFile,
        CheckIfFilesExist,
        CreateFolder
    };

signals:
    void receivedFile(int requestId, QByteArray data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void checkedIfFilesExist(int id, QStringList found, QStringList notFound);
};

struct FileRequestContainer {
    /// Get File
    FileRequestContainer(const int& id, const QString& filePath)
        : requestId(id), string1(filePath), requestType(FileAccess::GetFile) { }

    /// Get Files
    FileRequestContainer(const int& id, const QString& directory, const QString& fileEnding)
        : requestId(id), string1(directory), string2(fileEnding), requestType(FileAccess::GetFiles) { }

    /// Get File List
    FileRequestContainer(const int& id, const QString& directory, const bool& folders)
        : requestId(id), folders(folders), string1(directory), requestType(FileAccess::GetFileList) { }

    /// Save File
    FileRequestContainer(const QString& filePath, const QByteArray& data)
        : string1(filePath), data(data), requestType(FileAccess::SaveFile) { }

    /// Rename File
    FileRequestContainer(const QString& newFile, const QString& oldFile, const QByteArray& data)
        : string1(newFile), string2(oldFile), data(data), requestType(FileAccess::RenameFile) { }

    /// Rename Folder
    FileRequestContainer(const QString& newFolder, const QString& oldFolder)
        : string1(newFolder), string2(oldFolder), requestType(FileAccess::RenameFolder) { }

    /// Remove File / Create Folder
    FileRequestContainer(const QString& filePath, FileAccess::FileRequest requestType)
        : string1(filePath), requestType(requestType) { }

    /// Check if files exist
    FileRequestContainer(const int& requestId, const QStringList& files)
        : requestId(requestId), files(files), requestType(FileAccess::CheckIfFilesExist) { }

    int requestId = -1;
    bool folders = false;
    QString string1, string2;
    QByteArray data;
    QStringList files;
    FileAccess::FileRequest requestType;
};

#endif // FILEACCESS_H
