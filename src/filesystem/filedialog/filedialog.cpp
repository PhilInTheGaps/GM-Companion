#include "filedialog.h"
#include "fileobject.h"
#include "file.h"
#include "logging.h"
#include "utils/utils.h"

#include "thirdparty/asyncfuture/asyncfuture.h"

using namespace Files;
using namespace AsyncFuture;

FileDialog::FileDialog(QObject *parent)
    : QObject(parent), a_folderMode(false), a_isLoading(false)
{
    connect(this, &FileDialog::currentDirChanged, &FileDialog::onCurrentDirChanged);
}

void FileDialog::setCurrentDir(const QString& dir)
{
    qCDebug(gmFileDialog()) << "Setting current dir:" << dir;
    m_currentDir = dir.split('/');
    clearForward();

    emit currentDirChanged(dir);
}

void FileDialog::enterFolder(int index)
{
    if (Utils::isInBounds(entries(), index))
    {
        auto *folder = qobject_cast<FileObject*>(entries()[index]);

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

auto FileDialog::getSelected(int index) const -> QString
{
    auto selectedFolder = m_currentDir;

    if (Utils::isInBounds(entries(), index))
    {
        selectedFolder.append(qobject_cast<FileObject*>(entries()[index])->name());
    }

    return FileUtils::dirFromFolders(selectedFolder);
}

void FileDialog::forward()
{
    qCDebug(gmFileDialog()) << "forward()";

    if (!m_forwardFolders.isEmpty())
    {
        m_currentDir.push_back(m_forwardFolders.takeFirst());
    }

    emit currentDirChanged(currentDir());
}

void FileDialog::clearForward()
{
    m_forwardFolders.clear();
}

auto FileDialog::back() -> void
{
    qCDebug(gmFileDialog()) << "back()";

    if (m_currentDir.length() > 1)
    {
        m_forwardFolders.push_front(m_currentDir.takeLast());
    }

    emit currentDirChanged(currentDir());
}

void FileDialog::updateFileList()
{
    qCDebug(gmFileDialog()) << "updateFileList()";

    clearFileList();
    stopCurrentRequest();

    isLoading(true);

    m_currentFuture = File::listAsync(currentDir(), !folderMode(), true);
    observe(m_currentFuture).subscribe([this](FileListResult *result){
        onFileListReceived(result);
    }, [this](){
        qCDebug(gmFileDialog()) << "file list update was cancelled.";
        isLoading(false);
    });
}

void FileDialog::clearFileList()
{
    for (const auto& file : entries())
    {
        if (file) file->deleteLater();
    }

    entries({});
}

void FileDialog::stopCurrentRequest()
{
    if (m_currentFuture.isRunning())
    {
        m_currentFuture.cancel();
    }
}

void FileDialog::onFileListReceived(FileListResult* result)
{
    QList<QObject*> objects;

    for (const auto& folder : result->folders())
    {
        objects.append(new FileObject(folder, true, this));
    }

    for (const auto& file : result->files())
    {
        objects.append(new FileObject(file, false, this));
    }

    isLoading(false);
    entries(objects);
    result->deleteLater();
}

void FileDialog::onCurrentDirChanged(const QString &/*dir*/)
{
    emit canGoForwardChanged(canGoForward());
    emit canGoBackChanged(canGoBack());
    updateFileList();
}
