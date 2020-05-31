#ifndef FILEACCESSNEXTCLOUD_H
#define FILEACCESSNEXTCLOUD_H

#include "fileaccess.h"
#include "filemanager.h"
#include <QMap>
#include <QTemporaryFile>

class FileAccessNextCloud : public FileAccess
{
    Q_OBJECT
public:
    explicit FileAccessNextCloud(QObject *parent = nullptr);

    void getFile(int requestId, const QString &filePath) override;
    void getFiles(int requestId, const QString &directory, const QString &fileEnding) override;
    void getFileList(int requestId, const QString &directory, bool folders) override;

    void saveFile(const QString &filePath, const QByteArray &data) override;
    void renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data) override;
    void renameFolder(const QString &newFolder, const QString &oldFolder) override;
    void deleteFile(const QString &filePath) override;
    void checkIfFilesExist(int requestId, QStringList files) override;
    void createFolder(const QString &folderPath) override;

private:
    static QString fileNameFromHref(QString href);

    QMap<QString, QTemporaryFile*> m_fileCache;
    bool tryGetFileFromCache(const QString& filePath, QByteArray *data);
    void saveFileInCache(const QString& filePath, const QByteArray& data);
    void removeFileFromCache(const QString& filePath);

    void downloadFiles(const int &requestId, QMap<int, QString> *requests);

    QByteArray getXml(QString filepath) const;
    QByteArray getFileListXml() const;

};

#endif // FILEACCESSNEXTCLOUD_H
