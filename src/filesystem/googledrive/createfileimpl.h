#pragma once

#include <QByteArray>
#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileResult;

namespace GoogleDrive
{

/// Create a file or folder
/// 1. Make sure the file/folder does not exist already
/// 2. Get parent folder ID
/// 3. Build MetaData
/// 4. Send "create" request
class CreateFileImpl
{
public:
    static auto create(const QString &path, const QByteArray &mimeType, FileAccessGoogleDrive *fa)
        -> QFuture<FileResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QString parentId;
        QByteArray mimeType;
    };

    static auto create(State &&state) -> QFuture<FileResult>;
    static auto onFileCheckComplete(State &&state) -> QFuture<FileResult>;
    static auto onParentIdReceived(State &&state) -> QFuture<FileResult>;
    static auto createParentDirAndTryAgain(State &&state) -> QFuture<FileResult>;
};

} // namespace GoogleDrive
} // namespace Files
