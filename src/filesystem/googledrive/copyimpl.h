#pragma once

#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileResult;

namespace GoogleDrive
{

/// Copy a file
/// 1. Make sure the destination file does not exist already
/// 2. Get file ID
/// 3. Get ID of destination folder
/// 4. Build MetaData
/// 5. Send copy request (POST)
class CopyImpl
{
public:
    static auto copy(const QString &path, const QString &copy, FileAccessGoogleDrive *fa) -> QFuture<FileResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QString copyPath;
        QString fileId;
        QString newParentId;
    };

    static auto copy(State &&state) -> QFuture<FileResult>;
    static auto onCheckSucceeded(State &&state) -> QFuture<FileResult>;
    static auto onFileIdReceived(State &&state) -> QFuture<FileResult>;
    static auto onNewParentIdReceived(State &&state) -> QFuture<FileResult>;
    static auto createNewParentDirAndTryAgain(State &&state) -> QFuture<FileResult>;
};

} // namespace GoogleDrive
} // namespace Files
