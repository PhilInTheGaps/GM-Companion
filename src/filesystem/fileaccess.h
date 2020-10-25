#ifndef FILEACCESS_H
#define FILEACCESS_H

#include <QObject>

class FileAccess : public QObject
{
    Q_OBJECT
public:
    explicit FileAccess(QObject *parent = nullptr) : QObject(parent) {}

    virtual void getFile(int requestId, const QString &filePath) = 0;
    virtual void getFiles(int requestId, const QString &directory, const QString &fileEnding) = 0;
    virtual void getFileList(int requestId, const QString &directory, bool folders) = 0;
    virtual void saveFile(int requestId, const QString &filePath, const QByteArray &data) = 0;
    virtual void renameFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data = "") = 0;
    virtual void renameFolder(int requestId, const QString &newFolder, const QString& oldFolder) = 0;
    virtual void deleteFile(int requestId, const QString &filePath) = 0;
    virtual void checkIfFilesExist(int requestId, QStringList files) = 0;
    virtual void createFolder(int requestId, const QString &folderPath) = 0;

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
    void receivedFile(int requestId, const QByteArray &data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void savedFile(int requestId);
    void renamedFile(int requestId);
    void renamedFolder(int requestId);
    void deletedFile(int requestId);
    void createdFolder(int requestId);
    void checkedIfFilesExist(int requestId, QStringList found, QStringList notFound);
};

#endif // FILEACCESS_H
