#include "fileaccesslocal.h"
#include "logging.h"
#include <QDateTime>
#include <QDir>
#include <QFile>

FileAccessLocal::FileAccessLocal(QObject *parent) : FileAccess(parent)
{
    auto *finder = new FileFinder;

    connect(&workerThread, &QThread::finished,                          finder, &FileFinder::deleteLater);

    connect(this,          &FileAccessLocal::startGettingFile,          finder, &FileFinder::getFile);
    connect(this,          &FileAccessLocal::startGettingFiles,         finder, &FileFinder::getFiles);
    connect(this,          &FileAccessLocal::startGettingFileList,      finder, &FileFinder::getFileList);
    connect(this,          &FileAccessLocal::startSavingFile,           finder, &FileFinder::saveFile);
    connect(this,          &FileAccessLocal::startSavingFileDeleteOld,  finder, &FileFinder::saveFileDeleteOld);
    connect(this,          &FileAccessLocal::startDeletingFile,         finder, &FileFinder::deleteFile);
    connect(this,          &FileAccessLocal::startCheckingIfFilesExist, finder, &FileFinder::checkIfFilesExist);

    connect(finder,        &FileFinder::receivedFile,                   this,   &FileAccessLocal::receivedFile);
    connect(finder,        &FileFinder::receivedFiles,                  this,   &FileAccessLocal::receivedFiles);
    connect(finder,        &FileFinder::receivedFileList,               this,   &FileAccessLocal::receivedFileList);
    connect(finder,        &FileFinder::checkedIfFilesExist,            this,   &FileAccessLocal::checkedIfFilesExist);

    finder->moveToThread(&workerThread);
    workerThread.start();
}

FileAccessLocal::~FileAccessLocal()
{
    workerThread.quit();
    workerThread.wait();
}

/**
 * @brief Get the content of a single file located at filePath
 */
void FileAccessLocal::getFile(const int& id, const QString& filePath)
{
    emit startGettingFile(id, filePath);
}

/**
 * @brief Get the contents of all files in a directory
 */
void FileAccessLocal::getFiles(const int& id, const QString& directory, const QString& fileEnding)
{
    emit startGettingFiles(id, directory, fileEnding);
}

/**
 * @brief Get a list of all files in a directory
 * @param folder Find folders? If true only only folders are found, else only
 * files.
 */
void FileAccessLocal::getFileList(const int& requestId, const QString& directory, const bool& folders)
{
    emit startGettingFileList(requestId, directory, folders);
}

/**
 * @brief Write data into file at filePath
 */
void FileAccessLocal::saveFile(const QString& filePath, const QByteArray& data)
{
    emit startSavingFile(filePath, data);
}

void FileAccessLocal::saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile)
{
    emit startSavingFileDeleteOld(newFile, data, oldFile);
}

/**
 * @brief Delete the file at filePath
 */
void FileAccessLocal::deleteFile(const QString& filePath)
{
    emit startDeletingFile(filePath);
}

void FileAccessLocal::checkIfFilesExist(const int& id, QStringList files)
{
    emit startCheckingIfFilesExist(id, files);
}

void FileFinder::getFile(const int& requestId, const QString& filePath)
{
    qCDebug(gmFileAccessLocal()) << "Getting file" << filePath;
    emit receivedFile(requestId, getFileData(filePath));
}

void FileFinder::getFiles(const int& requestId, const QString& directory, const QString& fileEnding)
{
    qCDebug(gmFileAccessLocal()) << "Getting files in directory" << directory << "with fileEnding filter:" << fileEnding;

    QDir dir(directory);

    dir.setFilter(QDir::Files);

    if (!fileEnding.isEmpty()) dir.setNameFilters({ fileEnding });

    QList<QByteArray> data;

    for (const auto& fileName : dir.entryList())
    {
        data.append(getFileData(directory + "/" + fileName));
    }

    emit receivedFiles(requestId, data);
}

void FileFinder::getFileList(const int& requestId, const QString& directory, const bool& folders)
{
    qCDebug(gmFileAccessLocal()) << "Getting a list of files in directory" << directory << "folders:" << folders;

    QDir dir(directory);

    dir.setFilter(folders ? QDir::Dirs | QDir::NoDotAndDotDot : QDir::Files);

    emit receivedFileList(requestId, dir.entryList());
}

void FileFinder::saveFile(const QString& filePath, const QByteArray& data)
{
    QFile f(filePath);

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(data);
        f.close();
    }
    else
    {
        qCCritical(gmFileAccessLocal()) << "Could not save file" << filePath;
    }
}

void FileFinder::saveFileDeleteOld(const QString& newFile, const QByteArray& data, const QString& oldFile)
{
    saveFile(newFile, data);

    QFile f(oldFile);
    QFileInfo fi(f);

    // Safety clause, don't delete file if it was modified recently
    if (f.exists() && (fi.lastModified().secsTo(QDateTime::currentDateTime()) > 3))
    {
        if (!f.remove())
        {
            qCCritical(gmFileAccessLocal()) << "Could not delete old file" << oldFile;
        }
    }
}

void FileFinder::deleteFile(const QString& filePath)
{
    QFile f(filePath);

    if (f.exists())
    {
        if (!f.remove())
        {
            qCCritical(gmFileAccessLocal()) << "Could not delete file" << filePath;
        }
    }
    else
    {
        qCCritical(gmFileAccessLocal()) << "Could not delete file" << filePath << ", does not exist";
    }
}

void FileFinder::checkIfFilesExist(const int& requestId, const QStringList& files)
{
    qCDebug(gmFileAccessLocal()) << "Started FileFinder for" << files.count() << "files";

    QStringList found;
    QStringList notFound;

    for (const auto& filePath : files)
    {
        if (QFile(filePath).exists())
        {
            found.append(filePath);
        }
        else
        {
            notFound.append(filePath);
        }
    }

    emit checkedIfFilesExist(requestId, found, notFound);
}

/**
 * @brief Get the content of the file at filePath
 */
auto FileFinder::getFileData(const QString& filePath)->QByteArray
{
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly)) {
        auto data = file.readAll();
        file.close();
        return data;
    }

    qCCritical(gmFileAccessLocal()) << "Could not open file" << filePath << ". Error:" << file.error() << file.errorString();
    return QByteArray();
}
