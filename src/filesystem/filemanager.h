#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
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

    void getFile(int requestId, QString filePath);
    void getFiles(int requestId, QString directory, QString fileEnding = "");
    void getFileList(int requestId, QString directory, bool folders = false);

    void saveFile(QString filePath, QByteArray data);
    void saveFileDeleteOld(QString newFile, QByteArray data, QString oldFile);
    void deleteFile(QString filePath);
    void checkIfFilesExist(int requestId, QStringList files);

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
