#include "filemanager.h"

#include <utility>
#include "settings/settingsmanager.h"
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

void FileManager::getFile(int requestId, QString filePath)
{
    m_fileAccess->getFile(requestId, std::move(filePath));
}

void FileManager::getFiles(int requestId, QString directory, QString fileEnding)
{
    m_fileAccess->getFiles(requestId, std::move(directory), std::move(fileEnding));
}

void FileManager::getFileList(int requestId, QString directory, bool folders)
{
    m_fileAccess->getFileList(requestId, std::move(directory), folders);
}

void FileManager::saveFile(QString filePath, QByteArray data)
{
    m_fileAccess->saveFile(std::move(filePath), std::move(data));
}

void FileManager::saveFileDeleteOld(QString newFile, QByteArray data, QString oldFile)
{
    m_fileAccess->saveFileDeleteOld(std::move(newFile), std::move(data), std::move(oldFile));
}

void FileManager::deleteFile(QString filePath)
{
    m_fileAccess->deleteFile(std::move(filePath));
}

void FileManager::checkIfFilesExist(int requestId, QStringList files)
{
    m_fileAccess->checkIfFilesExist(requestId, std::move(files));
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
        // TODO
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
