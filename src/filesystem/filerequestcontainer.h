#ifndef FILEREQUESTCONTAINER_H
#define FILEREQUESTCONTAINER_H

#include "fileaccess.h"

struct FileRequestContainer {
    FileRequestContainer(int requestId, FileAccess::FileRequest type)
        : requestId(requestId), requestType(type) {}

    /// Build container for GetFile request
    static FileRequestContainer getFile(int id, const QString& filePath) {
        FileRequestContainer container(id, FileAccess::GetFile);
        container.string1 = filePath;
        return container;
    }

    /// Build container for GetFiles request
    static FileRequestContainer getFiles(int id, const QString& directory, const QString& fileEnding) {
        FileRequestContainer container(id, FileAccess::GetFiles);
        container.string1 = directory;
        container.string2 = fileEnding;
        return container;
    }

    /// Build container for GetFileList request
    static FileRequestContainer getFileList(int id, const QString& directory, const bool& folders) {
        FileRequestContainer container(id, FileAccess::GetFileList);
        container.string1 = directory;
        container.folders = folders;
        return container;
    }

    /// Build container for SaveFile request
    static FileRequestContainer saveFile(int id, const QString& filePath, const QByteArray& data) {
        FileRequestContainer container(id, FileAccess::SaveFile);
        container.string1 = filePath;
        container.data = data;
        return container;
    }

    /// Build container for RenameFile request
    static FileRequestContainer renameFile(int id, const QString& newFile, const QString& oldFile, const QByteArray& data) {
        FileRequestContainer container(id, FileAccess::RenameFile);
        container.string1 = newFile;
        container.string2 = oldFile;
        container.data = data;
        return container;
    }

    /// Build container for RenameFolder request
    static FileRequestContainer renameFolder(int id, const QString& newFolder, const QString& oldFolder) {
        FileRequestContainer container(id, FileAccess::RenameFolder);
        container.string1 = newFolder;
        container.string2 = oldFolder;
        return container;
    }

    /// Build container for RemoveFile request
    static FileRequestContainer removeFile(int id, const QString& filePath) {
        FileRequestContainer container(id, FileAccess::RemoveFile);
        container.string1 = filePath;
        return container;
    }

    /// Build container for CreateFolder request
    static FileRequestContainer createFolder(int id, const QString& filePath) {
        FileRequestContainer container(id, FileAccess::CreateFolder);
        container.string1 = filePath;
        return container;
    }

    /// Build container for CheckIfFilesExist request
    static FileRequestContainer checkIfFilesExist(int id, const QStringList& files) {
        FileRequestContainer container(id, FileAccess::CheckIfFilesExist);
        container.files = files;
        return container;
    }

    int requestId = -1;
    bool folders = false;
    QString string1, string2;
    QByteArray data;
    QStringList files;
    FileAccess::FileRequest requestType;
};

#endif // FILEREQUESTCONTAINER_H
