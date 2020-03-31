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
    FileRequestContainer(int id, QString filePath) : requestId(id), string1(filePath), requestType(GetFile) { }
    FileRequestContainer(int id, QString directory, QString fileEnding) : requestId(id), string1(directory), string2(fileEnding), requestType(GetFiles) { }
    FileRequestContainer(int id, QString directory, bool folders) : requestId(id), folders(folders), string1(directory), requestType(GetFileList) { }
    FileRequestContainer(QString filePath, QByteArray data) : string1(filePath), data(data), requestType(SaveFile) { }
    FileRequestContainer(QString newFile, QByteArray data, QString oldFile) : string1(newFile), string2(oldFile), data(data), requestType(SaveFileDeleteOld) { }
    FileRequestContainer(QString filePath) : string1(filePath), requestType(DeleteFile) { }
    FileRequestContainer(int requestId, QStringList files) : requestId(requestId), files(files), requestType(CheckIfFilesExist) { }

    int requestId = -1;
    bool folders;
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

    virtual void getFile(int requestId, QString filePath) = 0;
    virtual void getFiles(int requestId, QString directory, QString fileEnding) = 0;
    virtual void getFileList(int requestId, QString directory, bool folders) = 0;
    virtual void saveFile(QString filePath, QByteArray data) = 0;
    virtual void saveFileDeleteOld(QString newFile, QByteArray data, QString oldFile) = 0;
    virtual void deleteFile(QString filePath) = 0;
    virtual void checkIfFilesExist(int requestId, QStringList files) = 0;

signals:
    void receivedFile(int requestId, QByteArray data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void checkedIfFilesExist(int id, QStringList found, QStringList notFound);
};

#endif // FILEACCESS_H
