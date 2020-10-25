#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "fileaccess.h"

class FileManager : public QObject
{
    Q_OBJECT
public:
    static FileManager *getInstance();
    ~FileManager();

    static int getUniqueRequestId();

public slots:
    void updateFileAccess();

    void getFile(int requestId, const QString &filePath);
    void getFiles(int requestId, const QString &directory, const QString &fileEnding = "");
    void getFileList(int requestId, const QString &directory, bool folders = false);

    void saveFile(const QString &filePath, const QByteArray &data);
    void saveFile(int requestId, const QString &filePath, const QByteArray &data);

    void renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data = "");
    void renameFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data = "");

    void renameFolder(const QString& newFolder, const QString& oldFolder);
    void renameFolder(int requestId, const QString& newFolder, const QString& oldFolder);

    void deleteFile(const QString &filePath);
    void deleteFile(int requestId, const QString &filePath);

    void checkIfFilesExist(int requestId, const QStringList &files);

    void createFolder(const QString &folderPath);
    void createFolder(int requestId, const QString &folderPath);

private:
    explicit FileManager(QObject *parent = nullptr);

    static bool instanceFlag;
    static FileManager *single;
    static int fileAccessCount;

    FileAccess *m_fileAccess = nullptr;

    static QString cleanPath(const QString &path);

signals:
    void receivedFile(int requestId, const QByteArray &data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void savedFile(int requestId);
    void renamedFile(int requestId);
    void renamedFolder(int requestId);
    void deletedFile(int requestId);
    void createdFolder(int requestId);
    void checkedIfFilesExist(int id, QStringList found, QStringList notFound);
};

#endif // FILEMANAGER_H
