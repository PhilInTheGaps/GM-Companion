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

FileManager::~FileManager()
{
    instanceFlag = false;
}

void FileManager::getFile(int requestId, const QString &filePath)
{
    m_fileAccess->getFile(requestId, filePath);
}

void FileManager::getFiles(int requestId, const QString &directory, const QString &fileEnding)
{
    m_fileAccess->getFiles(requestId, directory, fileEnding);
}

void FileManager::getFileList(int requestId, const QString &directory, bool folders)
{
    m_fileAccess->getFileList(requestId, std::move(directory), folders);
}

void FileManager::saveFile(const QString filePath, const QByteArray &data)
{
    m_fileAccess->saveFile(filePath, data);
}

void FileManager::renameFile(const QString &newFile, const QString &oldFile, const QByteArray &data)
{
    m_fileAccess->renameFile(newFile, oldFile, data);
}

void FileManager::renameFolder(const QString &newFolder, const QString &oldFolder)
{
    m_fileAccess->renameFolder(newFolder, oldFolder);
}

void FileManager::deleteFile(const QString &filePath)
{
    m_fileAccess->deleteFile(filePath);
}

void FileManager::checkIfFilesExist(int requestId, const QStringList &files)
{
    m_fileAccess->checkIfFilesExist(requestId, files);
}

void FileManager::createFolder(const QString &folderPath)
{
    m_fileAccess->createFolder(folderPath);
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
        m_fileAccess = new FileAccessGoogleDrive;
    }
    else if (cloudMode == "NextCloud")
    {
        m_fileAccess = new FileAccessNextCloud;
    }
    else
    {
        m_fileAccess = new FileAccessLocal;
    }

    connect(m_fileAccess, &FileAccess::receivedFile,        this, &FileManager::receivedFile);
    connect(m_fileAccess, &FileAccess::receivedFiles,       this, &FileManager::receivedFiles);
    connect(m_fileAccess, &FileAccess::receivedFileList,    this, &FileManager::receivedFileList);
    connect(m_fileAccess, &FileAccess::checkedIfFilesExist, this, &FileManager::checkedIfFilesExist);
}
