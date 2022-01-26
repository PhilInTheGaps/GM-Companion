#pragma once

#include <QDir>
#include "fileaccess.h"

namespace Files {

class FileAccessLocal : public FileAccess
{
    Q_OBJECT
public:
    explicit FileAccessLocal(QObject *parent) : FileAccess(parent) {}

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
    static FileDataResult* getData(const QString& path, bool allowCache);
    static FileResult* createDir(const QDir& dir);
    static FileResult* save(const QString& path, const QByteArray& data);
    static FileResult* move(const QString& oldPath, const QString& newPath);
    static FileResult* copy(const QString &path, const QString &copy);
    static QFlags<QDir::Filter> getDirFilter(bool files, bool folders);
    static FileCheckResult* check(const QString& path, bool allowCache);

};

}
