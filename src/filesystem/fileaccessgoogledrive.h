#pragma once

#include <QObject>
#include <QMap>
#include <QJsonArray>
#include <QNetworkRequest>
#include "fileaccess.h"
#include "cache/filecache.h"

namespace Files
{

class GoogleDriveMimeType
{
public:
    static constexpr const char* FOLDER = "application/vnd.google-apps.folder";
    static constexpr const char* FILE = "application/vnd.google-apps.file";
    static constexpr const char* UNKNOWN = "application/vnd.google-apps.unknown";
    static constexpr const char* AUDIO = "application/vnd.google-apps.audio";
    static constexpr const char* VIDEO = "application/vnd.google-apps.video";
    static constexpr const char* PHOTO = "application/vnd.google-apps.photo";
};

class FileAccessGoogleDrive : public FileAccess
{
    Q_OBJECT
public:
    explicit FileAccessGoogleDrive(QObject *parent) : FileAccess(parent) {}

    QFuture<FileDataResult*> getDataAsync(const QString &path, bool allowCache) override;
    QFuture<QVector<FileDataResult*>> getDataAsync(const QStringList& paths, bool allowCache) override;
    QFuture<FileResult*> saveAsync(const QString& path, const QByteArray& data) override;
    QFuture<FileResult*> moveAsync(const QString& oldPath, const QString& newPath) override;
    QFuture<FileResult*> deleteAsync(const QString& path) override;
    QFuture<FileResult*> copyAsync(const QString& path, const QString& copy) override;
    QFuture<FileListResult*> listAsync(const QString& path, bool files, bool folders) override;
    QFuture<FileResult*> createDirAsync(const QString& path) override;
    QFuture<FileCheckResult*> checkAsync(const QString& path, bool allowCache) override;
    QFuture<FileMultiCheckResult*> checkAsync(const QStringList& paths, bool allowCache) override;

private:
    FileCache m_fileCache;
    FileCache m_idCache;

    static constexpr const char* FILES_ENDPOINT = "https://www.googleapis.com/drive/v3/files";
    static constexpr const char* UPLOAD_ENDPOINT = "https://www.googleapis.com/upload/drive/v3/files";
    static constexpr const char* BATCH_ENDPOINT = "https://www.googleapis.com/batch/drive/v3";

    QFuture<FileResult*> createFileAsync(const QString& path, const QByteArray &mimeType);
    QFuture<FileResult*> updateFileContent(const QString &id, const QString &path, const QByteArray &data);

    QFuture<void> getFolderEntryIds(const QString &parentId, const QString &parentPath, const QString &pageToken = "");
    QFuture<void> getFolderEntryIds(const QPair<QString, QByteArray> &dir, const QString &pageToken = "");
    QFuture<QString> getFileIdAsync(const QString &path);
    QFuture<QPair<QString, QByteArray>> getParentIdAsync(const QString &path);

    static QByteArray makeMetaData(const QString &name, const QString &parentId = "", const QByteArray &mimeType = "");
    static QNetworkRequest makeJsonRequest(const QUrl &url);

    QFuture<FileListResult*> listAsync(const QString &path, const QString &q, const QString &pageToken = "");
    static QString makeListQuery(const QString &id, bool files, bool folders);
};

}
