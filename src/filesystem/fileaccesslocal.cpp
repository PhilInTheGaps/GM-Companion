#include "fileaccesslocal.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QtConcurrent/QtConcurrentRun>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

Q_LOGGING_CATEGORY(gmFileAccessLocal, "gm.files.access.local")

/// Read data from one file
auto FileAccessLocal::getData(const QString &path, bool allowCache) -> std::shared_ptr<FileDataResult>
{
    Q_UNUSED(allowCache)

    QFile f(path);

    if (f.open(QIODevice::ReadOnly))
    {
        auto data = f.readAll();
        f.close();
        return std::make_shared<FileDataResult>(data);
    }

    qCWarning(gmFileAccessLocal()) << "Warning: Could not find file" << path << f.error() << f.errorString();
    return std::make_shared<FileDataResult>(f.errorString());
}

/// Read data from one file async
auto FileAccessLocal::getDataAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileDataResult>>
{
    qCDebug(gmFileAccessLocal()) << "Getting data from file:" << path << "...";

    return QtConcurrent::run(&FileAccessLocal::getData, path, allowCache);
}

/// Read data from multiple files
auto FileAccessLocal::getDataAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>
{
    qCDebug(gmFileAccessLocal()) << "Getting data from multiple files:" << paths << "...";

    return QtConcurrent::run([paths, allowCache]() {
        std::vector<std::shared_ptr<FileDataResult>> results;
        results.reserve(paths.size());

        foreach (const auto &path, paths)
        {
            results.push_back(getData(path, allowCache));
        }

        return results;
    });
}

/// Create a directory
auto FileAccessLocal::createDir(const QDir &dir) -> std::shared_ptr<FileResult>
{
    if (dir.exists())
    {
        auto message = u"The directory %1 already exists ..."_s.arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return std::make_shared<FileResult>(message);
    }

    if (!dir.mkpath(dir.path()))
    {
        auto message = u"Could not create directory %1."_s.arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return std::make_shared<FileResult>(message);
    }
    return std::make_shared<FileResult>(true);
}

/// Create a directory
auto FileAccessLocal::createDirAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessLocal()) << "Creating directory:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::createDir, QDir(path));
}

/// Save data to a file
auto FileAccessLocal::save(const QString &path, const QByteArray &data) -> std::shared_ptr<FileResult>
{
    QFile f(path);
    QString errorMessage;
    bool status = false;

    if (QFileInfo info(f); !info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            errorMessage = u"Could not save file %1: %2"_s.arg(path, createDirResult->errorMessage());
            return std::make_shared<FileResult>(false, errorMessage);
        }
    }

    if (f.open(QIODevice::WriteOnly))
    {
        if (f.write(data) == -1)
        {
            errorMessage =
                u"Could not write to file %1: %2 %3"_s.arg(path, QString::number(f.error()), f.errorString());
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
        errorMessage =
            u"Could not open file for saving %1: %2 %3"_s.arg(path, QString::number(f.error()), f.errorString());
        status = false;
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
    }

    return std::make_shared<FileResult>(status, errorMessage);
}

/// Save data to a file async
auto FileAccessLocal::saveAsync(const QString &path, const QByteArray &data) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessLocal()) << "Saving file:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::save, path, data);
}

auto FileAccessLocal::move(const QString &oldPath, const QString &newPath) -> std::shared_ptr<FileResult>
{
    QFile f(oldPath);
    QFileInfo info(newPath);

    if (!info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            auto errorMessage = u"Could not move %1 to %2: %3"_s.arg(oldPath, newPath, createDirResult->errorMessage());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return std::make_shared<FileResult>(false, errorMessage);
        }
    }

    if (!f.rename(newPath))
    {
        auto errorMessage =
            u"Could not move %1 to %2: %3 %4"_s.arg(oldPath, newPath, QString::number(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return std::make_shared<FileResult>(false, errorMessage);
    }

    return std::make_shared<FileResult>(true);
}

/// Move file from oldPath to newPath
auto FileAccessLocal::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessLocal()) << "Moving file:" << oldPath << "->" << newPath << "...";
    return QtConcurrent::run(&FileAccessLocal::move, oldPath, newPath);
}

/// Delete the file or folder at path
auto FileAccessLocal::deleteAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessLocal()) << "Deleting file:" << path << "...";
    return QtConcurrent::run([path]() {
        QFileInfo info(path);

        if (!info.exists())
            return std::make_shared<FileResult>(false, u"The file or folder %1 does not exist"_s.arg(path));

        if (QDir dir(path); info.isDir() && !dir.removeRecursively())
        {
            auto errorMessage = u"Could not delete folder %1"_s.arg(path);
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return std::make_shared<FileResult>(false, errorMessage);
        }
        else if (QFile f(path); info.isFile() && !f.remove())
        {
            auto errorMessage =
                u"Could not delete file %1: %2 %3"_s.arg(path, QString::number(f.error()), f.errorString());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return std::make_shared<FileResult>(false, errorMessage);
        }

        return std::make_shared<FileResult>(true);
    });
}

auto FileAccessLocal::copy(const QString &path, const QString &copy) -> std::shared_ptr<FileResult>
{
    QFile f(path);

    if (QFileInfo info(copy); !info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult->success())
        {
            auto errorMessage = u"Could not copy %1 to %2: %3"_s.arg(path, copy, createDirResult->errorMessage());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return std::make_shared<FileResult>(false, errorMessage);
        }
    }

    if (!f.copy(copy))
    {
        auto errorMessage =
            u"Could not copy %1 to %2: %3 %4"_s.arg(path, copy, QString::number(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return std::make_shared<FileResult>(false, errorMessage);
    }

    return std::make_shared<FileResult>(true);
}

/// Copy a file at path to a new path
auto FileAccessLocal::copyAsync(const QString &path, const QString &copy) -> QFuture<std::shared_ptr<FileResult>>
{
    qCDebug(gmFileAccessLocal()) << "Copying file:" << path << "->" << copy << "...";
    return QtConcurrent::run(&FileAccessLocal::copy, path, copy);
}

/// List files or/and folders in a directory
auto FileAccessLocal::listAsync(const QString &path, bool files, bool folders)
    -> QFuture<std::shared_ptr<FileListResult>>
{
    return QtConcurrent::run([path, files, folders]() {
        QDir dir(path);
        return std::make_shared<FileListResult>(path, dir.entryList(getDirFilter(false, folders)),
                                                dir.entryList(getDirFilter(files, false)));
    });
}

/// Check if a file exists
auto FileAccessLocal::check(const QString &path, bool allowCache) -> std::shared_ptr<FileCheckResult>
{
    Q_UNUSED(allowCache)

    QFile f(path);
    return std::make_shared<FileCheckResult>(path, f.exists());
}

/// Check if a file exists async
auto FileAccessLocal::checkAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileCheckResult>>
{
    return QtConcurrent::run(&FileAccessLocal::check, path, allowCache);
}

/// Check which files exist
auto FileAccessLocal::checkAsync(const QStringList &paths, bool allowCache)
    -> QFuture<std::shared_ptr<FileMultiCheckResult>>
{
    return QtConcurrent::run([paths, allowCache]() {
        auto result = std::make_shared<FileMultiCheckResult>(true);
        for (const auto &path : paths)
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
