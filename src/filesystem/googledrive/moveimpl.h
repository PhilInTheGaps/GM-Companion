#pragma once

#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileResult;

namespace GoogleDrive
{

/// Move a file from one path to another
/// 1. Get file ID
/// 2. Get ID of old parent folder
/// 3. Get ID of new parent folder
/// 4. Build new metadata (update name)
/// 5. Send PATCH request
class MoveImpl
{
public:
    static auto move(const QString &oldPath, const QString &newPath, FileAccessGoogleDrive *fa) -> QFuture<FileResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString oldPath;
        QString newPath;
        QString fileId;
        QString oldParentId;
        QString newParentId;
    };

    static auto move(State &&state) -> QFuture<FileResult>;

    static auto onFileIdReceived(State &&state) -> QFuture<FileResult>;
    static auto onParentIdReceived(State &&state) -> QFuture<FileResult>;
    static auto onNewParentIdReceived(State &&state) -> QFuture<FileResult>;

    static auto createNewParentFolderAndTryAgain(State &&state) -> QFuture<FileResult>;
};

} // namespace GoogleDrive
} // namespace Files
