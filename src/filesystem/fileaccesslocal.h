#ifndef FILEACCESSLOCAL_H
#define FILEACCESSLOCAL_H

#include "fileaccess.h"

#include <QThread>

class FileFinder : public QObject
{
    Q_OBJECT
public:
    FileFinder() {}

public slots:
    void getFile(int requestId, const QString& filePath);
    void getFiles(int requestId, const QString& directory, const QString& fileEnding);
    void getFileList(int requestId, const QString& directory, bool folders);
    static void saveFile(const QString& filePath, const QByteArray& data);
    void saveFileDeleteOld(QString newFile, QByteArray data, const QString& oldFile);
    static void deleteFile(const QString& filePath);
    void checkIfFilesExist(int requestId, const QStringList& files);

private:
    static QByteArray getFileData(const QString& filePath);

signals:
    void receivedFile(int requestId, QByteArray data);
    void receivedFiles(int requestId, QList<QByteArray> data);
    void receivedFileList(int requestId, QStringList fileNames);
    void checkedIfFilesExist(int requestId, QStringList found, QStringList notFound);
};

class FileAccessLocal : public FileAccess
{
    Q_OBJECT
    QThread workerThread;

public:
    FileAccessLocal(QObject *parent = nullptr);
    ~FileAccessLocal();

    void getFile(int id, QString filePath);
    void getFiles(int id, QString directory, QString fileEnding);
    void getFileList(int requestId, QString directory, bool folders);
    void saveFile(QString filePath, QByteArray data);
    void saveFileDeleteOld(QString newFile, QByteArray data, QString oldFile);
    void deleteFile(QString filePath);

    void checkIfFilesExist(int id, QStringList files);

signals:
    void startGettingFile(int requestId, QString filePath);
    void startGettingFiles(int requestId, QString directory, QString fileEnding);
    void startGettingFileList(int requestId, QString directory, bool folders);
    void startSavingFile(QString filePath, QByteArray data);
    void startSavingFileDeleteOld(QString newFile, QByteArray data, QString oldFile);
    void startDeletingFile(QString filePath);
    void startCheckingIfFilesExist(int requestId, QStringList files);
};

#endif // FILEACCESSLOCAL_H
