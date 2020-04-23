#ifndef FILEACCESS_H
#define FILEACCESS_H

#include <QObject>

enum FileRequest {
    GetFile,
    GetFiles,
    GetFileList,
    SaveFile,
    SaveFileDeleteOld,
    DeleteFile,
    CheckIfFilesExist
};

struct FileRequestContainer {
    FileRequestContainer(const int& id, const QString& filePath)
        : requestId(id), string1(filePath), requestType(GetFile) { }
    FileRequestContainer(const int& id, const QString& directory, const QString& fileEnding)
        : requestId(id), string1(directory), string2(fileEnding), requestType(GetFiles) { }
    FileRequestContainer(const int& id, const QString& directory, const bool& folders)
        : requestId(id), folders(folders), string1(directory), requestType(GetFileList) { }
    FileRequestContainer(const QString& filePath, const QByteArray& data)
        : string1(filePath), data(data), requestType(SaveFile) { }
    FileRequestContainer(const QString& newFile, const QByteArray& data, const QString& oldFile)
        : string1(newFile), string2(oldFile), data(data), requestType(SaveFileDeleteOld) { }
    FileRequestContainer(const QString& filePath)
        : string1(filePath), requestType(DeleteFile) { }
    FileRequestContainer(const int& requestId, const QStringList& files)
        : requestId(requestId), files(files), requestType(CheckIfFilesExist) { }

    int requestId = -1;
    bool folders = false;
    QString string1, string2;
    QByteArray data;
    QStringList files;
    FileRequest requestType;
};

class FileAccess : public QObject
{
    Q_OBJECT
public:
    explicit FileAccess(QObject *parent = nullptr) : QObject(parent) {}

    virtual void getFile(const int& requestId, const QString& filePath) = 0;
    virtual void getFiles(const int& requestId, const QString& directory, const QString& fileEnding) = 0;
    virtual void getFileList(const int& requestId, const QString& directory, const bool& folders) = 0;
    virtual void saveFile(const QString& filePath, const QByteArray& data) = 0;
    virtual void saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile) = 0;
    virtual void deleteFile(const QString& filePath) = 0;
    virtual void checkIfFilesExist(const int& requestId, QStringList files) = 0;

signals:
    void receivedFile(int requestId, QByteArray data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void checkedIfFilesExist(int id, QStringList found, QStringList notFound);
};

#endif // FILEACCESS_H
