#include "filedialog.h"
#include "file.h"
#include "fileobject.h"
#include "utils/utils.h"
#include <QLoggingCategory>

using namespace Files;

Q_LOGGING_CATEGORY(gmFileDialog, "gm.files.dialog")

FileDialog::FileDialog(QObject *parent) : QObject(parent), a_folderMode(false), a_isLoading(false)
{
    connect(this, &FileDialog::currentDirChanged, &FileDialog::onCurrentDirChanged);
}

void FileDialog::setCurrentDir(const QString &dir)
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
        const auto *folder = qobject_cast<FileObject *>(entries().at(index));

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
        selectedFolder.append(qobject_cast<FileObject *>(entries().at(index))->name());
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

void FileDialog::createFolder(const QString &folderName)
{
    const auto path = FileUtils::fileInDir(folderName, currentDir());

    FileAccess::getInstance()->createDirAsync(path).then(this, [this](FileResult *result) {
        if (!result)
        {
            qCWarning(gmFileDialog()) << "Error: createDirAsync returned a null result!";
            return;
        }

        if (!result->success())
        {
            qCWarning(gmFileDialog()) << result->errorMessage();
        }

        result->deleteLater();
        updateFileList();
    });
}

void FileDialog::updateFileList()
{
    qCDebug(gmFileDialog()) << "updateFileList()";

    clearFileList();
    stopCurrentRequest();

    isLoading(true);

    m_currentFuture = File::listAsync(currentDir(), !folderMode(), true);
    m_currentFuture.then(this, [this](FileListResult *result) { onFileListReceived(result); })
        .onCanceled(this, [this]() {
            qCDebug(gmFileDialog()) << "file list update was cancelled.";
            isLoading(false);
        });
}

void FileDialog::clearFileList()
{
    foreach (const auto &file, entries())
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

void FileDialog::onFileListReceived(FileListResult *result)
{
    QList<QObject *> objects;

    foreach (const auto &folder, result->folders())
    {
        objects.append(new FileObject(folder, true, this));
    }

    foreach (const auto &file, result->files())
    {
        objects.append(new FileObject(file, false, this));
    }

    isLoading(false);
    entries(objects);
    result->deleteLater();
}

void FileDialog::onCurrentDirChanged(const QString & /*dir*/)
{
    emit canGoForwardChanged(canGoForward());
    emit canGoBackChanged(canGoBack());
    updateFileList();
}
