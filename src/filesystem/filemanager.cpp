#include "filemanager.h"
#include "settings/settingsmanager.h"
#include "fileaccessnextcloud.h"
#include "fileaccessgoogledrive.h"
#include "fileaccesslocal.h"
#include "logging.h"

bool FileManager::instanceFlag   = false;
FileManager *FileManager::single = nullptr;
int FileManager::fileAccessCount = 0;

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    updateFileAccess();
}

QString FileManager::cleanPath(const QString &path)
{
    auto result = path;
    result = result.replace("file://", "");
    return result;
}

FileManager::~FileManager()
{
    instanceFlag = false;
}

void FileManager::getFile(int requestId, const QString &filePath)
{
    m_fileAccess->getFile(requestId, cleanPath(filePath));
}

void FileManager::getFiles(int requestId, const QString &directory, const QString &fileEnding)
{
    m_fileAccess->getFiles(requestId, cleanPath(directory), fileEnding);
}

void FileManager::getFileList(int requestId, const QString &directory, bool folders)
{
    m_fileAccess->getFileList(requestId, cleanPath(directory), folders);
}

void FileManager::saveFile(const QString &filePath, const QByteArray &data)
{
    saveFile(getUniqueRequestId(), filePath, data);
}

void FileManager::saveFile(int requestId, const QString &filePath, const QByteArray &data)
{
    m_fileAccess->saveFile(requestId, cleanPath(filePath), data);
}

void FileManager::renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data)
{
    renameFile(getUniqueRequestId(), newFile, oldFile, data);
}

void FileManager::renameFile(int requestId, const QString &newFile, const QString &oldFile, const QByteArray &data)
{
    m_fileAccess->renameFile(requestId, cleanPath(newFile), cleanPath(oldFile), data);
}

void FileManager::renameFolder(const QString &newFolder, const QString &oldFolder)
{
    renameFolder(getUniqueRequestId(), newFolder, oldFolder);
}

void FileManager::renameFolder(int requestId, const QString &newFolder, const QString &oldFolder)
{
    m_fileAccess->renameFolder(requestId, cleanPath(newFolder), cleanPath(oldFolder));
}

void FileManager::deleteFile(const QString &filePath)
{
    deleteFile(getUniqueRequestId(), filePath);
}

void FileManager::deleteFile(int requestId, const QString &filePath)
{
    m_fileAccess->deleteFile(requestId, cleanPath(filePath));
}

void FileManager::checkIfFilesExist(int requestId, const QStringList &files)
{
    m_fileAccess->checkIfFilesExist(requestId, files);
}

void FileManager::createFolder(const QString &folderPath)
{
    createFolder(getUniqueRequestId(), folderPath);
}

void FileManager::createFolder(int requestId, const QString &folderPath)
{
    m_fileAccess->createFolder(requestId, cleanPath(folderPath));
}

auto FileManager::getUniqueRequestId()->int
{
    return ++fileAccessCount;
}

auto FileManager::getInstance()->FileManager *
{
    if (!instanceFlag)
    {
        single       = new FileManager;
        instanceFlag = true;
    }
    return single;
}

void FileManager::updateFileAccess()
{
    auto cloudMode = SettingsManager::getSetting("cloudMode", "local");

    if (cloudMode == "GoogleDrive")
    {
        m_fileAccess = new FileAccessGoogleDrive(this);
    }
    else if (cloudMode == "NextCloud")
    {
        m_fileAccess = new FileAccessNextCloud(this);
    }
    else
    {
        m_fileAccess = new FileAccessLocal(this);
    }

    connect(m_fileAccess, &FileAccess::receivedFile,        this, &FileManager::receivedFile);
    connect(m_fileAccess, &FileAccess::receivedFiles,       this, &FileManager::receivedFiles);
    connect(m_fileAccess, &FileAccess::receivedFileList,    this, &FileManager::receivedFileList);
    connect(m_fileAccess, &FileAccess::savedFile,           this, &FileManager::savedFile);
    connect(m_fileAccess, &FileAccess::renamedFile,         this, &FileManager::renamedFile);
    connect(m_fileAccess, &FileAccess::renamedFolder,       this, &FileManager::renamedFolder);
    connect(m_fileAccess, &FileAccess::deletedFile,         this, &FileManager::deletedFile);
    connect(m_fileAccess, &FileAccess::createdFolder,       this, &FileManager::createdFolder);
    connect(m_fileAccess, &FileAccess::checkedIfFilesExist, this, &FileManager::checkedIfFilesExist);
}
