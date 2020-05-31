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
    void getFileList(int requestId, const QString& directory, const bool& folders);
    void checkIfFilesExist(int requestId, const QStringList& files);

    static void saveFile(const QString &filePath, const QByteArray& data);
    static void renameFile(const QString &newFile, const QString& oldFile, const QByteArray& data = "");
    static void renameFolder(const QString &newFolder, const QString &oldFolder);
    static void deleteFile(const QString &filePath);
    static void createFolder(const QString &folderPath);

private:
    static QByteArray getFileData(const QString& filePath);
    static void saveFileDeleteOld(const QString& newFile, const QString& oldFile, const QByteArray& data);

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

    void getFile(int id, const QString &filePath) override;
    void getFiles(int id, const QString &directory, const QString &fileEnding) override;
    void getFileList(int requestId, const QString &directory, bool folders) override;
    void saveFile(const QString &filePath, const QByteArray &data) override;
    void renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data) override;
    void renameFolder(const QString &newFolder, const QString &oldFolder) override;
    void deleteFile(const QString &filePath) override;
    void checkIfFilesExist(int id, QStringList files) override;
    void createFolder(const QString &folderPath) override;

signals:
    void startGettingFile(int requestId, QString filePath);
    void startGettingFiles(int requestId, QString directory, QString fileEnding);
    void startGettingFileList(int requestId, QString directory, bool folders);
    void startSavingFile(const QString &filePath, const QByteArray &data);
    void startRenamingFile(const QString &newFile, const QString &oldFile, const QByteArray &data);
    void startRenamingFolder(const QString &newFolder, const QString& oldFolder);
    void startDeletingFile(const QString &filePath);
    void startCheckingIfFilesExist(int requestId, const QStringList &files);
    void startCreatingFolder(const QString &folderPath);
};

#endif // FILEACCESSLOCAL_H
