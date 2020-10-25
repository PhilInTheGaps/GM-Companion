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

    void saveFile(int requestId, const QString &filePath, const QByteArray& data);
    void renameFile(int requestId, const QString &newFile, const QString& oldFile, const QByteArray& data = "");
    void renameFolder(int requestId, const QString &newFolder, const QString &oldFolder);
    void deleteFile(int requestId, const QString &filePath);
    void createFolder(int requestId, const QString &folderPath);

private:
    static QByteArray getFileData(const QString& filePath);
    void saveFileDeleteOld(int requestId, const QString& newFile, const QString& oldFile, const QByteArray& data);

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

class FileAccessLocal : public FileAccess
{
    Q_OBJECT
    QThread workerThread;

public:
    FileAccessLocal(QObject *parent = nullptr);
    ~FileAccessLocal() override;

    void getFile(int requestId, const QString &filePath) override;
    void getFiles(int requestId, const QString &directory, const QString &fileEnding) override;
    void getFileList(int requestId, const QString &directory, bool folders) override;
    void saveFile(int requestId, const QString &filePath, const QByteArray &data) override;
    void renameFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data) override;
    void renameFolder(int requestId, const QString &newFolder, const QString &oldFolder) override;
    void deleteFile(int requestId, const QString &filePath) override;
    void checkIfFilesExist(int requestId, QStringList files) override;
    void createFolder(int requestId, const QString &folderPath) override;

signals:
    void startGettingFile(int requestId, const QString &filePath);
    void startGettingFiles(int requestId, const QString &directory, const QString &fileEnding);
    void startGettingFileList(int requestId, QString directory, bool folders);
    void startSavingFile(int requestId, const QString &filePath, const QByteArray &data);
    void startRenamingFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data);
    void startRenamingFolder(int requestId, const QString &newFolder, const QString& oldFolder);
    void startDeletingFile(int requestId, const QString &filePath);
    void startCheckingIfFilesExist(int requestId, const QStringList &files);
    void startCreatingFolder(int requestId, const QString &folderPath);
};

#endif // FILEACCESSLOCAL_H
