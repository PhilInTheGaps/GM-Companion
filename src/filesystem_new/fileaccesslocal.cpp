#include "fileaccesslocal.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "logging.h"

using namespace Files;

/// Read data from one file
auto FileAccessLocal::getData(const QString &path, bool allowCache) -> FileDataResult*
{
    Q_UNUSED(allowCache);

    QFile f(path);

    if (f.open(QIODevice::ReadOnly))
    {
        auto data = f.readAll();
        f.close();
        return new FileDataResult(data);
    }

    qCWarning(gmFileAccessLocal()) << "Warning: Could not find file" << path << f.error() << f.errorString();
    return new FileDataResult(f.errorString());
}

/// Read data from one file async
auto FileAccessLocal::getDataAsync(const QString& path, bool allowCache) -> QFuture<FileDataResult*>
{
    qCDebug(gmFileAccessLocal()) << "Getting data from file:" << path << "...";

    return QtConcurrent::run(&FileAccessLocal::getData, path, allowCache);
}

/// Read data from multiple files
auto FileAccessLocal::getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<QVector<FileDataResult *>>
{
    qCDebug(gmFileAccessLocal()) << "Getting data from multiple files:" << paths << "...";

    return QtConcurrent::run([paths, allowCache]() {
        QVector<FileDataResult*> results;

        for (const auto& path : paths)
        {
            results.append(getData(path, allowCache));
        }

        return results;
    });
}

/// Create a directory
auto FileAccessLocal::createDir(const QDir &dir) -> FileResult*
{
    if (dir.exists())
    {
        auto message = QString("The directory %1 already exists ...").arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return new FileResult(message);
    }

    if (!dir.mkpath(dir.path()))
    {
        auto message = QString("Could not create directory %1.").arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return new FileResult(message);
    }
    return new FileResult(true);
}

/// Create a directory
auto FileAccessLocal::createDirAsync(const QString &path) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessLocal()) << "Creating directory:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::createDir, QDir(path));
}

/// Save data to a file
auto FileAccessLocal::save(const QString &path, const QByteArray &data) -> FileResult*
{
    QFile f(path);
    QFileInfo info(f);
    QString errorMessage;
    bool status = false;

    if (!info.dir().exists())
    {
        auto *createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            errorMessage = QString("Could not save file %1: %2")
                    .arg(path, createDirResult->errorMessage());
            createDirResult->deleteLater();
            return new FileResult(false, errorMessage);
        }
        createDirResult->deleteLater();
    }

    if (f.open(QIODevice::WriteOnly))
    {
        if (f.write(data) == -1)
        {
            errorMessage = QString("Could not write to file %1: %2 %3")
                    .arg(path, QString(f.error()), f.errorString());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        }
        else
        {
            status = true;
        }

        f.close();
    }
    else
    {
        errorMessage = QString("Could not open file for saving %1: %2 %3")
                .arg(path, QString(f.error()), f.errorString());
        status = false;
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
    }

    return new FileResult(status, errorMessage);
}

/// Save data to a file async
auto FileAccessLocal::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessLocal()) << "Saving file:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::save, path, data);
}

auto FileAccessLocal::move(const QString &oldPath, const QString &newPath) -> FileResult*
{
    QFile f(oldPath);
    QFileInfo info(newPath);

    if (!info.dir().exists())
    {
        auto *createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            auto errorMessage = QString("Could not move %1 to %2: %3")
                    .arg(oldPath, newPath, createDirResult->errorMessage());
            qCWarning(gmFileAccessNextCloud()) << "Warning:" << errorMessage;
            createDirResult->deleteLater();
            return new FileResult(false, errorMessage);
        }
        createDirResult->deleteLater();
    }

    if (!f.rename(newPath))
    {
        auto errorMessage = QString("Could not move %1 to %2: %3 %4")
                .arg(oldPath, newPath, QString(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return new FileResult(false, errorMessage);
    }

    return new FileResult(true);
}

/// Move file from oldPath to newPath
auto FileAccessLocal::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessLocal()) << "Moving file:" << oldPath << "->" << newPath << "...";
    return QtConcurrent::run(&FileAccessLocal::move, oldPath, newPath);
}

/// Delete the file or folder at path
auto FileAccessLocal::deleteAsync(const QString &path) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessLocal()) << "Deleting file:" << path << "...";
    return QtConcurrent::run([path]() {
        QFile f(path);

        if (!f.remove())
        {
            auto errorMessage = QString("Could not delete file/folder %1: %2 %3")
                    .arg(path, QString(f.error()), f.errorString());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return new FileResult(false, errorMessage);
        }

        return new FileResult(true);
    });
}

auto FileAccessLocal::copy(const QString &path, const QString &copy) -> FileResult*
{
    QFile f(path);
    QFileInfo info(copy);

    if (!info.dir().exists())
    {
        auto *createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            auto errorMessage = QString("Could not copy %1 to %2: %3")
                    .arg(path, copy, createDirResult->errorMessage());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            createDirResult->deleteLater();
            return new FileResult(false, errorMessage);
        }
        createDirResult->deleteLater();
    }

    if (!f.copy(copy))
    {
        auto errorMessage = QString("Could not copy %1 to %2: %3 %4")
                .arg(path, copy, QString(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return new FileResult(false, errorMessage);
    }

    return new FileResult(true);
}

/// Copy a file at path to a new path
auto FileAccessLocal::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult*>
{
    qCDebug(gmFileAccessLocal()) << "Copying file:" << path << "->" << copy << "...";
    return QtConcurrent::run(&FileAccessLocal::copy, path, copy);
}

/// List files or/and folders in a directory
auto FileAccessLocal::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult*>
{
    return QtConcurrent::run([path, files, folders]() {
        QDir dir(path);
        return new FileListResult(
                    dir.entryList(getDirFilter(false, folders)),
                    dir.entryList(getDirFilter(files, false)));
    });
}

/// Check if a file exists
auto FileAccessLocal::check(const QString &path, bool allowCache) -> FileCheckResult*
{
    Q_UNUSED(allowCache);

    QFile f(path);
    return new FileCheckResult(path, f.exists());
}

/// Check if a file exists async
auto FileAccessLocal::checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult*>
{
    return QtConcurrent::run(&FileAccessLocal::check, path, allowCache);
}

/// Check which files exist
auto FileAccessLocal::checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult*>
{
    return QtConcurrent::run([paths, allowCache]() {
        auto *result = new FileMultiCheckResult(true);
        for (const auto& path : paths)
        {
            result->add(check(path, allowCache));
        }
        return result;
    });
}

auto FileAccessLocal::getDirFilter(bool files, bool folders) -> QFlags<QDir::Filter>
{
    if (files && folders) return QDir::NoDotAndDotDot | QDir::AllEntries;

    if (files) return QDir::NoDotAndDotDot | QDir::Files;

    if (folders) return QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Drives;

    return QDir::NoDotAndDotDot;
}
