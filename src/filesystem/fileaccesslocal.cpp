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
auto FileAccessLocal::getData(const QString &path) -> FileDataResult
{
    QFile f(path);

    if (f.open(QIODevice::ReadOnly))
    {
        auto data = f.readAll();
        f.close();
        return FileDataResult(std::move(data));
    }

    qCWarning(gmFileAccessLocal()) << "Warning: Could not find file" << path << f.error() << f.errorString();
    return FileDataResult(f.errorString());
}

/// Read data from one file async
auto FileAccessLocal::getDataAsync(const QString &path, Options options) -> QFuture<FileDataResult>
{
    Q_UNUSED(options)

    qCDebug(gmFileAccessLocal()) << "Getting data from file:" << path << "...";

    return QtConcurrent::run(&FileAccessLocal::getData, path).then(&m_context, [](FileDataResult &&result) {
        return std::move(result);
    });
}

/// Read data from multiple files
auto FileAccessLocal::getDataAsync(const QStringList &paths, Options options) -> QFuture<std::vector<FileDataResult>>
{
    Q_UNUSED(options)

    qCDebug(gmFileAccessLocal()) << "Getting data from multiple files:" << paths << "...";

    return QtConcurrent::run([paths]() {
               std::vector<FileDataResult> results;
               results.reserve(paths.size());

               foreach (const auto &path, paths)
               {
                   results.push_back(getData(path));
               }

               return results;
           })
        .then(&m_context, [](std::vector<FileDataResult> &&result) { return std::move(result); });
}

/// Create a directory
auto FileAccessLocal::createDir(const QDir &dir) -> FileResult
{
    if (dir.exists())
    {
        auto message = u"The directory %1 already exists ..."_s.arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return FileResult(std::move(message));
    }

    if (!dir.mkpath(dir.path()))
    {
        auto message = u"Could not create directory %1."_s.arg(dir.path());
        qCWarning(gmFileAccessLocal()) << message;
        return FileResult(std::move(message));
    }
    return FileResult(true);
}

/// Create a directory
auto FileAccessLocal::createDirAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessLocal()) << "Creating directory:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::createDir, QDir(path)).then(&m_context, [](FileResult &&result) {
        return std::move(result);
    });
}

/// Save data to a file
auto FileAccessLocal::save(const QString &path, const QByteArray &data) -> FileResult
{
    QFile f(path);
    QString errorMessage;
    bool status = false;

    if (QFileInfo const info(f); !info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult.success())
        {
            errorMessage = u"Could not save file %1: %2"_s.arg(path, createDirResult.errorMessage());
            return FileResult(std::move(errorMessage));
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

    return FileResult(status, errorMessage);
}

/// Save data to a file async
auto FileAccessLocal::saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessLocal()) << "Saving file:" << path << "...";
    return QtConcurrent::run(&FileAccessLocal::save, path, data).then(&m_context, [](FileResult &&result) {
        return std::move(result);
    });
}

auto FileAccessLocal::move(const QString &oldPath, const QString &newPath) -> FileResult
{
    if (QFileInfo const info(newPath); !info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult.success())
        {
            auto errorMessage = u"Could not move %1 to %2: %3"_s.arg(oldPath, newPath, createDirResult.errorMessage());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return FileResult(false, errorMessage);
        }
    }

    if (QFile f(oldPath); !f.rename(newPath))
    {
        auto errorMessage =
            u"Could not move %1 to %2: %3 %4"_s.arg(oldPath, newPath, QString::number(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return FileResult(false, errorMessage);
    }

    return FileResult(true);
}

/// Move file from oldPath to newPath
auto FileAccessLocal::moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessLocal()) << "Moving file:" << oldPath << "->" << newPath << "...";
    return QtConcurrent::run(&FileAccessLocal::move, oldPath, newPath).then(&m_context, [](FileResult &&result) {
        return std::move(result);
    });
}

/// Delete the file or folder at path
auto FileAccessLocal::deleteAsync(const QString &path) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessLocal()) << "Deleting file:" << path << "...";
    return QtConcurrent::run([path]() {
               QFileInfo const info(path);

               if (!info.exists()) return FileResult(false, u"The file or folder %1 does not exist"_s.arg(path));

               if (QDir dir(path); info.isDir() && !dir.removeRecursively())
               {
                   auto errorMessage = u"Could not delete folder %1"_s.arg(path);
                   qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
                   return FileResult(false, errorMessage);
               }

               if (QFile f(path); info.isFile() && !f.remove())
               {
                   auto errorMessage =
                       u"Could not delete file %1: %2 %3"_s.arg(path, QString::number(f.error()), f.errorString());
                   qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
                   return FileResult(false, errorMessage);
               }

               return FileResult(true);
           })
        .then(&m_context, [](FileResult &&result) { return std::move(result); });
}

auto FileAccessLocal::copy(const QString &path, const QString &copy) -> FileResult
{
    QFile f(path);

    if (QFileInfo const info(copy); !info.dir().exists())
    {
        auto createDirResult = createDir(info.dir());
        if (!createDirResult.success())
        {
            auto errorMessage = u"Could not copy %1 to %2: %3"_s.arg(path, copy, createDirResult.errorMessage());
            qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
            return FileResult(false, errorMessage);
        }
    }

    if (!f.copy(copy))
    {
        auto errorMessage =
            u"Could not copy %1 to %2: %3 %4"_s.arg(path, copy, QString::number(f.error()), f.errorString());
        qCWarning(gmFileAccessLocal()) << "Warning:" << errorMessage;
        return FileResult(false, errorMessage);
    }

    return FileResult(true);
}

/// Copy a file at path to a new path
auto FileAccessLocal::copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult>
{
    qCDebug(gmFileAccessLocal()) << "Copying file:" << path << "->" << copy << "...";
    return QtConcurrent::run(&FileAccessLocal::copy, path, copy).then(&m_context, [](FileResult &&result) {
        return std::move(result);
    });
}

/// List files or/and folders in a directory
auto FileAccessLocal::listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult>
{
    return QtConcurrent::run([path, files, folders]() {
               QDir const dir(path);
               return FileListResult(path, dir.entryList(getDirFilter(false, folders)),
                                     dir.entryList(getDirFilter(files, false)));
           })
        .then(&m_context, [](FileListResult &&result) { return std::move(result); });
}

/// Check if a file exists
auto FileAccessLocal::check(const QString &path) -> FileCheckResult
{
    QFile const f(path);
    return FileCheckResult(path, f.exists());
}

/// Check if a file exists async
auto FileAccessLocal::checkAsync(const QString &path, Options options) -> QFuture<FileCheckResult>
{
    Q_UNUSED(options)

    return QtConcurrent::run(&FileAccessLocal::check, path).then(&m_context, [](FileCheckResult &&result) {
        return std::move(result);
    });
}

/// Check which files exist
auto FileAccessLocal::checkAsync(const QStringList &paths, Options options) -> QFuture<FileMultiCheckResult>
{
    Q_UNUSED(options)

    return QtConcurrent::run([paths]() {
               FileMultiCheckResult result(true);
               for (const auto &path : paths)
               {
                   result.add(check(path));
               }
               return result;
           })
        .then(&m_context, [](FileMultiCheckResult &&result) { return std::move(result); });
}

auto FileAccessLocal::getHomeDir() -> QString
{
    return QDir::homePath();
}

auto FileAccessLocal::getDirFilter(bool files, bool folders) -> QFlags<QDir::Filter>
{
    if (files && folders) return QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden;

    if (files) return QDir::NoDotAndDotDot | QDir::Files | QDir::Hidden;

    if (folders) return QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Drives | QDir::Hidden;

    return QDir::NoDotAndDotDot;
}
