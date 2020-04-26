#include "filedialog.h"
#include "logging.h"
#include "filemanager.h"

FileObject::FileObject(QString name, bool isFolder, QObject *parent)
    : QObject(parent), m_name(name), m_isFolder(isFolder)
{
}

FileDialog::FileDialog(QObject *parent) : QObject(parent)
{
    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &FileDialog::onFileListReceived);
}

void FileDialog::setCurrentDir(const QString& dir)
{
    qCDebug(gmFileDialog()) << "Setting current dir:" << dir;
    m_currentDir = dir.split('/');
    emit currentDirChanged();

    clearForward();
    updateFileList();
}

void FileDialog::enterFolder(int index)
{
    qCDebug(gmFileDialog()) << "Entering folder" << index;

    if ((m_files.length() > index) && (index > -1))
    {
        auto *folder = static_cast<FileObject*>(m_files[index]);

        if (folder->isFolder())
        {
            m_currentDir.append(folder->name());
            emit currentDirChanged();
            updateFileList();
        }
    }

    m_forwardFolders.clear();
    emit canForwardChanged();
}

auto FileDialog::getSelected(int index) const -> QString
{
    auto selectedFolder = m_currentDir;

    if ((m_files.length() > index) && (index > -1))
    {
        selectedFolder.append(static_cast<FileObject*>(m_files[index])->name());
    }

    return FileUtils::dirFromFolders(selectedFolder);
}

void FileDialog::forward()
{
    qCDebug(gmFileDialog()) << "forward()";

    if (m_forwardFolders.length() > 0)
    {
        m_currentDir.push_back(m_forwardFolders.takeFirst());
        emit currentDirChanged();
        emit canForwardChanged();
        updateFileList();
    }
}

void FileDialog::clearForward()
{
    m_forwardFolders.clear();
    emit canForwardChanged();
}

void FileDialog::back()
{
    qCDebug(gmFileDialog()) << "back()";

    if (m_currentDir.length() > 1)
    {
        m_forwardFolders.push_front(m_currentDir.takeLast());
        emit canForwardChanged();
        emit currentDirChanged();
        updateFileList();
    }
}

void FileDialog::updateFileList()
{
    qCDebug(gmFileDialog()) << "updateFileList()";

    m_files.clear();
    emit filesChanged();

    m_foldersRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFileList(m_foldersRequestId, currentDir(), true);

    if (!m_folderMode)
    {
        m_filesRequestId = FileManager::getUniqueRequestId();
        FileManager::getInstance()->getFileList(m_filesRequestId, currentDir(), false);
    }
}

void FileDialog::onFileListReceived(const int& requestId, const QStringList& files)
{
    if (!((requestId == m_foldersRequestId) || (requestId == m_filesRequestId))) return;

    auto isFolder = requestId == m_foldersRequestId;

    for (const auto& file : files)
    {
        m_files.append(new FileObject(file, isFolder));
    }



    emit filesChanged();
}
