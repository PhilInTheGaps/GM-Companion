#include "filedialog.h"
#include "logging.h"
#include "filemanager.h"
#include "utils/utils.h"
//#include <utility>

FileObject::FileObject(const QString& name, bool isFolder, QObject *parent)
    : QObject(parent), m_name(name), m_isFolder(isFolder)
{
}

FileDialog::FileDialog(QObject *parent) : QObject(parent)
{
}

auto FileDialog::setCurrentDir(const QString& dir) -> void
{
    qCDebug(gmFileDialog()) << "Setting current dir:" << dir;
    m_currentDir = dir.split('/');
    emit currentDirChanged();

    clearForward();
    updateFileList();
}

auto FileDialog::enterFolder(int index) -> void
{
    qCDebug(gmFileDialog()) << "Entering folder" << index;

    if ((m_files.length() > index) && (index > -1))
    {
        auto *folder = qobject_cast<FileObject*>(m_files[index]);

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

    if (Utils::isInBounds(m_files, index))
    {
        selectedFolder.append(qobject_cast<FileObject*>(m_files[index])->name());
    }

    return FileUtils::dirFromFolders(selectedFolder);
}

auto FileDialog::forward() -> void
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

auto FileDialog::clearForward() -> void
{
    m_forwardFolders.clear();
    emit canForwardChanged();
}

auto FileDialog::back() -> void
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

auto FileDialog::updateFileList() -> void
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

auto FileDialog::onFileListReceived(const int& requestId, const QStringList& files) -> void
{
    if (!((requestId == m_foldersRequestId) || (requestId == m_filesRequestId))) return;

    auto isFolder = requestId == m_foldersRequestId;

    for (const auto& file : files)
    {
        m_files.append(new FileObject(file, isFolder));
    }



    emit filesChanged();
}
