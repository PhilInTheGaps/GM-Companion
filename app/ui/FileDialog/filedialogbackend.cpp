#include "filedialogbackend.h"
#include "file.h"
#include "fileobject.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QLoggingCategory>

using namespace Files;

Q_LOGGING_CATEGORY(gmFileDialog, "gm.files.dialog")

FileDialogBackend::FileDialogBackend(QObject *parent) : QObject(parent)
{
    connect(this, &FileDialogBackend::currentDirChanged, &FileDialogBackend::onCurrentDirChanged);
}

auto FileDialogBackend::currentDir() const -> QString
{
    return FileUtils::dirFromFolders(m_currentDir);
}

void FileDialogBackend::setCurrentDir(const QString &dir)
{
    qCDebug(gmFileDialog()) << "Setting current dir:" << dir;
    m_currentDir = dir.split('/');
    clearForward();

    emit currentDirChanged(dir);
}

auto FileDialogBackend::canGoForward() const -> bool
{
    return !m_forwardFolders.isEmpty();
}

auto FileDialogBackend::canGoBack() const -> bool
{
    return m_currentDir.length() > 1;
}

void FileDialogBackend::enterFolder(int index)
{
    if (Utils::isInBounds(entries(), index))
    {
        const auto *folder = entries().at(index);

        if (folder->isFolder())
        {
            qCDebug(gmFileDialog()) << "Entering folder" << index << "(" << folder->name() << ")";

            m_currentDir.append(folder->name());
            clearForward();
        }
    }
    else
    {
        qCDebug(gmFileDialog()) << "Tried to enter folder" << index << "but index is out of bounds.";
    }

    emit currentDirChanged(currentDir());
}

auto FileDialogBackend::getSelected(int index) const -> QString
{
    auto selectedFolder = m_currentDir;

    if (Utils::isInBounds(entries(), index))
    {
        selectedFolder.append(entries().at(index)->name());
    }

    return FileUtils::dirFromFolders(selectedFolder);
}

void FileDialogBackend::forward()
{
    qCDebug(gmFileDialog()) << "forward()";

    if (!m_forwardFolders.isEmpty())
    {
        m_currentDir.push_back(m_forwardFolders.takeFirst());
    }

    emit currentDirChanged(currentDir());
}

void FileDialogBackend::clearForward()
{
    m_forwardFolders.clear();
}

auto FileDialogBackend::back() -> void
{
    qCDebug(gmFileDialog()) << "back()";

    if (m_currentDir.length() > 1)
    {
        m_forwardFolders.push_front(m_currentDir.takeLast());
    }

    emit currentDirChanged(currentDir());
}

void FileDialogBackend::createFolder(const QString &folderName)
{
    const auto path = FileUtils::fileInDir(folderName, currentDir());

    File::createDirAsync(path).then([this](const FileResult &result) {
        if (!result.success())
        {
            qCWarning(gmFileDialog()) << result.errorMessage();
        }

        updateFileList();
    });
}

void FileDialogBackend::updateFileList()
{
    qCDebug(gmFileDialog()) << "updateFileList()";

    clearFileList();
    stopCurrentRequest();

    isLoading(true);

    m_currentFuture = File::listAsync(currentDir(), !folderMode(), true);
    m_currentFuture.then([this](const FileListResult &result) { onFileListReceived(result); }).onCanceled([this]() {
        qCDebug(gmFileDialog()) << "file list update was cancelled.";
        isLoading(false);
    });
}

void FileDialogBackend::clearFileList()
{
    if (a_entries.isEmpty()) return;

    foreach (const auto &file, entries())
    {
        if (file) file->deleteLater();
    }

    a_entries.clear();
    emit entriesChanged();
}

void FileDialogBackend::stopCurrentRequest()
{
    if (m_currentFuture.isRunning())
    {
        m_currentFuture.cancel();
    }
}

void FileDialogBackend::onFileListReceived(const FileListResult &result)
{
    auto countBefore = a_entries.count();

    foreach (const auto &folder, result.folders())
    {
        a_entries.append(new FileObject(folder, true, this));
    }

    foreach (const auto &file, result.files())
    {
        a_entries.append(new FileObject(file, false, this));
    }

    isLoading(false);

    if (countBefore != a_entries.count())
    {
        emit entriesChanged();
    }
}

void FileDialogBackend::onCurrentDirChanged(const QString & /*dir*/)
{
    emit canGoForwardChanged(canGoForward());
    emit canGoBackChanged(canGoBack());
    updateFileList();
}
