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
    connect(this,          &FileAccessLocal::startRenamingFile,         finder, &FileFinder::renameFile);
    connect(this,          &FileAccessLocal::startRenamingFolder,       finder, &FileFinder::renameFolder);
    connect(this,          &FileAccessLocal::startDeletingFile,         finder, &FileFinder::deleteFile);
    connect(this,          &FileAccessLocal::startCheckingIfFilesExist, finder, &FileFinder::checkIfFilesExist);
    connect(this,          &FileAccessLocal::startCreatingFolder,       finder, &FileFinder::createFolder);

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
void FileAccessLocal::getFile(int id, const QString& filePath)
{
    emit startGettingFile(id, filePath);
}

/**
 * @brief Get the contents of all files in a directory
 */
void FileAccessLocal::getFiles(int id, const QString& directory, const QString& fileEnding)
{
    emit startGettingFiles(id, directory, fileEnding);
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
void FileAccessLocal::saveFile(const QString& filePath, const QByteArray& data)
{
    emit startSavingFile(filePath, data);
}

void FileAccessLocal::renameFile(const QString& newFile, const QString& oldFile, const QByteArray& data)
{
    emit startRenamingFile(newFile, oldFile, data);
}

void FileAccessLocal::renameFolder(const QString &newFolder, const QString &oldFolder)
{
    emit startRenamingFolder(newFolder, oldFolder);
}

/**
 * @brief Delete the file at filePath
 */
void FileAccessLocal::deleteFile(const QString& filePath)
{
    emit startDeletingFile(filePath);
}

void FileAccessLocal::checkIfFilesExist(int id, QStringList files)
{
    emit startCheckingIfFilesExist(id, files);
}

void FileAccessLocal::createFolder(const QString &folderPath)
{
    emit startCreatingFolder(folderPath);
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
        qCWarning(gmFileAccessLocal()) << "Could not save file" << filePath;
    }
}

void FileFinder::renameFile(const QString& newFile, const QString& oldFile, const QByteArray& data)
{
    // If no data is provided, simply rename the file,
    // else create a new file and delete the old
    if (data.isEmpty())
    {
        QFile f(oldFile);

        if (!f.rename(newFile))
        {
            qCDebug(gmFileAccessLocal()) << "Could not rename file, trying to save and delete ...";

            FileFinder::saveFileDeleteOld(newFile, oldFile, getFileData(oldFile));
        }
    }
    else
    {
        FileFinder::saveFileDeleteOld(newFile, oldFile, data);
    }
}

void FileFinder::renameFolder(const QString &newFolder, const QString &oldFolder)
{
    QDir d(oldFolder);

    if (!d.rename(oldFolder, newFolder))
    {
        qCWarning(gmFileAccessLocal()) << "Error: Could not rename" << oldFolder << "to" << newFolder;
    }
}

void FileFinder::saveFileDeleteOld(const QString &newFile, const QString &oldFile, const QByteArray &data)
{
    FileFinder::saveFile(newFile, data);
    FileFinder::deleteFile(oldFile);
}

void FileFinder::deleteFile(const QString& filePath)
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
            deleteFile(filePath + "/" + fileName);
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
}

void FileFinder::createFolder(const QString &folderPath)
{
    QDir d(folderPath);
    d.mkpath(folderPath);
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
