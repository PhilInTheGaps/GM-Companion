#include "fileaccesslocal.h"
#include "logging.h"
#include "utils/fileutils.h"
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
    connect(this,          &FileAccessLocal::startRenamingFile,         finder, &FileFinder::renameFile);
    connect(this,          &FileAccessLocal::startRenamingFolder,       finder, &FileFinder::renameFolder);
    connect(this,          &FileAccessLocal::startDeletingFile,         finder, &FileFinder::deleteFile);
    connect(this,          &FileAccessLocal::startCheckingIfFilesExist, finder, &FileFinder::checkIfFilesExist);
    connect(this,          &FileAccessLocal::startCreatingFolder,       finder, &FileFinder::createFolder);

    connect(finder,        &FileFinder::receivedFile,                   this,   &FileAccessLocal::receivedFile);
    connect(finder,        &FileFinder::receivedFiles,                  this,   &FileAccessLocal::receivedFiles);
    connect(finder,        &FileFinder::receivedFileList,               this,   &FileAccessLocal::receivedFileList);
    connect(finder,        &FileFinder::savedFile,                      this,   &FileAccessLocal::savedFile);
    connect(finder,        &FileFinder::renamedFile,                    this,   &FileAccessLocal::renamedFile);
    connect(finder,        &FileFinder::renamedFolder,                  this,   &FileAccessLocal::renamedFolder);
    connect(finder,        &FileFinder::deletedFile,                    this,   &FileAccessLocal::deletedFile);
    connect(finder,        &FileFinder::createdFolder,                  this,   &FileAccessLocal::createdFolder);
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
void FileAccessLocal::getFile(int requestId, const QString& filePath)
{
    emit startGettingFile(requestId, filePath);
}

/**
 * @brief Get the contents of all files in a directory
 */
void FileAccessLocal::getFiles(int requestId, const QString& directory, const QString& fileEnding)
{
    emit startGettingFiles(requestId, directory, fileEnding);
}

/**
 * @brief Get a list of all files in a directory
 * @param folder Find folders? If true only only folders are found, else only
 * files.
 */
void FileAccessLocal::getFileList(int requestId, const QString& directory, bool folders)
{
    emit startGettingFileList(requestId, directory, folders);
}

/**
 * @brief Write data into file at filePath
 */
void FileAccessLocal::saveFile(int requestId, const QString& filePath, const QByteArray& data)
{
    emit startSavingFile(requestId, filePath, data);
}

void FileAccessLocal::renameFile(int requestId, const QString& newFile, const QString& oldFile, const QByteArray& data)
{
    emit startRenamingFile(requestId, newFile, oldFile, data);
}

void FileAccessLocal::renameFolder(int requestId, const QString &newFolder, const QString &oldFolder)
{
    emit startRenamingFolder(requestId, newFolder, oldFolder);
}

/**
 * @brief Delete the file at filePath
 */
void FileAccessLocal::deleteFile(int requestId, const QString& filePath)
{
    emit startDeletingFile(requestId, filePath);
}

void FileAccessLocal::checkIfFilesExist(int requestId, QStringList files)
{
    emit startCheckingIfFilesExist(requestId, files);
}

void FileAccessLocal::createFolder(int requestId, const QString &folderPath)
{
    emit startCreatingFolder(requestId, folderPath);
}

void FileFinder::getFile(int requestId, const QString& filePath)
{
    qCDebug(gmFileAccessLocal()) << "Getting file" << filePath;
    emit receivedFile(requestId, getFileData(filePath));
}

void FileFinder::getFiles(int requestId, const QString& directory, const QString& fileEnding)
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

void FileFinder::getFileList(int requestId, const QString& directory, const bool& folders)
{
    qCDebug(gmFileAccessLocal()) << "Getting a list of files in directory" << directory << "folders:" << folders;

    QDir dir(directory);

    dir.setFilter(folders ? QDir::Dirs | QDir::NoDotAndDotDot : QDir::Files);

    emit receivedFileList(requestId, dir.entryList());
}

void FileFinder::saveFile(int requestId, const QString& filePath, const QByteArray& data)
{
    // Check if the directory exists and if not, create it
    auto folder = FileUtils::dirFromPath(filePath);
    QDir dir(folder);

    if (!dir.exists())
    {
        dir.mkpath(folder);
    }

    QFile f(filePath);

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(data);
        f.close();
    }
    else
    {
        qCWarning(gmFileAccessLocal()) << "Could not save file" << filePath;
    }

    emit savedFile(requestId);
}

void FileFinder::renameFile(int requestId, const QString& newFile, const QString& oldFile, const QByteArray& data)
{
    // If no data is provided, simply rename the file,
    // else create a new file and delete the old
    if (data.isEmpty())
    {
        QFile f(oldFile);

        if (!f.rename(newFile))
        {
            qCDebug(gmFileAccessLocal()) << "Could not rename file, trying to save and delete ...";

            saveFileDeleteOld(requestId, newFile, oldFile, getFileData(oldFile));
        }
    }
    else
    {
        saveFileDeleteOld(requestId, newFile, oldFile, data);
    }
}

void FileFinder::renameFolder(int requestId, const QString &newFolder, const QString &oldFolder)
{
    QDir d(oldFolder);

    if (!d.rename(oldFolder, newFolder))
    {
        qCWarning(gmFileAccessLocal()) << "Error: Could not rename" << oldFolder << "to" << newFolder;
    }

    emit renamedFolder(requestId);
}

void FileFinder::saveFileDeleteOld(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data)
{
    FileFinder::saveFile(requestId, newFile, data);
    FileFinder::deleteFile(requestId, oldFile);
}

void FileFinder::deleteFile(int requestId, const QString& filePath)
{
    qCDebug(gmFileAccessLocal()) << "Deleting" << filePath;

    QFile f(filePath);
    QFileInfo fi(f);

    if (fi.isDir())
    {
        // Recursively delete all files in folder and then the folder itself
        QDir dir(filePath);

        for (const auto &fileName : dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries))
        {
            deleteFile(-1, filePath + "/" + fileName);
        }

        if (!dir.rmdir(filePath))
        {
            qCWarning(gmFileAccessLocal()) << "Could not delete folder" << filePath;
        }
    }
    else if (f.exists())
    {
        // Safety clause, don't delete file if it was modified recently
        if (fi.lastModified().secsTo(QDateTime::currentDateTime()) < 3)
        {
            qCWarning(gmFileAccessLocal()) << "Did not delete file as it was modified less than 3 seconds ago.";
            return;
        }

        if (!f.remove())
        {
            qCWarning(gmFileAccessLocal()) << "Could not delete file" << filePath;
        }

    }
    else
    {
        qCWarning(gmFileAccessLocal()) << "Could not delete file" << filePath << ", does not exist";
    }

    if (requestId > -1) emit deletedFile(requestId);
}

void FileFinder::createFolder(int requestId, const QString &folderPath)
{
    QDir d(folderPath);
    d.mkpath(folderPath);

    emit createdFolder(requestId);
}

void FileFinder::checkIfFilesExist(int requestId, const QStringList& files)
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

    qCWarning(gmFileAccessLocal()) << "Could not open file" << filePath << ". Error:" << file.error() << file.errorString();
    return QByteArray();
}
