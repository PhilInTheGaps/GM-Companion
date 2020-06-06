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

    void saveFile(QString filePath, const QByteArray &data);
    void renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data = "");
    void renameFolder(const QString& newFolder, const QString& oldFolder);
    void deleteFile(const QString &filePath);
    void checkIfFilesExist(int requestId, const QStringList &files);
    void createFolder(const QString &folderPath);

private:
    explicit FileManager(QObject *parent = nullptr);

    static bool instanceFlag;
    static FileManager *single;
    static int fileAccessCount;

    FileAccess *m_fileAccess = nullptr;

signals:
    void receivedFile(int id, QByteArray data);
    void receivedFiles(int id, QList<QByteArray> data);
    void receivedFileList(int id, QStringList fileNames);
    void checkedIfFilesExist(int id, QStringList found, QStringList notFound);
};

#endif // FILEMANAGER_H
