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

    void getFile(const int& requestId, const QString& filePath) override;
    void getFiles(const int& requestId, const QString& directory, const QString& fileEnding) override;
    void getFileList(const int& requestId, const QString& directory, const bool& folders) override;
    void saveFile(const QString& filePath, const QByteArray& data) override;
    void saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile) override;
    void deleteFile(const QString& filePath) override;
    void checkIfFilesExist(const int& requestId, QStringList files) override;

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
