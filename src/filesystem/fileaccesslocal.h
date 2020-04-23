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
    void getFile(const int& requestId, const QString& filePath);
    void getFiles(const int& requestId, const QString& directory, const QString& fileEnding);
    void getFileList(const int& requestId, const QString& directory, const bool& folders);
    static void saveFile(const QString& filePath, const QByteArray& data);
    void saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile);
    static void deleteFile(const QString& filePath);
    void checkIfFilesExist(const int& requestId, const QStringList& files);

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
    ~FileAccessLocal() override;

    void getFile(const int& id, const QString& filePath) override;
    void getFiles(const int& id, const QString& directory, const QString& fileEnding) override;
    void getFileList(const int &requestId, const QString &directory, const bool &folders) override;
    void saveFile(const QString &filePath, const QByteArray &data) override;
    void saveFileDeleteOld(const QString &newFile, const QByteArray &data, const QString &oldFile) override;
    void deleteFile(const QString &filePath) override;
    void checkIfFilesExist(const int &id, QStringList files) override;

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
